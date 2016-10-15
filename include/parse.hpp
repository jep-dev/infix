#ifndef PARSE_HPP
#define PARSE_HPP

#include <iosfwd>
#include <string>
#include "expression.hpp"
#include "math.hpp"

int search_ltr(std::string const&, std::string const&);
int search_rtl(std::string const&, std::string const&);
bool parse_constant(std::string const&, float&);
bool parse_alias(std::string const&, float&);
bool parse_symbol(std::string const&, char&);
bool parse_unary(std::string const&, std::string&, std::string&);
bool parse_negative(std::string const&, std::string&);
bool parse_binary(std::string const&, char&, std::string&, std::string&);
expr* parse_expr(std::string const&);
bool get_param(char, Math::e_param&);
Math::function* get_function(expr*);
std::string preparse(std::string const&);
Math::function* parse_function(std::string const&);

#endif
