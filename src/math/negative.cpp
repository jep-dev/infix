#include "math.hpp"
#include <iostream>

namespace Math{
	e_type negative::get_type(void) const
	{
		return e_negative;
	}
	e_order negative::get_order(void) const
	{
		return e_order_term;
	}
	bool negative::constant(void) const
	{
		return operand -> constant();
	}
	bool negative::varies(e_param p) const
	{
		return operand -> varies(p);
	}
	bool negative::operator==(function const& f) const
	{
		if(f.get_type() != e_negative) {
			return false;
		}
		return *(static_cast<const negative*>(&f) -> operand) == *operand;
	}
	float negative::operator()(float r, float s, float t,
			float u, float v, float w) const
	{
		return -(*operand)(r, s, t, u, v, w);
	}
	function* negative::substitute(e_param p, function *fn) const
	{
		auto output = new negative(operand -> substitute(p, fn));
		auto reduced = output -> reduce();
		delete output;
		return reduced;
	}
	function* negative::bind(e_param p, float val) const
	{
		auto output = new negative(operand -> bind(p,val));
		auto reduced = output -> reduce();
		delete output;
		return reduced;
	}
	function* negative::derive(e_param p) const
	{
		auto output = new negative(operand -> derive(p));
		auto reduced = output -> reduce();
		delete output;
		return reduced;
	}
	function* negative::clone(void) const
	{
		return new negative(operand -> clone());
	}
	function* negative::reduce(void) const
	{
		auto reduced = operand -> reduce();
		if(reduced -> constant()) {
			auto val = (*reduced)(0,0,0,0,0,0);
			delete reduced;
			return new value(-val);
		}
		switch(reduced -> get_type()) {
			case e_negative: {
				auto cast = static_cast<const negative*>(reduced);
				auto output = cast -> operand -> reduce();
				delete reduced;
				return output;
			} break;
			case e_difference: {
				auto cast = static_cast<const difference*>(reduced);
				auto output = new difference(cast -> rhs -> clone(),
						cast -> lhs -> clone());
				delete reduced;
				auto output_red = output -> reduce();
				delete output;
				return output_red;
			} break;
			case e_product: {
				auto cast = static_cast<const product*>(reduced);
				function *lhs, *rhs;
				if(cast -> lhs -> constant()) {
					lhs = cast -> lhs -> reduce();
					rhs = cast -> rhs -> reduce();
				} else if(cast -> rhs -> constant()) {
					lhs = cast -> rhs -> reduce();
					rhs = cast -> lhs -> reduce();
				} else {
					lhs = cast -> lhs -> clone();
					rhs = cast -> rhs -> clone();
					delete reduced;
					int count = 1;
					if(lhs -> get_type() == e_negative) {
						count++;
						function *temp = static_cast<const negative*>(lhs)
							-> operand -> clone();
						delete lhs;
						lhs = temp;
					}
					if(rhs -> get_type() == e_negative) {
						count++;
						function *temp = static_cast<const negative*>(rhs)
							-> operand -> clone();
						delete rhs;
						rhs = temp;
					}
					if(count == 1 || count == 3) {
						return new negative(new product(lhs, rhs));
					} else {
						return new product(lhs, rhs);
					}
				}
				delete reduced;
				auto lval = (*lhs)(0,0,0,0,0,0);
				delete lhs;
				if(lval == -1) {
					return new negative(rhs);
				} else if(lval < 0) {
					return new negative(new product(new value(-lval), rhs));
				}
				return new negative(new product(new value(lval), rhs));
			} break;
			case e_ratio: {
				auto cast = static_cast<const ratio*>(reduced);
				function *num = cast -> numerator -> clone(),
						 *den = cast -> denominator -> clone();
				delete reduced;
				int count = 1;
				if(num -> get_type() == e_negative) {
					count++;
					function *temp = static_cast<const negative*>(num)
						-> operand -> clone();
					delete num;
					num = temp;
				}
				if(den -> get_type() == e_negative) {
					count++;
					function *temp = static_cast<const negative*>(den)
						-> operand -> clone();
					delete den;
					den = temp;
				}
				if(count == 1 || count == 3) {
					return new negative(new ratio(num, den));
				} else {
					return new ratio(num, den);
				}
			} break;
			default: {
				return new negative(reduced);
			} break;
		}
	}
	std::ostream& negative::print(std::ostream &os) const
	{
		if(operand -> get_order() == e_order_sum) {
			return os << "-(" << *operand << ')';
		}
		return os << '-' << *operand;
	}
	negative::negative(function *operand):
		operand(operand), function() {}
	negative::~negative(void)
	{
		delete operand;
	}
}
