#ifndef FUNCTION_HPP
#define FUNCTION_HPP

#include <vector>

namespace Math {

	struct function {
		static int num_operands(function const&);

		virtual e_type get_type(void) const = 0;
		virtual e_order get_order(void) const = 0;
		//virtual int get_operands(void) const;
		virtual bool constant(void) const = 0;
		virtual bool varies(e_param) const = 0;
		bool operator<(function const&) const;
		bool operator==(float) const;
		virtual bool operator==(function const&) const = 0;
		bool operator!=(function const&) const;
		virtual float operator()(float, float, float,
				float, float, float) const = 0;
		virtual function* substitute(e_param, function*) const = 0;
		virtual function* bind(e_param, float val) const = 0;
		virtual function* derive(e_param) const = 0;
		virtual function* clone(void) const = 0;
		virtual function* reduce(void) const = 0;
		virtual std::ostream& print(std::ostream& os) const = 0;
		friend std::ostream& operator<<
			(std::ostream &os, function const& fn);
		function(void);
		virtual ~function(void);
	};

}

#endif
