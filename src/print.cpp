#include <algorithm>
#include <iomanip>
#include <iostream>
#include <functional>
#include <sstream>
#include <vector>

#include "print.hpp"

std::ostream& print_order(Math::function const& fn,
		std::ostream &os, std::string pre)
{
	return os << pre << ' ' << fn.get_order() << std::endl;
}

std::ostream& print(Math::function const& fn,
		std::ostream &os, std::string prefix)
{
	using namespace Math;
	os << prefix << '(';
	int parameters = 0;
	auto reduced = function::reduce(fn);
	//auto reduced = fn.reduce();
	for(int i = 0; i < e_param_total; i++) {
		auto param = (e_param)i;
		if(reduced -> varies(param)) {
			os << (parameters++ > 0 ? ", " : "") << param;
		}
	}
	os << ") = " << *reduced << '\n';
	delete reduced;
	return os;
}

std::ostream& print_derivative(Math::function const& fn, Math::e_param p,
		std::ostream &os, std::string prefix)
{
	using namespace Math;
	auto reduced = function::reduce(fn);
	//auto reduced = fn.reduce();
	if(reduced -> varies(p)) {
		auto dx = new variable(p);
		auto df_dx = reduced -> derive(p);
		print(*df_dx, os << prefix << "df/d" << *dx << ' ', "");
		delete dx;
		delete df_dx;
	}
	delete reduced;
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
