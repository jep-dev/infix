#include "math.hpp"
#include <iostream>

namespace Math {
	e_type value::get_type(void) const
	{
		return e_value;
	}
	bool value::constant(void) const
	{
		return true;
	}
	bool value::varies(e_param p) const
	{
		return false;
	}
	e_order value::order(void) const 
	{
		return e_order_term;
	}
	bool value::operator==(function const& f) const
	{
		if(f.get_type() != e_value) {
			return false;
		}
		return (static_cast<const value*>(&f)->val) == val;
	}
	float value::operator()(float r, float s, float t,
			float u, float v, float w) const
	{
		return val;
	}
	function* value::substitute(e_param p, function *fn) const
	{
		return clone();
	}
	function* value::bind(e_param p, float val) const
	{
		return clone();
	}
	function* value::derive(e_param p) const
	{
		return new value(0);
	}
	function* value::clone(void) const
	{
		return new value(val);
	}
	function* value::reduce(void) const
	{
		return clone();
	}
	std::ostream& value::print(std::ostream &os) const
	{
		return os << val;
	}
	value::value(float val):
		val(val), function() {}
	value::~value(void) {}
}
