#include "math.hpp"
#include <iostream>

namespace Math {
	e_type tangent::get_type(void) const
	{
		return e_tangent;
	}
	e_order tangent::get_order(void) const
	{
		return e_order_term;
	}
	bool tangent::constant(void) const
	{
		return operand -> constant();
	}
	bool tangent::varies(e_param p) const
	{
		return operand -> varies(p);
	}
	bool tangent::operator==(function const& f) const
	{
		if(f.get_type() != e_tangent) {
			return false;
		}
		return *(static_cast<const tangent*>(&f) -> operand) == *operand;
	}
	float tangent::operator()(float r, float s, float t,
			float u, float v, float w) const
	{
		return tan((*operand)(r, s, t, u, v, w));
	}
	function* tangent::substitute(e_param p, function *fn) const
	{
		auto output = new tangent(operand -> substitute(p, fn));
		auto reduced = output -> reduce();
		delete output;
		return reduced;
	}
	function* tangent::bind(e_param p, float val) const
	{
		auto output = new tangent(operand -> bind(p, val));
		auto reduced = output -> reduce();
		delete output;
		return reduced;
	}
	function* tangent::derive(e_param p) const
	{
		auto output = new product(new product(
				new secant(operand->clone()),
				new secant(operand->clone())),
				operand->derive(p));
		auto reduced = output -> reduce();
		delete output;
		return reduced;
	}
	function* tangent::clone(void) const
	{
		return new tangent(operand->clone());
	}
	function* tangent::reduce(void) const
	{
		if(constant()) {
			return new value((*this)(0,0,0,0,0,0));
		}
		if(operand -> get_type() == e_arctangent) {
			auto cast = static_cast<const arctangent*>(operand);
			return cast -> operand -> reduce();
		}
		return new tangent(operand->reduce());
	}
	std::ostream& tangent::print(std::ostream &os) const
	{
		return os << "tan(" << *operand << ")";
	}
	tangent::tangent(function *operand):
		operand(operand), function() {}
	tangent::~tangent(void)
	{
		delete operand;
	}
}

