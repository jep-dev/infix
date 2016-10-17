#ifndef PRINT_HPP
#define PRINT_HPP

#include "math.hpp"
#include <string>
#include <iosfwd>

std::ostream& print(Math::function const&,
		std::ostream&, std::string = "f");
std::ostream& print_order(Math::function const&,
		std::ostream&, std::string = "");

std::ostream& print_derivative(Math::function const&, Math::e_param,
		std::ostream&, std::string = "");
std::ostream& print_derivatives(Math::function const&,
		std::ostream&, std::string = "");

void columnate(std::string, std::ostream&, int = 40);

#endif
