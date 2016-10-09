#ifndef POWER_HPP
#define POWER_HPP

#include "math.hpp"

namespace Math {
	
	struct power : public function {
		function *base, *exp;
		virtual e_type get_type(void) const;
		virtual bool constant(void) const;
		virtual bool varies(e_param) const;
		virtual e_order order(void) const;
		virtual bool operator==(function const&) const;
		virtual float operator()(float, float, float,
				float, float, float) const;
		virtual function* substitute(e_param, function*) const;
		virtual function* bind(e_param, float) const;
		virtual function* derive(e_param) const;
		virtual function* clone(void) const;
		virtual function* reduce(void) const;
		virtual std::ostream& print(std::ostream&) const;
		power(function*, function*);
		virtual ~power(void);
	};
	
}

#endif
