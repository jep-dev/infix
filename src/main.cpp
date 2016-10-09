#include <algorithm>
//#include <cmath>
#include <iostream>
#include <iomanip>
#include <limits>
#include <sstream>
#include <string>
#include <utility>
#include <vector>

#include "math.hpp"
#include "parse.hpp"
#include "print.hpp"

bool parse_bind(std::string const& src, Math::e_param &param,
		Math::function *& sub)
{
	std::istringstream iss(src);
	std::string w0, w1, w2, w3;
	iss >> w0 >> w1 >> w2 >> w3;
	if((w0 != "let" && w0 != "set" && w0 != "bind") || w2 != "=")
		return false;
	char sym;
	if(!parse_symbol(w1, sym))
		return false;
	if(!get_param(sym, param))
		return false;
	return sub = parse_function(w3);
}

bool parse_unbind(std::string const& src, Math::e_param &param)
{
	std::istringstream iss(src);
	std::string w0, w1;
	iss >> w0 >> w1;
	if(w0 != "unset" && w0 != "unbind")
		return false;
	char sym;
	if(!parse_symbol(w1, sym))
		return false;
	if(!get_param(sym, param))
		return false;
	return true;
}

int main(int argc, const char *argv[])
{
	using namespace Math;
	std::string line;
	std::vector<std::pair<e_param, function*>> bindings;
	while(std::getline(std::cin, line)) {
		if(line == "")
			break;
		if(auto parsed = parse_function(line)) {
			auto reduced = parsed -> reduce();
			delete parsed;
			for(auto binding : bindings) {
				auto bound = reduced ->
					substitute(binding.first, binding.second);
				delete reduced;
				auto bound_red = bound -> reduce();
				delete bound;
				reduced = bound_red;
			}
			print(*reduced, std::cout);
			print_derivatives(*reduced, std::cout);
			delete reduced;
		} else {
			e_param param;
			function *binding;
			if(parse_unbind(line, param)) {
				for(auto it = std::begin(bindings);
						it != std::end(bindings);) {
					if(it -> first == param) {
						delete it -> second;
						bindings.erase(it);
					} else {
						it++;
					}
				}
			} else if(parse_bind(line, param, binding)) {
				bindings.emplace_back(param, binding);
			} else {
				std::cout << "Expression parsing failed." << std::endl;
			}
		}
	}
	for(auto it = std::begin(bindings); it != std::end(bindings);) {
		delete it -> second;
		it++;
	}
}
