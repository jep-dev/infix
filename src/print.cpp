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
	//auto reduced = function::reduce(fn);
	auto reduced = function::reduce2(fn);
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
	//auto reduced = function::reduce(fn);
	auto reduced = function::reduce2(fn);
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

void print_columns(std::string src_lhs, std::string src_rhs,
		std::ostream &dest, int lwidth, int rwidth, std::string delim)
{
	std::vector<std::string> lines_lhs, lines_rhs;
	std::string line;
	for(int i = 0; i < 2; i++) {
		for(auto c : src_lhs) {
			if(c == '\n') {
				lines_lhs.emplace_back(line);
				line = "";
			} else {
				line += c;
			}
		}
		if(line.length()) {
			lines_lhs.emplace_back(line);
			line = "";
		}
		std::swap(src_lhs, src_rhs);
		std::swap(lines_lhs, lines_rhs);
	}
	/*
	std::istringstream iss_lhs(src_lhs), iss_rhs(src_rhs);
	while(std::getline(iss_lhs, line)) {
		lines_lhs.emplace_back(line);
	}
	while(std::getline(iss_rhs, line)) {
		lines_rhs.emplace_back(line);
	}
	*/
	for(auto l_it = std::begin(lines_lhs), r_it = std::begin(lines_rhs);
			l_it != std::end(lines_lhs) || r_it != std::end(lines_rhs);) {
		std::string lhs, rhs;
		if(l_it == std::end(lines_lhs)) {
			lhs = "";
		} else {
			lhs = *l_it;
			l_it++;
		}
		if(r_it == std::end(lines_rhs)) {
			rhs = "";
		} else {
			rhs = *r_it;
			r_it++;
		}
		auto len_lhs = lhs.length(), len_rhs = rhs.length();
		bool once = true;
		while(once || len_lhs || len_rhs) {
			once = false;
			if(len_lhs > lwidth) {
				dest << lhs.substr(0, lwidth);
				lhs = lhs.substr(lwidth);
			} else {
				dest << std::setw(lwidth) << std::right << lhs;
				lhs = "";
			}
			len_lhs = lhs.length();
			dest << delim;
			if(len_rhs > rwidth) {
				dest << rhs.substr(0, rwidth);
				rhs = rhs.substr(rwidth);
			} else {
				dest << std::setw(rwidth) << std::left << rhs;
				rhs = "";
			}
			len_rhs = rhs.length();
			dest << '\n';
		}
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
