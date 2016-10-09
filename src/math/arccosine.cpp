#include "math.hpp"
#include <iostream>

namespace Math {
	e_type arccosine::get_type(void) const
	{
		return e_arccosine;
	}
	bool arccosine::constant(void) const
	{
		return operand -> constant();
	}
	bool arccosine::varies(e_param p) const
	{
		return operand -> varies(p);
	}
	e_order arccosine::order(void) const
	{
		return e_order_term;
	}
	bool arccosine::operator==(function const& f) const
	{
		if(f.get_type() != e_arccosine) {
			return false;
		}
		return *(static_cast<const arccosine*>(&f) -> operand) == *operand;
	}
	float arccosine::operator()(float r, float s, float t,
			float u, float v, float w) const
	{
		return acos((*operand)(r, s, t, u, v, w));
	}
	function* arccosine::substitute(e_param p, function *fn) const
	{
		auto output = new arccosine(operand -> substitute(p, fn));
		auto reduced = output -> reduce();
		delete output;
		return reduced;
	}
	function* arccosine::bind(e_param p, float val) const
	{
		auto output = new arccosine(operand -> bind(p, val));
		auto reduced = output -> reduce();
		delete output;
		return reduced;
	}
	function* arccosine::derive(e_param p) const
	{
		auto output = new ratio(
				new product(new value(-1), operand->derive(p)),
				new power(new difference(new value(1),
						new power(operand->clone(), new value(2))),
					new value(.5f)));
		auto reduced = output -> reduce();
		delete output;
		return reduced;
	}
	function* arccosine::clone(void) const
	{
		return new arccosine(operand -> clone());
	}
	function* arccosine::reduce(void) const
	{
		if(operand -> constant()) {
			return new value(acos((*operand)(0,0,0,0,0,0)));
		}
		if(operand -> get_type() == e_cosine) {
			return static_cast<cosine*>(operand) -> operand -> reduce();
		}
		return clone();
	}
	std::ostream& arccosine::print(std::ostream &os) const
	{
		return os << "acos(" << *operand << ")";
	}
	arccosine::arccosine(function *operand):
		operand(operand), function() {}
	arccosine::~arccosine(void)
	{
		delete operand;
	}
}
