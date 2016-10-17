#include <algorithm>
#include <iostream>
#include <functional>
#include "math.hpp"

namespace Math {
	std::ostream& operator<<(std::ostream &os, e_type const& type)
	{
		switch(type) {
			case e_value: return os << "value";
			case e_variable: return os << "variable";
			case e_negative: return os << "negative";
			case e_cosine: return os << "cosine";
			case e_sine: return os << "sine";
			case e_tangent: return os << "tangent";
			case e_secant: return os << "secant";
			case e_cosecant: return os << "cosecant";
			case e_cotangent: return os << "cotangent";
			case e_arccosine: return os << "arccosine";
			case e_arcsine: return os << "arcsine";
			case e_arctangent: return os << "arctangent";
			case e_logarithm: return os << "logarithm";
			case e_sum: return os << "sum";
			case e_difference: return os << "difference";
			case e_product: return os << "product";
			case e_ratio: return os << "ratio";
			case e_power: return os << "exponent";
			default: return os;
		}
	}

	std::ostream& operator<<(std::ostream &os, e_order const& order)
	{
		static constexpr const char *strings[] = {
			"term", "sum", "ratio", "product", "exponent", ""
		};
		return os << strings[order];
	}

	std::ostream& operator<<(std::ostream &os, e_param const& param)
	{
		return os << (char)(param+'r');
	}

	void add_factor(function *fn, bool sign, std::vector<power*> &vec)
	{
		function *lbase, *lexp;
		switch(fn -> get_type()) {
			case e_power: {
				auto cast = static_cast<const power*>(fn);
				lbase = cast -> base -> clone();
				lexp = cast -> exp -> clone();
			} break;
			case e_secant: {
				auto cast = static_cast<const secant*>(fn);
				lbase = new cosine(cast -> operand -> clone());
				lexp = new value(-1);
			} break;
			case e_cosecant: {
				auto cast = static_cast<const cosecant*>(fn);
				lbase = new sine(cast -> operand -> clone());
				lexp = new value(-1);
			} break;
			case e_cotangent: {
				auto cast = static_cast<const cotangent*>(fn);
				add_factor(new cosine(cast -> operand -> clone()), sign, vec);
				add_factor(new sine(cast -> operand -> clone()), !sign, vec);
				delete fn;
				return;
			} break;
			case e_tangent: {
				auto cast = static_cast<const tangent*>(fn);
				add_factor(new sine(cast -> operand -> clone()), sign, vec);
				add_factor(new cosine(cast -> operand -> clone()), !sign, vec);
				delete fn;
				return;
			} break;
			default: {
				lbase = fn -> clone();
				lexp = new value(1);
			} break;
		}
		if(!sign) {
			lexp = new negative(lexp);
		}
		bool found = false;
		for(auto it = std::begin(vec); it != std::end(vec);) {
			auto rbase = (*it) -> base, rexp = (*it) -> exp;
			if(*lbase == *rbase) {
				found = true;
				function *exp = new sum(lexp, rexp -> clone());
				delete *it;
				/*
 				*it = new power(lbase, exp -> reduce());
				delete exp;
				*/
				*it = new power(lbase, exp);
				break;
			}
			it++;
		}
		if(!found) {
			vec.emplace_back(new power(lbase, lexp));
		}
		delete fn;
	}
	void add_factors(function *fn, bool sign, std::vector<power*> &factors)
	{
		switch(fn -> get_type()) {
			case e_product: {
				auto cast = static_cast<const product*>(fn);
				add_factors(cast -> lhs -> clone(), sign, factors);
				add_factors(cast -> rhs -> clone(), sign, factors);
			} break;
			case e_ratio: {
				auto cast = static_cast<const ratio*>(fn);
				add_factors(cast -> numerator -> clone(), sign, factors);
				add_factors(cast -> denominator -> clone(), !sign, factors);
			} break;
			case e_negative: {
				auto cast = static_cast<const negative*>(fn);
				add_factors(new value(-1), sign, factors);
				add_factors(cast -> operand -> clone(), sign, factors);
			} break;
			default:
				add_factor(fn -> clone(), sign, factors);
				break;
		}
		delete fn;
	}
	void get_factors(function const& fn, std::vector<power*> &factors)
	{
		add_factors(fn.reduce(), true, factors);
		auto compare = [] (power *a, power *b) -> bool {
			auto lbase = a -> base, lexp = a -> exp,
				 rbase = b -> base, rexp = b -> exp;
			return *rexp < *lexp || (*lexp == *rexp && *lbase < *rbase);
		};
		std::sort(std::begin(factors), std::end(factors), compare);
		for(auto it = std::begin(factors); it != std::end(factors);) {
			auto reduced = (*it) -> reduce();
			if(*reduced == 1) {
				delete *it;
				it = factors.erase(it);
			} else {
				it++;
			}
			delete reduced;
		}
	}
	
