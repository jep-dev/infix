#ifndef PRINT_HPP
#define PRINT_HPP

#include "math.hpp"
#include <string>
#include <iosfwd>

void print(Math::function const&, std::ostream&, std::string = "f");
void print_order(Math::function const&, std::ostream&, std::string = "");

void print_factors(Math::function const&, std::ostream&, std::string = "");
void print_terms(Math::function const&, std::ostream&, std::string = "");

void print_derivative(Math::function const&, Math::e_param, std::ostream&);
void print_derivatives(Math::function const&, std::ostream&);
void columnate(std::string, std::ostream&, int = 40);

#endif
