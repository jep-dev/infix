#include "math.hpp"
#include "print.hpp"

#include <algorithm>
#include <iostream>
#include <iomanip>
#include <sstream>
#include <string>
#include <vector>

typedef enum {
	e_test_bind=0,
	e_test_substitute,
	e_test_derivatives,
	e_test_powers
} e_test;

void test(std::ostream &) {}
template<typename T1, typename... TN> void test(std::ostream &os)
{
	test<T1>(os);
	os << '\n';
	test<TN...>(os);
}
template<e_test E1, e_test... EN> void test(std::ostream &os)
{
	test<E1>(os);
	os << '\n';
	test<EN...>(os);
}

template<> void test<Math::value>(std::ostream &os)
{
	using namespace Math;
	auto x = new value(0);
	print(*x, os);
	print_derivatives(*x, os);
	delete x;
}

template<> void test<Math::variable>(std::ostream &os)
{
	using namespace Math;
	auto x = new variable(e_param_u);
	print(*x, os);
	print_derivatives(*x, os);
	delete x;
}

template<> void test<Math::negative>(std::ostream &os)
{
	using namespace Math;
	auto x = new negative(new variable(e_param_u));
	print(*x, os);
	print_derivatives(*x, os);
	delete x;
}

template<> void test<Math::cosine>(std::ostream &os)
{
	using namespace Math;
	auto x = new cosine(new variable(e_param_u));
	print(*x, os);
	print_derivatives(*x, os);
	delete x;
}

template<> void test<Math::sine>(std::ostream &os)
{
	using namespace Math;
	auto x = new sine(new product(new variable(e_param_u),
				new variable(e_param_v)));
	print(*x, os);
	print_derivatives(*x, os);
	delete x;
}

template<> void test<Math::tangent>(std::ostream &os)
{
	using namespace Math;
	auto x = new tangent(new variable(e_param_u));
	print(*x, os);
	print_derivatives(*x, os);
	delete x;
}

template<> void test<Math::secant>(std::ostream &os)
{
	using namespace Math;
	auto x = new secant(new variable(e_param_u));
	print(*x, os);
	print_derivatives(*x, os);
	delete x;
}

template<> void test<Math::cosecant>(std::ostream &os)
{
	using namespace Math;
	auto x = new cosecant(new variable(e_param_u));
	print(*x, os);
	print_derivatives(*x, os);
	delete x;
}

template<> void test<Math::cotangent>(std::ostream &os)
{
	using namespace Math;
	auto x = new cotangent(new variable(e_param_u));
	print(*x, os);
	print_derivatives(*x, os);
	delete x;
}

template<> void test<Math::arccosine>(std::ostream &os)
{
	using namespace Math;
	auto x = new arccosine(new power(new variable(e_param_u), new value(3)));
	print(*x, os);
	print_derivatives(*x, os);
	delete x;
}

template<> void test<Math::arcsine>(std::ostream &os)
{
	using namespace Math;
	auto x = new arcsine(new variable(e_param_u));
	print(*x, os);
	print_derivatives(*x, os);
	delete x;
}

template<> void test<Math::arctangent>(std::ostream &os)
{
	using namespace Math;
	auto x = new arctangent(new variable(e_param_u));
	print(*x, os);
	print_derivatives(*x, os);
	delete x;
}

template<> void test<Math::logarithm>(std::ostream &os)
{
	using namespace Math;
	auto x = new logarithm(new variable(e_param_u));
	print(*x, os);
	print_derivatives(*x, os);
	delete x;
}

template<> void test<Math::sum>(std::ostream &os)
{
	using namespace Math;
	auto x = new sum(new variable(e_param_v),
			new variable(e_param_w));
	print(*x, os);
	print_derivatives(*x, os);
	delete x;
}

template<> void test<Math::difference>(std::ostream &os)
{
	using namespace Math;
	auto x = new difference(new variable(e_param_v),
			new variable(e_param_w));
	print(*x, os);
	print_derivatives(*x, os);
	delete x;
}

template<> void test<Math::product>(std::ostream &os)
{
	using namespace Math;
	auto x = new product(new variable(e_param_v),
			new variable(e_param_w));
	print(*x, os);
	print_derivatives(*x, os);
	delete x;
}

template<> void test<Math::ratio>(std::ostream &os)
{
	using namespace Math;
	auto x = new ratio(new value(1), new variable(e_param_u));
	print(*x, os);
	print_derivatives(*x, os);
	delete x;
}

template<> void test<Math::power>(std::ostream &os)
{
	using namespace Math;
	auto x = new product(new power(new variable(e_param_u), new value(-2)),
			new variable(e_param_v));
	print(*x, os);
	print_derivatives(*x, os);
	delete x;
}

template<> void test<e_test_bind>(std::ostream &os)
{
	os << "Testing bindings\n";
	using namespace Math;
	function *bound, *x = new product(new product(new variable(e_param_u),
				new variable(e_param_v)), new variable(e_param_w));
	print(*x, os);
	e_param parameters[]{e_param_u, e_param_v, e_param_w};
	float values[]{1.0f, 2.0f, 3.0f};
	for(int i = 0; i < 3; i++) {
		auto parameter = new variable(parameters[i]);
		bound = x -> bind(parameters[i], values[i]);
		os << "  Binding " << *parameter << "=" << values[i] << "; ";
		delete parameter;
		print(*bound, os);
		delete x;
		x = bound;
	}
	delete x;
}

template<> void test<e_test_substitute>(std::ostream &os)
{
	os << "Testing substitutions\n";
	using namespace Math;
	function *x = new product(new variable(e_param_u),
				new variable(e_param_v)),
			*sub1 = new sum(new variable(e_param_w), new value(1)),
			*sub2 = new sum(new variable(e_param_w), new value(2)),
			*subbed1 = x -> substitute(e_param_u, sub1),
			*subbed2 = subbed1 -> substitute(e_param_v, sub2);
	print(*x, os);
	delete x;
	os << "Substituting u = w+1\n";
	print(*subbed1, os);
	delete subbed1;
	delete sub1;
	os << "Substituting v = w+2\n";
	print(*subbed2, os);
	delete subbed2;
	delete sub2;
}

template<> void test<e_test_derivatives>(std::ostream &os)
{
	os << "Testing repeated derivation\n";
	using namespace Math;
	function *y, *x = new cosine(new variable(e_param_u));
	print(*x, os);
	std::string prefix = "f";
	for(int i = 0; i < 4; i++) {
		y = x -> derive(e_param_u);
		delete x;
		os << "  ";
		prefix += "'";
		print(*y, os, prefix);
		x = y;
	}
	delete y;
}

template<> void test<e_test_powers>(std::ostream &os)
{
	os << "Testing variable base and exponent\n";
	using namespace Math;
	auto x = new power(new variable(e_param_u), new variable(e_param_u));
	print(*x, os);
	print_derivatives(*x, os);
	delete x;
}

int main(int argc, const char *argv[])
{
	using namespace Math;
	std::ostringstream oss;
	test<value, variable, negative,
		cosine, sine, tangent, secant, cosecant, cotangent,
		arccosine, arcsine, arctangent, logarithm,
		sum, difference, product, ratio, power>(oss);
	oss << '\n';
	test<e_test_bind, e_test_substitute,
		e_test_derivatives, e_test_powers>(oss);
	
	columnate(oss.str(), std::cout, 40);
}
