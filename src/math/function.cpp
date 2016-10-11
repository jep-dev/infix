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

	std::ostream& operator<<(std::ostream &os, function const& fn)
	{
		return fn.print(os);
	}
}
