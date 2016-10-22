#include "math.hpp"
#include <algorithm>
#include <iostream>

namespace Math {
	bool function::operator!=(function const& f) const
	{
		return !(*this == f);
	}
	bool function::operator==(float f) const
	{
		return constant() && (*this)(0,0,0,0,0,0) == f;
	}
	bool function::operator<(function const& f) const
	{
		auto lconst = constant(), rconst = f.constant();
		if(lconst && rconst) {
			return (*this)(0,0,0,0,0,0) < f(0,0,0,0,0,0);
		} else if(lconst) {
			return true;
		} else if(rconst) {
			return false;
		}
		int lcomp = 0, rcomp = 0;
		for(int i = 0; i < e_param_total; i++) {
			lcomp = (lcomp << 1) | varies((e_param)i);
			rcomp = (rcomp << 1) | f.varies((e_param)i);
		}
		if(lcomp == rcomp) {
			return get_type() < f.get_type();
		}
		return rcomp < lcomp;
	}

	function::function(void) {}
	function::~function(void) {}

	template<> void erase_all(std::vector<function*> &vec)
	{
		for(auto f : vec) delete f;
		vec.clear();
	}
	template<> void erase_all(std::vector<product*> &vec)
	{
		for(auto f : vec) delete f;
		vec.clear();
	}
	template<> void erase_all(std::vector<power*> &vec)
	{
		for(auto f : vec) delete f;
		vec.clear();
	}

	function* function::reduce2(function const& fn)
	{
		auto factorized = factorize(fn);
		std::vector<power*> factors2, factors1 = get_factors(*factorized);
		delete factorized;
		for(auto factor : factors1) {
			auto factor_base = factor -> base, factor_exp = factor -> exp;
			auto split_base = factorize(*factor_base);
			auto split_factors = get_factors(*split_base);
			delete split_base;
			for(auto split_factor : split_factors) {
				add_factors(new power(split_factor, factor_exp -> clone()),
						true, factors2);
			}
		}
		erase_all(factors1);
		return get_product(factors2, true);
	}

	function* function::reduce(function const& fn)
	{
 		function *total = nullptr;
		auto terms = get_terms(fn);
		for(auto term : terms) {
			function *reduced = term -> reduce();
			std::vector<power*> factors = get_factors(*reduced);
			delete reduced;
			auto factored = get_product(factors, true);
			total = total ? new sum(total, factored) : factored;
		}
		erase_all(terms);
		return total ? total : new value(0);
	}

	std::ostream& operator<<(std::ostream &os, function const& fn)
	{
		auto reduced = function::reduce(fn);
		reduced -> print(os);
		delete reduced;
		/*auto factorized = factorize(fn);
		factorized -> print(os);
		delete factorized;*/
		return os;
	}

	
	std::vector<product*> shared_terms(std::vector<product*> const& a,
			std::vector<product*> const& b)
	{
		std::vector<product*> output;
		for(auto a_it = std::begin(a); a_it != std::end(a); a_it++) {
			auto a_lhs = (*a_it) -> lhs, a_rhs = (*a_it) -> rhs;
			auto a_val = (*a_lhs)(0,0,0,0,0,0);
			for(auto b_it = std::begin(b); b_it != std::end(b); b_it++) {
				auto b_lhs = (*b_it) -> lhs, b_rhs = (*b_it) -> rhs;
				if(*a_rhs == *b_rhs) {
					auto b_val = (*b_lhs)(0,0,0,0,0,0), c_val = 0.0f;
					if(a_val >= 0 && b_val >= 0) {
						c_val = a_val > b_val ? b_val : a_val;
					} else if(a_val <= 0 && b_val <= 0) {
						c_val = a_val < b_val ? b_val : a_val;
					}
					if(c_val != 0) {
						output.emplace_back(new product(new value(c_val),
								b_rhs -> clone()));
						break;
					}
				}
			}
		}
		return output;
	}
	function* factorize(function const& fn)
	{
		auto terms = get_terms(fn);
		std::vector<power*> shared;
		for(auto it = std::begin(terms); it != std::end(terms); it++) {
			if(it == std::begin(terms)) {
				shared = get_factors(**it);
			} else {
				auto factors = get_factors(**it);
				for(auto l_it = std::begin(shared); l_it != std::end(shared);) {
					auto lhs = *l_it;
					bool found = false;
					auto l_base = lhs -> base, l_exp = lhs -> exp;
					for(auto rhs : factors) {
						auto r_base = rhs -> base, r_exp = rhs -> exp;
						if(*l_base == *r_base) {
							auto l_terms = get_terms(*l_exp),
								 r_terms = get_terms(*r_exp),
								 lr_terms = shared_terms(l_terms, r_terms);
							erase_all(l_terms);
							erase_all(r_terms);
							delete *l_it;
							auto lr_sum = get_sum(lr_terms, true);
							*l_it = new power(r_base -> clone(), lr_sum);
							l_it++;
							found = true;
							break;
						}
					}
					if(!found) {
						l_it = shared.erase(l_it);
					}
				}
				erase_all(factors);
			}
		}
		function *lhs = nullptr;
		for(auto it = std::begin(terms); it != std::end(terms);) {
			auto factors = get_factors(**it);
			std::vector<power*> factored;
			for(auto lhs : factors) {
				bool found = false;
				for(auto rhs : shared) {
					if(*(lhs -> base) == *(rhs -> base)) {
						found = true;
						auto base = rhs -> base -> clone();
						auto exp = new difference(lhs -> exp -> clone(),
								rhs -> exp -> clone());
						auto exp_red = exp -> reduce();
						if(*exp_red == 0) {
							delete base;
							delete exp_red;
						} else {
							factored.emplace_back(new power(base, exp_red));
						}
						delete exp;
						break;
					}
				}
				if(!found) {
					factored.emplace_back(
							static_cast<power*>(lhs -> clone()));
				}
			}
			erase_all(factors);
			delete *it;
			auto term = get_product(factored, true);
			auto term_red = term -> reduce();
			delete term;
			lhs = lhs ? new sum(lhs, term_red) : term_red;
			it++;
		}
		auto rhs = get_product(shared, true);
		return new product(lhs ? lhs : new value(1), rhs);
	}
	
