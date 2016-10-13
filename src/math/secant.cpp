#include "math.hpp"
#include <iostream>

namespace Math {
	e_type secant::get_type(void) const
	{
		return e_secant;
	}
	e_order secant::get_order(void) const
	{
		return e_order_term;
	}
	bool secant::constant(void) const
	{
		return operand->constant();
	}
	bool secant::varies(e_param p) const
	{
		return operand -> varies(p);
	}
	bool secant::operator==(function const& f) const
	{
		if(f.get_type() != e_secant) {
			return false;
		}
		return *(static_cast<const secant*>(&f) -> operand) == *operand;
	}
	float secant::operator()(float r, float s, float t,
			float u, float v, float w) const
	{
		return 1/cos((*operand)(r, s, t, u, v, w));
	}
	function* secant::substitute(e_param p, function *fn) const
	{
		auto output = new secant(operand -> substitute(p, fn));
		auto reduced = output -> reduce();
		delete output;
		return reduced;
	}
	function* secant::bind(e_param p, float val) const
	{
		auto output = new secant(operand -> bind(p, val));
		auto reduced = output -> reduce();
		delete output;
		return reduced;
	}
	function* secant::derive(e_param p) const
	{
		auto output = new product(new product(
				new secant(operand->clone()),
				new tangent(operand->clone())),
				operand->derive(p));
		auto reduced = output -> reduce();
		delete output;
		return reduced;
	}
	function* secant::clone(void) const
	{
		return new secant(operand->reduce());
	}
	function* secant::reduce(void) const
	{
		if(operand -> constant()) {
			return new value(1/cos((*operand)(0,0,0,0,0,0)));
		}
		if(operand -> get_type() == e_arccosine) {
			auto cast = static_cast<const arccosine*>(operand);
			return new ratio(new value(1), cast -> operand -> reduce());
		}
		return clone();
	}
	std::ostream& secant::print(std::ostream &os) const
	{
		return os << "sec(" << *operand << ")";
	}
	secant::secant(function *operand):
		operand(operand), function() {}
	secant::~secant(void)
	{
		delete operand;
	}
}
