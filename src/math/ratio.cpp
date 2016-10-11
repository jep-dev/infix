#include "math.hpp"
#include <iostream>

namespace Math {
	e_type ratio::get_type(void) const
	{
		return e_ratio;
	}
	bool ratio::constant(void) const
	{
		return numerator->constant() &&
			(*numerator == 0 || denominator->constant());
	}
	bool ratio::varies(e_param p) const
	{
		return !constant() && (numerator -> varies(p)
				|| denominator -> varies(p));
	}
	e_order ratio::order(void) const
	{
		return e_order_ratio;
	}
	bool ratio::operator==(function const& f) const
	{
		if(f.get_type() != e_ratio) {
			return false;
		}
		auto cast = static_cast<const ratio*>(&f);
		return *(cast -> numerator) == *numerator
			&& *(cast -> denominator) == *denominator;
	}
	float ratio::operator()(float r, float s, float t,
			float u, float v, float w) const
	{
		return (*numerator)(r, s, t, u, v, w)
			/ (*denominator)(r, s, t, u, v, w);
	}
	function* ratio::substitute(e_param p, function *fn) const
	{
		auto output = new ratio(numerator -> substitute(p, fn),
					denominator -> substitute(p, fn));
		auto reduced = output -> reduce();
		delete output;
		return reduced;
	}
	function* ratio::bind(e_param p, float val) const
	{
		auto output = new ratio(numerator -> bind(p, val),
					denominator -> bind(p, val));
		auto reduced = output -> reduce();
		delete output;
		return reduced;
	}
	function* ratio::derive(e_param p) const
	{
		auto output = new ratio(
				new difference(
					new product(numerator->derive(p), denominator->clone()),
					new product(numerator->clone(), denominator->derive(p))),
				new product(denominator->clone(), denominator->clone()));
		auto reduced = output -> reduce();
		delete output;
		return reduced;
	}
	function* ratio::clone(void) const
	{
		return new ratio(numerator->clone(), denominator->clone());
	}
	function* ratio::reduce(void) const
	{
		if(constant()) {
			return new value((*this)(0,0,0,0,0,0));
		}
		auto l = numerator -> reduce();
		auto r = denominator -> reduce();
		if(l -> constant()) {
			if(*l == 0) {
				delete l;
				delete r;
				return new value(0);
			}
		}
		if(r -> constant()) {
			auto rval = (*r)(0,0,0,0,0,0);
			if(rval == 1) {
				delete r;
				return l;
			}
			if(rval == -1) {
				delete r;
				return new negative(l);
			}
			if(rval < 0) {
				delete r;
				r = new value(-rval);
				return new negative(new ratio(l,r));
			}
		}
		if(*l == *r) {
			delete l;
			delete r;
			return new value(1);
		}
		function *lnum, *lden, *rnum, *rden;
		if(l -> get_type() == e_ratio) {
			auto lratio = static_cast<ratio*>(l);
			lnum = lratio -> numerator -> clone();
			lden = lratio -> denominator -> clone();
		} else {
			lnum = l -> clone();
			lden = new value(1);
		}
		delete l;
		if(r -> get_type() == e_ratio) {
			auto rratio = static_cast<ratio*>(r);
			rnum = rratio -> denominator -> clone();
			rden = rratio -> numerator -> clone();
		} else {
			rnum = new value(1);
			rden = r -> clone();
		}
		delete r;
		auto num = new product(lnum, rnum);
		auto num_red = num -> reduce();
		delete num;
		int negs = 0;
		if(num_red -> get_type() == e_negative) {
			negs++;
			function *temp = static_cast<const negative*>(num_red)
				-> operand -> clone();
			delete num_red;
			num_red = temp;
		}
		auto den = new product(lden, rden);
		auto den_red = den -> reduce();
		if(den_red -> get_type() == e_negative) {
			negs++;
			function *temp = static_cast<const negative*>(den_red)
				-> operand -> clone();
			delete den_red;
			den_red = temp;
		}
		delete den;
		if(negs == 1) {
			return new negative(new ratio(num_red, den_red));
		}
		return new ratio(num_red, den_red);
	}
	std::ostream& ratio::print(std::ostream &os) const {
		auto lcomp = numerator -> order(),
			 rcomp = denominator -> order();
		if(lcomp <= e_order_ratio && lcomp > e_order_term) {
			os << '(' << *numerator << ')';
		} else {
			os << *numerator;
		}
		os << "/";
		if(rcomp <= e_order_ratio && rcomp > e_order_term) {
			os << '(' << *denominator << ')';
		} else {
			os << *denominator;
		}
		return os;
	}
	ratio::ratio(function *numerator, function *denominator):
		numerator(numerator), denominator(denominator), function() {}
	ratio::~ratio(void)
	{
		delete numerator;
		delete denominator;
	}
}
