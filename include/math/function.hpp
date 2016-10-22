#ifndef FUNCTION_HPP
#define FUNCTION_HPP

#include <vector>

namespace Math {

	struct function {
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

		static function* reduce(function const&);
		static function* reduce2(function const&);
	};

	std::vector<product*> shared_terms(std::vector<product*> const&,
			std::vector<product*> const&);
	function* factorize(function const&);
	function* get_product(std::vector<power*>&, bool collect = true);
	function* get_sum(std::vector<product*>&, bool collect = true);
	void add_factor(function *fn, bool, std::vector<power*>&);
	void add_factors(function*, bool, std::vector<power*>&);
	std::vector<power*> get_factors(function const&);
	std::vector<product*> get_terms(function const&);

	template<typename T> void erase_all(std::vector<T*> &vec);
}

#endif
