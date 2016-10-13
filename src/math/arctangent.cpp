#include "math.hpp"
#include <iostream>

namespace Math {
	e_type arctangent::get_type(void) const
	{
		return e_arctangent;
	}
	e_order arctangent::get_order(void) const
	{
		return e_order_term;
	}
	bool arctangent::constant(void) const
	{
		return operand -> constant();
	}
	bool arctangent::varies(e_param p) const
	{
		return operand -> varies(p);
	}
	bool arctangent::operator==(function const& f) const
	{
		if(f.get_type() != e_arctangent) {
			return false;
		}
		return *(static_cast<const arctangent*>(&f)->operand) == *operand;
	}
	float arctangent::operator()(float r, float s, float t,
			float u, float v, float w) const
	{
		return atan((*operand)(r, s, t, u, v, w));
	}
	function* arctangent::substitute(e_param p, function *fn) const
	{
		auto output = new arctangent(operand -> substitute(p, fn));
		auto reduced = output -> reduce();
		delete output;
		return reduced;
	}
	function* arctangent::bind(e_param p, float val) const
	{
		auto output = new arctangent(operand -> bind(p, val));
		auto reduced = output -> reduce();
		delete output;
		return reduced;
	}
	function* arctangent::derive(e_param p) const
	{
		auto output = new ratio(operand -> derive(p),
				new sum(new value(1),
					new power(operand->clone(), new value(2))));
		auto reduced = output -> reduce();
		delete output;
		return reduced;
	}
	function* arctangent::clone(void) const
	{
		return new arctangent(operand -> clone());
	}
	function* arctangent::reduce(void) const
	{
		if(operand -> constant()) {
			return new value(atan((*operand)(0,0,0,0,0,0)));
		}
		if(operand -> get_type() == e_tangent) {
			return static_cast<tangent*>(operand) -> operand -> reduce();
		}
		return new arctangent(operand -> reduce());
	}
	std::ostream& arctangent::print(std::ostream &os) const
	{
		return os << "atan(" << *operand << ")";
	}
	arctangent::arctangent(function *operand):
		operand(operand), function() {}
	arctangent::~arctangent(void)
	{
		delete operand;
	}
}
