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

}