	void add_term(function *fn, bool sign, std::vector<product*> &vec)
	{
		bool found = false, split = false;
		function *ll, *lr;
		if(fn -> get_type() == e_product) {
			auto cast = static_cast<const product*>(fn);
			function *lhs = cast -> lhs -> reduce(),
					 *rhs = cast -> rhs -> reduce();
			if(lhs -> constant()) {
				auto lval = (*lhs)(0,0,0,0,0,0);
				if(rhs -> constant()) {
					lval = (*rhs)(0,0,0,0,0,0) * lval;
					ll = new value(sign ? lval : -lval);
					lr = new value(1);
					delete rhs;
				} else {
					ll = new value(sign ? lval : -lval);
					lr = rhs;
				}
				delete lhs;
				split = true;
			} else if(rhs -> constant()) {
				auto rval = (*rhs)(0,0,0,0,0,0);
				ll = new value(sign ? rval : -rval);
				lr = lhs;
				delete rhs;
				split = true;
			} else {
				delete lhs;
				delete rhs;
			}
		}
		if(!split) {
			ll = new value(sign ? 1 : -1);
			lr = fn -> clone();
		}
		for(auto it = std::begin(vec); it != std::end(vec);) {
			auto rl = (*it) -> lhs -> clone(),
				 rr = (*it) -> rhs -> clone();
			if(*lr == *rr) {
				found = true;
				delete rr;
				delete *it;
				*it = new product(new sum(ll, rl), lr);
				break;
			}
			delete rl;
			delete rr;
			it++;
		}
		if(!found) {
			vec.emplace_back(new product(ll, lr));
		}
		delete fn;
	}
	void add_terms(function *fn, bool sign, std::vector<product*> &terms)
	{
		switch(fn -> get_type()) {
			case e_sum: {
				auto cast = static_cast<const sum*>(fn);
				add_terms(cast -> lhs -> clone(), sign, terms);
				add_terms(cast -> rhs -> clone(), sign, terms);
			} break;
			case e_difference: {
				auto cast = static_cast<const difference*>(fn);
				add_terms(cast -> lhs -> clone(), sign, terms);
				add_terms(cast -> rhs -> clone(), !sign, terms);
			} break;
			case e_negative: {
				auto cast = static_cast<const negative*>(fn);
				add_terms(cast -> operand -> clone(), !sign, terms);
			} break;
			default:
				add_term(fn -> clone(), sign, terms);
				break;
		}
		delete fn;
	}
	void get_terms(function const& fn, std::vector<product*> &terms)
	{
		add_terms(fn.clone(), true, terms);
		auto compare = [] (product *a, product *b) -> bool {
			return *a < *b;
		};
		std::sort(std::begin(terms), std::end(terms), compare);
		for(auto it = std::begin(terms); it != std::end(terms);) {
			function *fn = (*it) -> reduce();
			if(*fn == 0) {
				delete *it;
				it = terms.erase(it);
			} else {
				it++;
			}
			delete fn;
		}
	}
}
