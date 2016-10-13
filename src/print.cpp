#include <algorithm>
#include <iomanip>
#include <iostream>
#include <functional>
#include <sstream>
#include <vector>

#include "print.hpp"

void print(std::vector<Math::function*> &vec, std::ostream &os) {
	int count = 0;
	for(auto f : vec) {
		auto red = f -> reduce();
		os << (count ? ", " : "") << *red;
		delete red;
		count++;
	}
}

void print_order(Math::function const& fn, std::ostream &os, std::string pre)
{
	os << pre << ' ' << fn.get_order() << std::endl;
}

void print_factors(Math::function const& fn, std::ostream &os, std::string pre)
{
	using namespace Math;
	std::vector<function*> num, den;
	get_factors(fn, num, den);
	print(num, os << pre << " {");
	print(den, os << "} / {");
	os << "}\n";
	for(auto n : num) {
		delete n;
	}
	for(auto d : den) {
		delete d;
	}
}

void print_terms(Math::function const& fn, std::ostream &os, std::string pre)
{
	using namespace Math;
	std::vector<function*> pos, neg;
	get_terms(fn, pos, neg);

	print(pos, os << pre << " {");
	print(neg, os << "} - {");
	os << "}\n";
	for(auto p : pos)
		delete p;
	for(auto n : neg)
		delete n;
}

void print(Math::function const& fn, std::ostream &os, std::string prefix)
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
}

void print_derivative(Math::function const& fn, Math::e_param p,
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
}

void print_derivatives(Math::function const& fn,
		std::ostream &os, std::string prefix)
{
	using namespace Math;
	for(int i = 0; i < e_param_total; i++) {
		print_derivative(fn, (e_param)i, os, prefix);
	}
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
