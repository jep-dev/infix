#include "math.hpp"
#include <iostream>

namespace Math {
	e_type cotangent::get_type(void) const
	{
		return e_cotangent;
	}
	bool cotangent::constant(void) const
	{
		return operand->constant();
	}
	bool cotangent::varies(e_param p) const
	{
		return operand -> varies(p);
	}
	e_order cotangent::order(void) const
	{
		return e_order_term;
	}
	bool cotangent::operator==(function const& f) const
	{
		if(f.get_type() != e_cotangent) {
			return false;
		}
		return *(static_cast<const cotangent*>(&f)->operand) == *operand;
	}
	float cotangent::operator()(float r, float s, float t,
			float u, float v, float w) const
	{
		auto val = (*operand)(r, s, t, u, v, w);
		return cos(val)/sin(val);
	}
	function* cotangent::substitute(e_param p, function *fn) const
	{
		auto output = new cotangent(operand -> substitute(p, fn));
		auto reduced = output -> reduce();
		delete output;
		return reduced;
	}
	function* cotangent::bind(e_param p, float val) const
	{
		auto output = new cotangent(operand -> bind(p, val));
		auto reduced = output -> reduce();
		delete output;
		return reduced;
	}
	function* cotangent::clone(void) const
	{
		return new cotangent(operand->reduce());
	}
	function* cotangent::derive(e_param p) const
	{
		auto output = new product(new value(-1),
				new product(new product(
						new cosecant(operand->clone()),
					new cosecant(operand->clone())),
					operand->derive(p)));
		auto reduced = output -> reduce();
		delete output;
		return reduced;
	}
	function* cotangent::reduce(void) const
	{
		if(operand -> constant()) {
			return new value(1/tan((*operand)(0,0,0,0,0,0)));
		}
		if(operand -> get_type() == e_arctangent) {
			auto cast = static_cast<const arctangent*>(operand);
			return new ratio(new value(1), cast -> operand -> reduce());
		}
		return clone();
	}
	std::ostream& cotangent::print(std::ostream &os) const
	{
		return os << "cot(" << *operand << ")";
	}
	cotangent::cotangent(function *operand):
		operand(operand), function() {}
	cotangent::~cotangent(void)
	{
		delete operand;
	}
}
