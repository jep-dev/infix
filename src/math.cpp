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

	void get_factors(function const& fn, std::vector<function*> &factors)
	{
		auto add = [] (function const& fn, bool sign,
				std::vector<function*> &vec) {
			function *lbase, *lexp;
			if(fn.get_type() == e_power) {
				auto cast = static_cast<const power*>(&fn);
				lbase = cast -> base -> clone();
				lexp = cast -> exp -> clone();
			} else {
				lbase = fn.clone();
				lexp = new value(1);
			}
			if(!sign) {
				lexp = new negative(lexp);
			}
			bool found = false;
			for(auto it = std::begin(vec); it != std::end(vec);) {
				auto cast = static_cast<const power*>(*it);
				if(*lbase == *(cast -> base)) {
					found = true;
					function *exp = new sum(lexp, cast -> exp -> clone());
					delete *it;
					*it = new power(lbase, exp -> reduce());
					delete exp;
					break;
				}
				it++;
			}
			if(!found) {
				vec.emplace_back(new power(lbase, lexp));
			}
		};
		std::function<void(function*, bool)> process;
		process = [&] (function *fn, bool sign) {
			switch(fn -> get_type()) {
				case e_product: {
					auto cast = static_cast<const product*>(fn);
					process(cast -> lhs -> clone(), sign);
					process(cast -> rhs -> clone(), sign);
				} break;
				case e_ratio: {
					auto cast = static_cast<const ratio*>(fn);
					process(cast -> numerator -> clone(), sign);
					process(cast -> denominator -> clone(), !sign);
				} break;
				case e_negative: {
					auto cast = static_cast<const negative*>(fn);
					process(new value(-1), sign);
					process(cast -> operand -> clone(), sign);
				} break;
				default:
					add(*fn, sign, factors);
					break;
			}
			delete fn;
		};
		process(fn.reduce(), true);
		auto compare = [] (function *a, function *b) -> bool {
			auto lcast = static_cast<const power*>(a),
				 rcast = static_cast<const power*>(b);
			auto lbase = lcast -> base, lexp = lcast -> exp,
				 rbase = rcast -> base, rexp = rcast -> exp;
			return *rexp < *lexp || (*lexp == *rexp && *lbase < *rbase);
		};
		std::sort(std::begin(factors), std::end(factors), compare);
		for(auto it = std::begin(factors); it != std::end(factors);) {
			auto cast = static_cast<const power*>(*it);
			if(*(cast -> exp) == 0 || *(cast -> base) == 1) {
				delete *it;
				it = factors.erase(it);
			} else {
				it++;
			}
		}
	}
	
	void get_terms(function const& fn, std::vector<function*> &terms)
	{
		auto add = [] (function *fn, bool sign, std::vector<function*> &vec) {
			bool found = false, split = false;
			function *ll, *lr;
			if(fn -> get_type() == e_product) {
				auto cast = static_cast<const product*>(fn);
				if(cast -> lhs -> constant()) {
					ll = cast -> lhs -> clone();
					lr = cast -> rhs -> clone();
					split = true;
				} else if(cast -> rhs -> constant()) {
					ll = cast -> rhs -> clone();
					lr = cast -> lhs -> clone();
					split = true;
				}
			}
			if(!split) {
				ll = new value(1);
				lr = fn -> clone();
			}
			if(!sign) {
				ll = new negative(ll);
			}
			for(auto it = std::begin(vec); it != std::end(vec);) {
				auto cast = static_cast<const product*>(*it);
				auto rl = cast -> lhs -> clone(),
					 rr = cast -> rhs -> clone();
				if(*lr == *rr) {
					found = true;
					delete rr;
					delete *it;
					function *lhs = new sum(ll, rl),
						*lhs_red = lhs -> reduce();
					delete lhs;
					*it = new product(lhs_red, lr);
					break;
				}
				delete rl;
				delete rr;
				it++;
			}
			if(!found) {
				vec.emplace_back(new product(ll, lr));
			}
		};
		std::function<void(function*, bool)> process;
		process = [&] (function *fn, bool sign) {
			switch(fn -> get_type()) {
				case e_sum: {
					auto cast = static_cast<const sum*>(fn);
					process(cast -> lhs -> clone(), sign);
					process(cast -> rhs -> clone(), sign);
				} break;
				case e_difference: {
					auto cast = static_cast<const difference*>(fn);
					process(cast -> lhs -> clone(), sign);
					process(cast -> rhs -> clone(), !sign);
				} break;
				case e_negative: {
					auto cast = static_cast<const negative*>(fn);
					process(cast -> operand -> clone(), !sign);
				} break;
				default:
					add(fn, sign, terms);
					break;
			}
			delete fn;
		};
		process(fn.clone(), true);
		auto compare = [] (function *a, function *b) -> bool {
			auto lcast = static_cast<const product*>(a),
				 rcast = static_cast<const product*>(b);
			auto ll = lcast -> lhs, lr = lcast -> rhs,
				 rl = rcast -> lhs, rr = rcast -> rhs;
			auto ll_val = (*ll)(0,0,0,0,0,0),
				 rl_val = (*rl)(0,0,0,0,0,0);
			bool lpos = ll_val > 0, rpos = rl_val > 0;
			return (lpos && !rpos) || (lpos && rpos && *lr < *rr);
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
