#include "math.hpp"
#include <iostream>

namespace Math {
	e_type variable::get_type(void) const
	{
		return e_variable;
	}
	bool variable::constant(void) const
	{
		return false;
	}
	bool variable::varies(e_param p) const
	{
		return p == param;
	}
	e_order variable::order(void) const
	{
		return e_order_term;
	}
	bool variable::operator==(function const& f) const
	{
		if(f.get_type() != e_variable) {
			return false;
		}
		return (static_cast<const variable*>(&f)->param) == param;
	}
	float variable::operator()(float r, float s, float t,
			float u, float v, float w) const
	{
		float values[] = {r,s,t,u,v,w};
		return values[param];
	}
	function* variable::substitute(e_param p, function *fn) const
	{
		return param == p ? fn -> clone() : clone();
	}
	function* variable::bind(e_param p, float val) const
	{
		return param == p ? new value(val) : clone();
	}
	function* variable::derive(e_param p) const
	{
		return p == param ? new value(1) : new value(0);
	}
	function* variable::clone(void) const
	{
		return new variable(param);
	}
	function* variable::reduce(void) const
	{
		return clone();
	}
	std::ostream& variable::print(std::ostream &os) const
	{
		static constexpr const char *labels = "rstuvw";
		return os << labels[param];
	}
	variable::variable(e_param param):
		param(param), function() {}
	variable::~variable(void){}
}
