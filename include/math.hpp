#ifndef MATH_HPP
#define MATH_HPP

#include <iosfwd>
#include <cmath>
#include <vector>

namespace Math {
	
	typedef enum {
		// Nullary (operands)
		e_value=0, e_variable,
		// Unary operators
		e_negative, e_cosine, e_sine, e_tangent,
		e_secant, e_cosecant, e_cotangent,
		e_arccosine, e_arcsine, e_arctangent, e_logarithm,
		// Binary operators
		e_difference, e_sum, e_ratio, e_product, e_power,
		e_total
	} e_type;
	std::ostream& operator<<(std::ostream&, e_type const&);

	typedef enum {
		e_order_term,
		e_order_sum,
		e_order_ratio,
		e_order_product,
		e_order_exponent
	} e_order;
	std::ostream& operator<<(std::ostream&, e_order const&);

	typedef enum {
		e_param_r=0, e_param_s, e_param_t,
		e_param_u, e_param_v, e_param_w,
		e_param_total
	} e_param;
	std::ostream& operator<<(std::ostream&, e_param const&);

	// Abstract base
	struct function;
	// Nullary (operands)
	struct value;
	struct variable;
	// Unary operators
	struct negative;
	struct cosine;
	struct sine;
	struct tangent;
	struct secant;
	struct cosecant;
	struct cotangent;
	struct arccosine;
	struct arcsine;
	struct logarithm;
	// Binary operators
	struct difference;
	struct sum;
	struct ratio;
	struct product;

	void get_factors(function const&, std::vector<function*>&);
	void get_terms(function const&, std::vector<function*>&);

}

#include "math/function.hpp"
#include "math/value.hpp"
#include "math/variable.hpp"
#include "math/negative.hpp"
#include "math/cosine.hpp"
#include "math/sine.hpp"
#include "math/tangent.hpp"
#include "math/secant.hpp"
#include "math/cosecant.hpp"
#include "math/cotangent.hpp"
#include "math/arccosine.hpp"
#include "math/arcsine.hpp"
#include "math/arctangent.hpp"
#include "math/logarithm.hpp"
#include "math/difference.hpp"
#include "math/sum.hpp"
#include "math/ratio.hpp"
#include "math/product.hpp"
#include "math/power.hpp"

#endif
