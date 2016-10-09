#include "math.hpp"
#include <iostream>

namespace Math {
	e_type cosine::get_type(void) const
	{
		return e_cosine;
	}
	bool cosine::constant(void) const
	{
		return operand->constant();
	}
	bool cosine::varies(e_param p) const
	{
		return operand -> varies(p);
	}
	e_order cosine::order(void) const
	{
		return e_order_term;
	}
	bool cosine::operator==(function const& f) const
	{
		if(f.get_type() != e_cosine) {
			return false;
		}
		return *(static_cast<const cosine*>(&f)->operand) == *operand;
	}
	float cosine::operator()(float r, float s, float t,
			float u, float v, float w) const
	{
		return cos((*operand)(r, s, t, u, v, w));
	}
	function* cosine::substitute(e_param p, function *fn) const
	{
		auto output = new cosine(operand -> substitute(p, fn));
		auto reduced = output -> reduce();
		delete output;
		return reduced;
	}
	function* cosine::bind(e_param p, float val) const
	{
		auto output = new cosine(operand -> bind(p, val));
		auto reduced = output -> reduce();
		delete output;
		return reduced;
	}
	function* cosine::derive(e_param p) const
	{
		auto output = new negative(new product(
					new sine(operand -> clone()),
					operand -> derive(p)));
		auto reduced = output -> reduce();
		delete output;
		return reduced;
	}
	function* cosine::clone(void) const
	{
		return new cosine(operand->clone());
	}
	function* cosine::reduce(void) const
	{
		if(constant()) {
			return new value((*this)(0,0,0,0,0,0));
		}
		if(operand -> get_type() == e_arccosine) {
			auto cast = static_cast<const arccosine*>(operand);
			return cast -> operand -> reduce();
		}
		return new cosine(operand->reduce());
	}
	std::ostream& cosine::print(std::ostream &os) const
	{
		os << "cos(";
		operand -> print(os);
		return os << ")";
	}
	cosine::cosine(function *operand):
		operand(operand), function() {}
	cosine::~cosine(void)
	{
		delete operand;
	}
}
