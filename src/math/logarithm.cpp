#include "math.hpp"
#include <iostream>

namespace Math {
	e_type logarithm::get_type(void) const
	{
		return e_logarithm;
	}
	e_order logarithm::get_order(void) const
	{
		return e_order_term;
	}
	bool logarithm::constant(void) const
	{
		return operand->constant();
	}
	bool logarithm::varies(e_param p) const
	{
		return operand -> varies(p);
	}
	bool logarithm::operator==(function const& f) const
	{
		if(f.get_type() != e_logarithm) {
			return false;
		}
		return *(static_cast<const logarithm*>(&f) -> operand) == *operand;
	}
	float logarithm::operator()(float r, float s, float t,
			float u, float v, float w) const
	{
		return log((*operand)(r, s, t, u, v, w));
	}
	function* logarithm::substitute(e_param p, function *fn) const
	{
		auto output = new logarithm(operand -> substitute(p, fn));
		auto reduced = output -> reduce();
		delete output;
		return reduced;
	}
	function* logarithm::bind(e_param p, float val) const
	{
		auto output = new logarithm(operand -> bind(p, val));
		auto reduced = output -> reduce();
		delete output;
		return reduced;
	}
	function* logarithm::derive(e_param p) const
	{
		auto output = new ratio(operand->derive(p), operand->clone());
		auto reduced = output -> reduce();
		delete output;
		return reduced;
	}
	function* logarithm::clone(void) const
	{
		return new logarithm(operand->clone());
	}
	function* logarithm::reduce(void) const
	{
		if(operand -> constant()) {
			return new value(log((*operand)(0,0,0,0,0,0)));
		}
		if(operand -> get_type() == e_power) {
			auto cast = static_cast<const power*>(operand);
			auto base = cast -> base -> clone();
			if(base -> constant() && (*base)(0,0,0,0,0,0) == float(M_E)) {
				auto exp = cast -> exp -> clone();
				delete base;
				return exp;
			}
			delete base;
		}
		return new logarithm(operand->reduce());
	}
	std::ostream& logarithm::print(std::ostream &os) const
	{
		return os << "log(" << *operand << ")";
	}
	logarithm::logarithm(function *operand):
		operand(operand), function() {}
	logarithm::~logarithm(void)
	{
		delete operand;
	}
}
