#include "math.hpp"
#include <iostream>

namespace Math {
	e_type power::get_type(void) const
	{
		return e_power;
	}
	bool power::constant(void) const
	{
		return (exp->constant() &&
				((*exp)(0,0,0,0,0,0)==0 || base -> constant()))
			|| (base -> constant() && (*base)(0,0,0,0,0,0) == 0);
	}
	bool power::varies(e_param p) const
	{
		return !constant() && (base -> varies(p) || exp -> varies(p));
	}
	e_order power::order(void) const
	{
		if(exp -> constant()) {
			if(*exp == 0 || *exp == 1) {
				return e_order_term;
			}
		}
		return e_order_exponent;
	}
	bool power::operator==(function const& f) const
	{
		if(f.get_type() != e_power) {
			return false;
		}
		auto cast = static_cast<const power*>(&f);
		return *(cast -> base) == *base
			&& *(cast -> exp) == *exp;
	}
	float power::operator()(float r, float s, float t,
			float u, float v, float w) const
	{
		return pow((*base)(r, s, t, u, v, w), (*exp)(r, s, t, u, v, w));
	}
	function* power::substitute(e_param p, function *fn) const
	{
		auto output = new power(base -> substitute(p, fn),
					exp -> substitute(p, fn));
		auto reduced = output -> reduce();
		delete output;
		return reduced;
	}
	function* power::bind(e_param p, float val) const
	{
		auto output = new power(base -> bind(p,val), exp -> bind(p,val));
		auto reduced = output -> reduce();
		delete output;
		return reduced;
	}
	function* power::derive(e_param p) const
	{
		auto base_red = base -> reduce();
		auto base_der = base_red -> derive(p);
		auto base_der_red = base_der -> reduce();
		delete base_der;
		
		auto exp_red = exp -> reduce();
		auto exp_der = exp_red -> derive(p);
		auto exp_der_red = exp_der -> reduce();
		delete exp_der;
		
		auto derived = new product(
				new power(
					base_red -> clone(), 
					new difference(exp_red -> clone(), new value(1))
				), new sum(
					new product(exp_red -> clone(), base_der_red -> clone()),
					new product(base_red -> clone(), new product(
						new logarithm(base_red -> clone()),
						exp_der_red -> clone()
					))
				));

		delete base_red;
		delete base_der_red;

		delete exp_red;
		delete exp_der_red;

		auto reduced = derived -> reduce();
		delete derived;
		return reduced;
	}
	function* power::clone(void) const
	{
		return new power(base -> reduce(), exp -> reduce());
	}
	function* power::reduce(void) const
	{
		auto base_red = base -> reduce();
		auto exp_red = exp -> reduce();
		if(exp_red -> constant()) {
			auto exp_value = (*exp_red)(0,0,0,0,0,0);
			if(exp_value == 0) {
				return new value(1);
			}
			if(exp_value == 1) {
				return base_red;
			}
			if(exp_value < 0) {
				auto output = new ratio(new value(1), new power(base_red,
							new product(new value(-1), exp_red)));
				auto reduced = output -> reduce();
				delete output;
				return reduced;
			}
		}
		if(base_red -> get_type() == e_power) {
			auto cast = static_cast<const power*>(base_red);
			auto cast_base = cast -> base -> clone();
			auto cast_exp = cast -> exp -> clone();
			delete base_red;
			return new power(cast_base, new sum(exp_red, cast_exp));
		}
		return new power(base_red, exp_red);
	}
	std::ostream& power::print(std::ostream &os) const
	{
		if((base -> order()) > e_order_term) {
			os << "(" << *base << ")";
		} else {
			os << *base;
		}
		os << '^';
		if((exp -> order()) > e_order_term) {
			os << "(" << *exp << ")";
		} else {
			return os << *exp;
		}
	}
	power::power(function *base, function *exp):
		base(base), exp(exp), function() {}
	power::~power(void)
	{
		delete base;
		delete exp;
	}
}
