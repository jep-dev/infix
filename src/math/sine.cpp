#include "math.hpp"
#include <iostream>

namespace Math {
	e_type sine::get_type(void) const
	{
		return e_sine;
	}
	bool sine::constant(void) const
	{
		return operand->constant();
	}
	bool sine::varies(e_param p) const
	{
		return operand -> varies(p);
	}
	e_order sine::order(void) const
	{
		return e_order_term;
	}
	bool sine::operator==(function const& f) const
	{
		if(f.get_type() != e_sine) {
			return false;
		}
		return *(static_cast<const sine*>(&f) -> operand) == *operand;
	}
	float sine::operator()(float r, float s, float t,
			float u, float v, float w) const
	{
		return sin((*operand)(r, s, t, u, v, w));
	}
	function* sine::substitute(e_param p, function *fn) const
	{
		auto output = new sine(operand -> substitute(p, fn));
		auto reduced = output -> reduce();
		delete output;
		return reduced;
	}
	function* sine::bind(e_param p, float val) const
	{
		auto output = new sine(operand -> bind(p, val));
		auto reduced = output -> reduce();
		delete output;
		return reduced;
	}
	function* sine::derive(e_param p) const
	{
		auto output = new product(new cosine(operand -> clone()),
				operand -> derive(p));
		auto reduced = output -> reduce();
		delete output;
		return reduced;
	}
	function* sine::clone(void) const
	{
		return new sine(operand->clone());
	}
	function* sine::reduce(void) const
	{
		if(constant()) {
			return new value((*this)(0,0,0,0,0,0));
		}
		if(operand -> get_type() == e_arcsine) {
			auto cast = static_cast<const arcsine*>(operand);
			return cast -> operand -> reduce();
		}
		return new sine(operand -> reduce());
	}
	std::ostream& sine::print(std::ostream &os) const
	{
		return os << "sin(" << *operand << ")";
	}
	sine::sine(function *operand):
		operand(operand), function() {}
	sine::~sine(void)
	{
		delete operand;
	}
}
