#include "math.hpp"
#include <iostream>

namespace Math {
	e_type product::get_type(void) const
	{
		return e_product;
	}
	bool product::constant(void) const
	{
		return lhs->constant() && rhs->constant();
	}
	bool product::varies(e_param p) const
	{
		return lhs -> varies(p) || rhs -> varies(p);
	}
	e_order product::order(void) const
	{
		return e_order_product;
	}
	bool product::operator==(function const& f) const
	{
		if(f.get_type() != e_product) {
			return false;
		}
		auto cast = static_cast<const product*>(&f);
		return *(cast -> lhs) == *lhs
			&& *(cast -> rhs) == *rhs;
	}
	float product::operator()(float r, float s, float t, 
			float u, float v, float w) const
	{
		return (*lhs)(r, s, t, u, v, w) * (*rhs)(r, s, t, u, v, w);
	}
	function* product::substitute(e_param p, function *fn) const
	{
		auto output = new product(lhs -> substitute(p, fn),
					rhs -> substitute(p, fn));
		auto reduced = output -> reduce();
		delete output;
		return reduced;
	}
	function* product::bind(e_param p, float val) const
	{
		auto output = new product(lhs -> bind(p, val),
					rhs -> bind(p, val));
		auto reduced = output -> reduce();
		delete output;
		return reduced;
	}
	function* product::derive(e_param p) const
	{
		auto output = new sum(
					new product(lhs -> derive(p), rhs -> clone()),
					new product(lhs -> clone(), rhs -> derive(p)));
		auto reduced = output -> reduce();
		delete output;
		return reduced;
	}
	function* product::clone(void) const
	{
		return new product(lhs->clone(), rhs->clone());
	}
	function* product::reduce(void) const
	{
		if(constant()) {
			return new value((*this)(0,0,0,0,0,0));
		}
		function *l = lhs -> reduce(),
				 *r = rhs -> reduce();
		if(*l < *r) {
			auto temp = r;
			r = l;
			l = temp;
		}
		if(l -> constant()) {
			auto lval = (*l)(0,0,0,0,0,0);
			if(lval == 0) {
				delete l;
				delete r;
				return new value(0);
			}
			if(lval == 1) {
				delete l;
				return r;
			}
			if(r -> get_type() == e_product) {
				auto rproduct = static_cast<const product*>(r);
				function *rl = rproduct -> lhs -> clone(),
						*rr = rproduct -> rhs -> clone();
				delete r;
				if(rr -> constant()) {
					auto temp = rr;
					rr = rl;
					rl = temp;
				}
				if(rl -> constant()) {
					auto new_lhs = new product(l, rl);
					auto new_lhs_red = new_lhs -> reduce();
					delete new_lhs;
					return new product(new_lhs_red, rr);
				}
				return new product(l, new product(rl, rr));
			}
		}
		auto ltype = l -> get_type(), rtype = r -> get_type();
		if((ltype == e_secant && rtype == e_cosine)
				|| (ltype == e_cosecant && rtype == e_sine)
				|| (ltype == e_cotangent && rtype == e_tangent)) {
			auto temp = r;
			r = l;
			l = temp;
		}
		ltype = l -> get_type();
		rtype = r -> get_type();
		if(ltype == e_cosine && rtype == e_secant) {
			auto lcast = static_cast<const cosine*>(l);
			auto rcast = static_cast<const secant*>(r);
			if(*(lcast -> operand) == *(rcast -> operand)) {
				delete l;
				delete r;
				return new value(1);
			}
		}
		if(ltype == e_sine && rtype == e_cosecant) {
			auto lcast = static_cast<const sine*>(l);
			auto rcast = static_cast<const cosecant*>(r);
			if(*(lcast -> operand) == *(rcast -> operand)) {
				delete l;
				delete r;
				return new value(1);
			}
		}
		if(ltype == e_tangent && rtype == e_cotangent) {
			auto lcast = static_cast<const tangent*>(l);
			auto rcast = static_cast<const cotangent*>(r);
			if(*(lcast -> operand) == *(rcast -> operand)) {
				delete l;
				delete r;
				return new value(1);
			}
		}
		if(ltype == e_ratio || rtype == e_ratio) {
			int negs = 0;
			function *lnum, *rnum, *lden, *rden;
			if(ltype == e_ratio) {
				auto lratio = static_cast<ratio*>(l);
				lnum = lratio -> numerator -> clone();
				lden = lratio -> denominator -> clone();
			} else {
				lnum = l -> clone();
				lden = new value(1);
			}
			delete l;
			if(lnum -> get_type() == e_negative) {
				negs++;
				function *temp = static_cast<const negative*>(lnum)
					-> operand -> clone();
				delete lnum;
				lnum = temp;
			}
			if(lden -> get_type() == e_negative) {
				negs++;
				function *temp = static_cast<const negative*>(lden)
					-> operand -> clone();
				delete lden;
				lden = temp;
			}
			if(rtype == e_ratio) {
				auto rratio = static_cast<ratio*>(r);
				rnum = rratio -> numerator -> clone();
				rden = rratio -> denominator -> clone();
			} else {
				rnum = r -> clone();
				rden = new value(1);
			}
			delete r;
			if(rnum -> get_type() == e_negative) {
				negs++;
				function *temp = static_cast<const negative*>(rnum)
					-> operand -> clone();
				delete rnum;
				rnum = temp;
			}
			if(rden -> get_type() == e_negative) {
				negs++;
				function *temp = static_cast<const negative*>(rden)
					-> operand -> clone();
				delete rden;
				rden = temp;
			}
			function *num = new product(lnum, rnum),
					*den = new product(lden, rden),
					*output = new ratio(num -> reduce(), den -> reduce());
			delete num;
			delete den;
			if((negs & 1) == 1) {
				return new negative(output);
			}
			return output;
		}
		if(*l == *r) {
			delete r;
			if(l -> get_type() == e_negative) {
				function *temp = static_cast<const negative*>(l)
					-> operand -> clone();
				delete l;
				l = temp;
			}
			auto output = new power(l, new value(2));
			auto reduced = output -> reduce();
			delete output;
			return reduced;
		}
		if(*r < *l) {
			return new product(l,r);
		} else {
			return new product(r,l);
		}
	}
	std::ostream& product::print(std::ostream &os) const
	{

		auto lcomp = lhs -> order(),
			 rcomp = rhs -> order();
		if(lhs -> constant() && (*lhs)(0,0,0,0,0,0) == -1) {
			os << "-";
			if(rcomp < e_order_product && rcomp > e_order_term) {
				os << "(" << *rhs << ")";
			} else {
				os << *rhs;
			}
			return os;
		}
		if(rhs -> constant() && (*rhs)(0,0,0,0,0,0) == -1) {
			os << "-";
			if(lcomp < e_order_product && lcomp > e_order_term) {
				os << "(" << *lhs << ")";
			} else {
				os << *lhs;
			}
			return os;
		}
		if(lcomp < e_order_product && lcomp > e_order_term) {
			os << "(" << *lhs << ")";
		} else {
			os << *lhs;
		}
		os << "*";
		if(rcomp < e_order_product && rcomp > e_order_term) {
			os << "(" << *rhs << ")";
		} else {
			os << *rhs;
		}
		return os;
	}
	product::product(function *lhs, function *rhs):
		lhs(lhs), rhs(rhs), function() {}
	product::~product(void)
	{
		delete lhs;
		delete rhs;
	}
}
