#include <iomanip>
#include <iostream>
#include <sstream>
#include <vector>

#include "print.hpp"

void print(Math::function const& fn, std::ostream &os, std::string prefix)
{
	using namespace Math;
	os << prefix << "(";
	int parameters = 0;
	if(fn.varies(e_param_r)) {
		os << 'r';
		parameters++;
	}
	if(fn.varies(e_param_s)) {
		os << (parameters ? ", s" : "s");
		parameters++;
	}
	if(fn.varies(e_param_t)) {
		os << (parameters ? ", t" : "t");
		parameters++;
	}
	if(fn.varies(e_param_u)) {
		os << (parameters ? ", u" : "u");
		parameters++;
	}
	if(fn.varies(e_param_v)) {
		os << (parameters ? ", v" : "v");
		parameters++;
	}
	if(fn.varies(e_param_w)) {
		os << (parameters ? ", w" : "w");
	}
	auto reduced = fn.reduce();
	os << ") = " << *reduced << '\n';
	delete reduced;
}

void print_derivative(Math::function const& fn, Math::e_param p,
		std::ostream &os)
{
	using namespace Math;
	if(fn.varies(p)) {
		auto reduced = fn.reduce();
		auto dx = new variable(p);
		auto df_dx = fn.derive(p);
		std::ostringstream oss;
		oss << "  df/d" << *dx << " ";
		print(*df_dx, os, oss.str());
		delete reduced;
		delete dx;
		delete df_dx;
	}
}

void print_derivatives(Math::function const& fn, std::ostream &os)
{
	using namespace Math;
	for(int i = 0; i < e_param_total; i++) {
		print_derivative(fn, (e_param)i, os);
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
