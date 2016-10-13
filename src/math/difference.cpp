#include "math.hpp"
#include <iostream>

namespace Math {
	e_type difference::get_type(void) const
	{
		return e_difference;
	}
	e_order difference::get_order(void) const
	{
		return e_order_sum;
	}
	bool difference::constant(void) const
	{
		return lhs->constant() && rhs->constant();
	}
	bool difference::varies(e_param p) const
	{
		return lhs -> varies(p) || rhs -> varies(p);
	}
	bool difference::operator==(function const& f) const
	{
		if(f.get_type() != e_difference) {
			return false;
		}
		auto cast = static_cast<const difference*>(&f);
		return *(cast -> lhs) == *lhs
			&& *(cast -> rhs) == *rhs;
	}
	float difference::operator()(float r, float s, float t,
			float u, float v, float w) const
	{
		return (*lhs)(r, s, t, u, v, w) - (*rhs)(r, s, t, u, v, w);
	}
	function* difference::substitute(e_param p, function *fn) const
	{
		auto output = new difference(lhs -> substitute(p, fn),
				rhs -> substitute(p, fn));
		auto reduced = output -> reduce();
		delete output;
		return reduced;
	}
	function* difference::bind(e_param p, float val) const
	{
		auto output = new difference(lhs -> bind(p, val),
				rhs -> bind(p, val));
		auto reduced = output -> reduce();
		delete output;
		return reduced;
	}
	function* difference::derive(e_param p) const
	{
		auto output = new difference(lhs -> derive(p), rhs -> derive(p));
		auto reduced = output -> reduce();
		delete output;
		return reduced;
	}
	function* difference::clone(void) const
	{
		return new difference(lhs -> clone(), rhs -> clone());
	}
	function* difference::reduce(void) const
	{
		if(constant()) {
			return new value((*this)(0,0,0,0,0,0));
		}
		auto l = lhs -> reduce();
		auto r = rhs -> reduce();
		if(l -> constant()) {
			auto lval = (*l)(0,0,0,0,0,0);
			if(lval == 0) {
				delete l;
				return new negative(r);
			}
		}
		if(r -> constant()) {
			auto rval = (*r)(0,0,0,0,0,0);
			if(rval == 0) {
				delete r;
				return l;
			}
		}
		if(*l == *r) {
			delete l;
			delete r;
			return new value(0);
		}
		auto ltype = l -> get_type(),
			 rtype = r -> get_type();
		if(ltype == e_negative && rtype == e_negative) {
			return new difference(r, l);
		}
		if(ltype == e_negative) {
			auto l_op = static_cast<const negative*>(l)
				-> operand -> clone();
			delete l;
			return new negative(new sum(l_op, r));
		}
		if(r -> get_type() == e_negative) {
			function *temp = static_cast<const negative*>(r)
				-> operand -> clone();
			delete r;
			return new sum(l, temp);
		}
		return new difference(l,r);
	}
	std::ostream& difference::print(std::ostream &os) const
	{
		os << *lhs << "-";
		if(rhs->get_order() == e_order_sum) {
			os << "(" << *rhs << ")";
		} else {
			os << *rhs;
		}
		return os;
	}
	difference::difference(function *lhs, function *rhs):
		lhs(lhs), rhs(rhs), function() {}
	difference::~difference(void) 
	{
		delete lhs;
		delete rhs;
	}
}
