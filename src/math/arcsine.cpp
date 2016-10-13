#include "math.hpp"
#include <iostream>

namespace Math {
	e_type arcsine::get_type(void) const
	{
		return e_arcsine;
	}
	e_order arcsine::get_order(void) const
	{
		return e_order_term;
	}
	bool arcsine::constant(void) const
	{
		return operand -> constant();
	}
	bool arcsine::varies(e_param p) const
	{
		return operand -> varies(p);
	}
	bool arcsine::operator==(function const& f) const
	{
		if(f.get_type() != e_arcsine) {
			return false;
		}
		return *(static_cast<const arcsine*>(&f)->operand) == *operand;
	}
	float arcsine::operator()(float r, float s, float t,
			float u, float v, float w) const
	{
		return asin((*operand)(r, s, t, u, v, w));
	}
	function* arcsine::substitute(e_param p, function *fn) const
	{
		auto output = new arcsine(operand -> substitute(p, fn));
		auto reduced = output -> reduce();
		delete output;
		return reduced;
	}
	function* arcsine::bind(e_param p, float val) const
	{
		auto output = new arcsine(operand -> bind(p, val));
		auto reduced = output -> reduce();
		delete output;
		return reduced;
	}
	function* arcsine::derive(e_param p) const
	{
		auto output = new ratio(
				operand -> derive(p),
				new power(new difference(
						new value(1), new power(operand->clone(),
							new value(2))), new value(.5f)));
		auto reduced = output -> reduce();
		delete output;
		return reduced;
	}
	function* arcsine::clone(void) const
	{
		return new arcsine(operand -> clone());
	}
	function* arcsine::reduce(void) const
	{
		if(operand -> constant()) {
			return new value(asin((*operand)(0,0,0,0,0,0)));
		}
		if(operand -> get_type() == e_sine) {
			return static_cast<sine*>(operand) -> operand -> reduce();
		}
		return new arcsine(operand -> reduce());
	}
	std::ostream& arcsine::print(std::ostream &os) const
	{
		return os << "asin(" << *operand << ")";
	}
	arcsine::arcsine(function *operand):
		operand(operand), function() {}
	arcsine::~arcsine(void)
	{
		delete operand;
	}
}
