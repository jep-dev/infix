#include "math.hpp"
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

	function* function::reduce(function const& fn)
	{
		function *total = nullptr;
		std::vector<product*> terms;
		get_terms(fn, terms);
		for(auto term : terms) {
			function *reduced = term -> reduce();
			std::vector<power*> factors;
			get_factors(*reduced, factors);
			delete reduced;
			delete term;

			function *factored = nullptr;
			//for(auto factor : factors) {
			for(auto it = std::begin(factors); it != std::end(factors);) {
				auto factor = *it;
				if(factored)
					factored = new product(factored, factor -> reduce());
				else
					factored = factor -> reduce();
				delete *it;
				it = factors.erase(it);
				/*if(factored) factored = new product(factored, factor);
				else factored = factor;*/
			}
			if(!factored) {
				factored = new value(1);
			}
			total = total ? new sum(total, factored) : factored;
		}
		return total ? total : new value(0);
		//return fn.reduce();
	}

	std::ostream& operator<<(std::ostream &os, function const& fn)
	{
		//auto reduced = function::reduce(fn);
		auto reduced = fn.reduce();
		reduced -> print(os);
		delete reduced;
		return os;
		//return fn.print(os);
	}
}