	function* get_product(std::vector<power*> &factors, bool collect)
	{
		function *output = nullptr;
		for(auto it = std::begin(factors); it != std::end(factors);) {
			auto term = output ? new product(output, (*it) -> clone())
				: (*it) -> clone();
			output = term -> reduce();
			delete term;
			if(collect) {
				delete *it;
				it = factors.erase(it);
			} else {
				it++;
			}
		}
		return output ? output : new value(1);
	}

	function* get_sum(std::vector<product*> &terms, bool collect)
	{
		function *output = nullptr;
		for(auto it = std::begin(terms); it != std::end(terms);) {
			auto term = (*it) -> reduce();
			if(output) output = new sum(output, term);
			else output = term;
			if(collect) {
				delete *it;
				it = terms.erase(it);
			} else {
				it++;
			}
		}
		return output ? output : new value(0);
	}

	void add_factor(function *fn, bool sign, std::vector<power*> &vec)
	{
		function *lbase, *lexp;
		switch(fn -> get_type()) {
			case e_power: {
				auto cast = static_cast<const power*>(fn);
				if(cast -> base -> get_type() == e_power) {
					auto cast_base = static_cast<const power*>(cast -> base);
					auto inner_base = cast_base -> base -> clone(),
						 inner_exp = cast_base -> exp -> clone();
					lbase = inner_base;
					lexp = new product(inner_exp, cast -> exp -> clone());
				} else {
					lbase = cast -> base -> clone();
					lexp = cast -> exp -> clone();
				}
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
				add_factor(fn, sign, factors);
				return;
		}
		delete fn;
	}

	std::vector<power*> get_factors(function const& fn)
	{
		std::vector<power*> factors;
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
		return factors;
	}

	void add_term(function *fn, bool sign, std::vector<product*> &vec)
	{
		bool found = false, split = false;
		function *ll, *lr;
		switch(fn -> get_type()) {
			case e_product: {
				auto cast = static_cast<const product*>(fn);
				function *lhs = cast -> lhs -> reduce(),
					 	 *rhs = cast -> rhs -> reduce();
				bool lconst = lhs -> constant(), rconst = rhs -> constant();
				if(lconst) {
					if(rconst) {
						auto val = (*fn)(0,0,0,0,0,0);
						ll = new value(sign ? val : -val);
						lr = new value(1);
						delete rhs;
					} else {
						auto val = (*lhs)(0,0,0,0,0,0);
						ll = new value(sign ? val : -val);
						lr = rhs;
					}
					delete lhs;
					split = true;
				} else {
					if(rconst) {
						auto val = (*rhs)(0,0,0,0,0,0);
						ll = new value(sign ? val : -val);
						lr = lhs;
						split = true;
					} else {
						delete lhs;
					}
					delete rhs;
				}
			} break;
			case e_value: {
				auto val = (*fn)(0,0,0,0,0,0);
				ll = new value(sign ? val : -val);
				lr = new value(1);
				split = true;
			} break;
			default:
				break;
		}
		if(split) {
			delete fn;
		} else {
			ll = new value(sign ? 1 : -1);
			lr = fn;
		}
		for(auto it = std::begin(vec); it != std::end(vec); it++) {
			auto rl = (*it) -> lhs, rr = (*it) -> rhs;
			if(*lr == *rr) {
				found = true;
				auto updated = new product(new sum(ll, rl -> clone()), lr);
				delete *it;
				*it = updated;
				break;
			}
		}
		if(!found) {
			vec.emplace_back(new product(ll, lr));
		}
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
				add_term(fn, sign, terms);
				return;
		}
		delete fn;
	}
	std::vector<product*> get_terms(function const& fn)
	{
		std::vector<product*> terms;
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
		return terms;
	}
}
