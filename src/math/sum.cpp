#include "math.hpp"
#include <algorithm>
#include <iostream>
#include <vector>

namespace Math {
	e_type sum::get_type(void) const
	{
		return e_sum;
	}
	e_order sum::get_order(void) const
	{
		return e_order_sum;
	}
	bool sum::constant(void) const
	{
		return lhs->constant() && rhs->constant();
	}
	bool sum::varies(e_param p) const
	{
		return lhs -> varies(p) || rhs -> varies(p);
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

		function *lterms = nullptr, *rterms = nullptr;
		std::vector<function*> terms;
		get_terms(*l, terms);
		get_terms(*r, terms);
		delete l;
		delete r;
		for(auto term : terms) {
			auto cast = static_cast<const product*>(term);
			auto lhs_val = (*(cast -> lhs))(0,0,0,0,0,0);
			if(lhs_val > 0) {
				function *reduced = term -> reduce();
				delete term;
				if(lterms) {
					lterms = new sum(lterms, reduced);
				} else {
					lterms = reduced;
				}
			} else {
				auto cast_rhs = cast -> rhs -> clone();
				delete term;
				term = new product(new value(-lhs_val), cast_rhs);
				if(rterms) {
					rterms = new sum(rterms, term -> reduce());
				} else {
					rterms = term -> reduce();
				}
				delete term;
			}
		}
		if(lterms && rterms) {
			return new difference(lterms, rterms);
		} else if(lterms) {
			return lterms;
		} else if(rterms) {
			return new negative(rterms);
		} else {
			return new value(0);
		}

		// -- Old reduce
		/*if(*l == *r) {
			delete r;
			auto output = new product(new value(2), l);
			auto reduced = output -> reduce();
			delete output;
			return reduced;
		}
		if(*r < *l) {
			std::swap(l,r);
		}
		auto ltype = l -> get_type(), rtype = r -> get_type();
		if(ltype == e_negative && rtype == e_negative) {
			auto ln = static_cast<const negative*>(l),
				 rn = static_cast<const negative*>(r);
			auto lop = ln -> operand -> clone(),
				 rop = rn -> operand -> clone();
			delete l;
			delete r;
			return new negative(new sum(lop, rop));
		} else if(ltype == e_negative) {
			auto ln = static_cast<const negative*>(l);
			auto lop = ln -> operand -> clone();
			delete l;
			return new difference(r, lop);
		} else if(rtype == e_negative) {
			auto rn = static_cast<const negative*>(r);
			auto rop = rn -> operand -> clone();
			delete r;
			return new difference(l, rop);
		} else {
			return new sum(l, r);
		}*/
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
