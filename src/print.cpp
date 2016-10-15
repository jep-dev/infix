#include <algorithm>
#include <iomanip>
#include <iostream>
#include <functional>
#include <sstream>
#include <vector>

#include "print.hpp"

std::ostream& print(std::vector<Math::function*> &vec, std::ostream &os) {
	int count = 0;
	for(auto f : vec) {
		auto red = f -> reduce();
		os << (count ? ", " : "") << *red;
		delete red;
		count++;
	}
	return os;
}

std::ostream& print_order(Math::function const& fn,
		std::ostream &os, std::string pre)
{
	return os << pre << ' ' << fn.get_order() << std::endl;
}

std::ostream& print_factors(Math::function const& fn,
		std::ostream &os, std::string pre)
{
	using namespace Math;
	std::vector<function*> factors;
	get_factors(fn, factors);
	print(factors, os << pre << " {");
	os << "}\n";
	for(auto factor : factors)
		delete factor;
	return os;
}

std::ostream& print_terms(Math::function const& fn,
		std::ostream &os, std::string pre)
{
	using namespace Math;
	std::vector<function*> terms;
	get_terms(fn, terms);
	print(terms, os << pre << " {");
	os << "}\n";
	for(auto term : terms)
		delete term;
	return os;
}

std::ostream& print(Math::function const& fn,
		std::ostream &os, std::string prefix)
{
	using namespace Math;
	os << prefix << '(';
	int parameters = 0;
	for(int i = 0; i < e_param_total; i++) {
		auto param = (e_param)i;
		if(fn.varies(param)) {
			if(parameters > 0) {
				os << ", ";
			}
			os << param;
			parameters++;
		}
	}
	auto reduced = fn.reduce();
	os << ") = " << *reduced << '\n';
	delete reduced;
	return os;
}

std::ostream& print_derivative(Math::function const& fn, Math::e_param p,
		std::ostream &os, std::string prefix)
{
	using namespace Math;
	if(fn.varies(p)) {
		auto reduced = fn.reduce();
		auto dx = new variable(p);
		auto df_dx = fn.derive(p);
		std::ostringstream oss;
		oss << prefix << "df/d" << *dx << " ";
		print(*df_dx, os, oss.str());
		delete reduced;
		delete dx;
		delete df_dx;
	}
	return os;
}

std::ostream& print_derivatives(Math::function const& fn,
		std::ostream &os, std::string prefix)
{
	using namespace Math;
	for(int i = 0; i < e_param_total; i++) {
		print_derivative(fn, (e_param)i, os, prefix);
	}
	return os;
}

void columnate(std::string src, std::ostream &dest, int len) {
	std::vector<std::string> lines;
	std::string line;
	std::istringstream iss(src);
	while(std::getline(iss, line)) {
		std::ostringstream oss;
		oss << "| " << std::left << std::setw(len - 4) << line << " |";
		lines.emplace_back(oss.str().substr(0, len));
	}
	std::ostringstream oss;
	oss << std::setw(len-2) << std::setfill('-') << "";
	std::string bar = oss.str();
	dest << '.' << bar << ".." << bar << ".\n";
	int nLines = lines.size(),
		rhalf = nLines/2, lhalf = nLines - rhalf;
	for(int i = 0, j = 0; i < lhalf; i++, j++) {
		dest << lines[i];
		if(j < rhalf) {
			dest << lines[lhalf + j];
		} else {
			dest << "| " << std::setw(len - 4) << "" << " |";
		}
		dest << '\n';
	}
	dest << '\'' << bar << "\'\'" << bar << "\'\n";
}
