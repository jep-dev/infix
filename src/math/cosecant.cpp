#include "math.hpp"
#include <iostream>

namespace Math {
	e_type cosecant::get_type(void) const
	{
		return e_cosecant;
	}
	bool cosecant::constant(void) const
	{
		return operand->constant();
	}
	bool cosecant::varies(e_param p) const
	{
		return operand -> varies(p);
	}
	e_order cosecant::order(void) const
	{
		return e_order_term;
	}
	bool cosecant::operator==(function const& f) const
	{
		if(f.get_type() != e_cosecant) {
			return false;
		}
		return *(static_cast<const cosecant*>(&f)->operand) == *operand;
	}
	float cosecant::operator()(float r, float s, float t,
			float u, float v, float w) const
	{
		return 1/sin((*operand)(r, s, t, u, v, w));
	}
	function* cosecant::substitute(e_param p, function *fn) const
	{
		auto output = new cosecant(operand -> substitute(p, fn));
		auto reduced = output -> reduce();
		delete output;
		return reduced;
	}
	function* cosecant::bind(e_param p, float val) const
	{
		auto output = new cosecant(operand -> bind(p, val));
		auto reduced = output -> reduce();
		delete output;
		return reduced;
	}
	function* cosecant::derive(e_param p) const
	{
		auto output = new product(new value(-1),
				new product(new product(
					new cosecant(operand->clone()),
					new cotangent(operand->clone())
				), operand->derive(p)));
		auto reduced = output -> reduce();
		delete output;
		return reduced;
	}
	function* cosecant::clone(void) const
	{
		return new cosecant(operand->reduce());
	}
	function* cosecant::reduce(void) const
	{
		if(operand -> constant()) {
			return new value(1/sin((*operand)(0,0,0,0,0,0)));
		}
		return clone();
	}
	std::ostream& cosecant::print(std::ostream &os) const
	{
		return os << "csc(" << *operand << ")";
	}
	cosecant::cosecant(function *operand):
		operand(operand), function() {}
	cosecant::~cosecant(void)
	{
		delete operand;
	}
}
