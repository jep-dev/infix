#include "math.hpp"
#include <iostream>

namespace Math {
	e_type sum::get_type(void) const
	{
		return e_sum;
	}
	bool sum::constant(void) const
	{
		return lhs->constant() && rhs->constant();
	}
	bool sum::varies(e_param p) const
	{
		return lhs -> varies(p) || rhs -> varies(p);
	}
	e_order sum::order(void) const
	{
		return e_order_sum;
	}
	bool sum::operator==(function const& f) const
	{
		if(f.get_type() != e_sum) {
			return false;
		}
		auto cast = static_cast<const sum*>(&f);
		return *(cast -> lhs) == *lhs
			&& *(cast -> rhs) == *rhs;
	}
	float sum::operator()(float r, float s, float t,
			float u, float v, float w) const
	{
		return (*lhs)(r, s, t, u, v, w) + (*rhs)(r, s, t, u, v, w);
	}
	function* sum::substitute(e_param p, function *fn) const
	{
		auto output = new sum(lhs -> substitute(p, fn),
				rhs -> substitute(p, fn));
		auto reduced = output -> reduce();
		delete output;
		return reduced;
	}
	function* sum::bind(e_param p, float val) const
	{
		auto output = new sum(lhs->bind(p,val), rhs->bind(p,val));
		auto reduced = output -> reduce();
		delete output;
		return reduced;
	}
	function* sum::derive(e_param p) const
	{
		auto output = new sum(lhs->derive(p), rhs->derive(p));
		auto reduced = output -> reduce();
		delete output;
		return reduced;
	}
	function* sum::clone(void) const
	{
		return new sum(lhs->clone(), rhs->clone());
	}
	function* sum::reduce(void) const
	{
		if(constant()) {
			return new value((*this)(0,0,0,0,0,0));
		}
		auto l = lhs -> reduce();
		auto r = rhs -> reduce();
		if(l -> constant() && *l == 0) {
			delete l;
			return r;
		}
		if(r -> constant() && *r == 0) {
			delete r;
			return l;
		}
		if(*l == *r) {
			delete r;
			auto output = new product(new value(2), l);
			auto reduced = output -> reduce();
			delete output;
			return reduced;
		}
		if(*r < *l) {
			auto temp = r;
			r = l;
			l = temp;
		}
		if(r -> get_type() == e_product) {
			auto pro = static_cast<product*>(r);
			auto pro_lhs = pro -> lhs -> clone();
			auto pro_rhs = pro -> rhs -> clone();
			delete r;
			if(pro_lhs -> constant() && *pro_lhs == -1) {
				delete pro_lhs;
				return new difference(l, pro_rhs);
			}
			if(pro_rhs -> constant() && *pro_rhs == -1) {
				delete pro_rhs;
				return new difference(l, pro_lhs);
			}
			r = new product(pro_lhs, pro_rhs);
		}
		return new sum(l, r);
	}
	std::ostream& sum::print(std::ostream &os) const
	{
		return os << *lhs << "+" << *rhs;
	}
	sum::sum(function *lhs, function *rhs):
		lhs(lhs), rhs(rhs), function() {}
	sum::~sum(void)
	{
		delete lhs;
		delete rhs;
	}
}
