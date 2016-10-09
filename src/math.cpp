#include "math.hpp"
#include <iostream>

namespace Math {
	std::ostream& operator<<(std::ostream &os, e_type const& type)
	{
		switch(type) {
			case e_value: return os << "value";
			case e_variable: return os << "variable";
			case e_negative: return os << "-";
			case e_cosine: return os << "cos";
			case e_sine: return os << "sin";
			case e_tangent: return os << "tan";
			case e_secant: return os << "sec";
			case e_cosecant: return os << "csc";
			case e_cotangent: return os << "cot";
			case e_arccosine: return os << "acos";
			case e_arcsine: return os << "asin";
			case e_arctangent: return os << "atan";
			case e_logarithm: return os << "log";
			case e_sum: return os << "+";
			case e_difference: return os << "-";
			case e_product: return os << "*";
			case e_ratio: return os << "/";
			case e_power: return os << "^";
			default: return os;
		}
	}
}
