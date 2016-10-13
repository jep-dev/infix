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

	void get_factors(function const& fn,
			std::vector<function*> &num, std::vector<function*> &den)
	{
		auto add = [] (function *fn, std::vector<function*> &vec) {
			function *lbase, *lexp;
			if(fn -> get_type() == e_power) {
				auto cast = static_cast<const power*>(fn);
				lbase = cast -> base -> clone();
				lexp = cast -> exp -> clone();
			} else {
				lbase = fn -> clone();
				lexp = new value(1);
			}
			bool found = false;
			for(auto vit = std::begin(vec); vit != std::end(vec);) {
				auto cast = static_cast<const power*>(*vit);
				function *rbase = cast -> base -> clone(),
					*rexp = cast -> exp -> clone();
				if(*lbase == *rbase) {
					found = true;
					delete *vit;
					function *exp = new sum(lexp -> clone(), rexp -> clone());
					*vit = new power(lbase -> clone(), exp -> reduce());
					delete exp;
				}
				delete rbase;
				delete rexp;
				if(found) break;
				else vit++;
			}
			if(!found) {
				vec.emplace_back(new power(lbase, lexp));
			} else {
				delete lbase;
				delete lexp;
			}
		};
		std::function<void(function*, std::vector<function*>&,
				std::vector<function*>&)> process;
		process = [&] (function* fn, std::vector<function*> &num,
				std::vector<function*> &den) {
			auto f_type = fn -> get_type();
			auto f_order = fn -> get_order();
			if(f_order == e_order_product) {
				auto cast = static_cast<const product*>(fn);
				process(cast -> lhs -> clone(), num, den);
				process(cast -> rhs -> clone(), num, den);
			} else if(f_order == e_order_ratio) {
				auto cast = static_cast<const ratio*>(fn);
				process(cast -> numerator -> clone(), num, den);
				process(cast -> denominator -> clone(), den, num);
			} else if(f_type == e_negative) {
				auto cast = static_cast<const negative*>(fn);
				process(new value(-1), num, den);
				process(cast -> operand -> clone(), num, den);
			} else {
				add(fn, num);
			}
			delete fn;
		};
		process(fn.reduce(), num, den);
		auto compare = [](function *a, function *b) -> bool {
			return *a < *b;
		};
		std::sort(std::begin(num), std::end(num), compare);
		std::sort(std::begin(den), std::end(den), compare);
		auto reduce = [] (std::vector<function*> &vec) {
			for(auto it = std::begin(vec); it != std::end(vec);) {
				auto cast = static_cast<const power*>(*it);
				if(*(cast -> exp) == 0) {
					delete *it;
					vec.erase(it);
				} else {
					it++;
				}
			}
		};
		reduce(num);
		reduce(den);
	}

	void get_terms(function const& fn, std::vector<function*> &pos,
			std::vector<function*> &neg)
	{
		std::function<void(function*, std::vector<function*>&,
				std::vector<function*>&)> process;
		process = [&] (function *fn, std::vector<function*> &pos,
				std::vector<function*> &neg) {
			auto f_type = fn -> get_type();
			auto f_order = fn -> get_order();
			if(f_order == e_order_sum) {
				if(f_type == e_sum) {
					auto cast = static_cast<const sum*>(fn);
					process(cast -> lhs -> clone(), pos, neg);
					process(cast -> rhs -> clone(), pos, neg);
				} else if(f_type == e_difference) {
					auto cast = static_cast<const difference*>(fn);
					process(cast -> lhs -> clone(), pos, neg);
					process(cast -> rhs -> clone(), neg, pos);
				}
				delete fn;
			} else if(f_type == e_negative) {
				auto cast = static_cast<const negative*>(fn);
				process(cast -> operand -> clone(), neg, pos);
				delete fn;
			} else {
				pos.emplace_back(fn);
			}
		};
		process(fn.reduce(), pos, neg);
		
		auto compare = [](function *a, function *b) -> bool {
			return *a < *b;
		};
		std::sort(std::begin(pos), std::end(pos), compare);
		std::sort(std::begin(neg), std::end(neg), compare);
	}

}
