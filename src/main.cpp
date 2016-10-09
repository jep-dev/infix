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
	if(w0 != "clear" && w0 != "unset" && w0 != "unbind")
		return false;
	char sym;
	if(!parse_symbol(w1, sym))
		return false;
	if(!get_param(sym, param))
		return false;
	return true;
}

bool parse_print(std::string const& src)
{
	return src == "set" || src == "let" || src == "print";
}

bool parse_clear(std::string const& src)
{
	std::istringstream iss(src);
	if(src == "clear" || src == "unset" || src == "unbind") {
		return true;
	}
	std::string w0, w1;
	iss >> w0;
	if(w0 == "clear" || w0 == "unset" || w0 == "unbind") {
		iss >> w1;
		if(w1 == "all")
			return true;
	}
	return false;
}

bool parse_quit(std::string const& src)
{
	return src == "" || src == "quit" || src == "exit";
}

bool parse_help(std::string const& src)
{
	return src == "help" || src == "usage";
}

void clear(std::vector<std::pair<Math::e_param, Math::function*>> &v)
{
	for(auto it = std::begin(v); it != std::end(v);) {
		delete it -> second;
		v.erase(it);
	}
}

void usage(void)
{
	std::cout << "Usage:\n"
		"  {|quit|exit}\n"
		"  {help|usage}\n"
		"  {let|set|print}\n"
		"  {clear|unset} {|all|r|s|t|u|v|w}\n"
		"  {let|set} {r|s|t|u|v|w} = <expression(r,s,t,u,v,w,e,pi)>\n"
		"  <expression(r,s,t,u,v,w,e,pi)>\n";
}

int main(int argc, const char *argv[])
{
	using namespace Math;
	std::string line;
	std::vector<std::pair<e_param, function*>> bindings;
	std::cout << "# ";
	while(std::getline(std::cin, line)) {
		e_param param;
		function *binding;
		if(parse_help(line)) {
			usage();
		} else if(parse_quit(line)) {
			break;
		} else if(parse_clear(line)) {
			clear(bindings);
		} else if(parse_print(line)) {
			for(auto it : bindings) {
				auto var = new variable(it.first);
				std::cout << *var << " = " << *it.second << '\n';
				delete var;
			}
		} else if(parse_unbind(line, param)) {
			for(auto it = std::begin(bindings); it != std::end(bindings);) {
				if(it -> first == param) {
					delete it -> second;
					bindings.erase(it);
				} else {
					it++;
				}
			}
		} else if(parse_bind(line, param, binding)) {
			bindings.emplace_back(param, binding);
		} else if(auto parsed = parse_function(line)) {
			for(auto binding : bindings) {
				auto bound = parsed ->
					substitute(binding.first, binding.second);
				delete parsed;
				parsed = bound;
			}
			auto reduced = parsed -> reduce();
			delete parsed;
			print(*reduced, std::cout);
			print_derivatives(*reduced, std::cout);
			delete reduced;
		} else {
			std::cout << "Expression parsing failed.\n";
			usage();
		}
		std::cout << "# ";
	}
	clear(bindings);
}
