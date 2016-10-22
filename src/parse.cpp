#include <cmath>
#include <iostream>
#include <sstream>
#include <vector>

#include "parse.hpp"
#include "math.hpp"

int search_rtl(std::string const& word, std::string const& chars)
{
	bool found = false;
	int len = word.length();
	for(int pos = 0; pos < len; pos++) {
		char w = word[len - pos - 1];
		for(auto c : chars) {
			if(w == c) {
				return pos;
			}
		}
	}
	return -1;
}

int search_ltr(std::string const& word, std::string const& chars)
{
	bool found = false;
	int len = word.length();
	for(int pos = 0; pos < len; pos++) {
		char w = word[pos];
		for(auto c : chars) {
			if(w == c) {
				return pos;
			}
		}
	}
	return -1;
}

bool parse_constant(std::string const& word, float &value)
{
	bool char0 = true, neg = false, dot = false, num = false;
	int start = 0, stop = word.length();
	std::string permit = "0123456789";
	for(auto c : word) {
		if(char0) {
			if(c == '-') {
				start++;
				neg = true;
			}
			char0 = false;
		} else if(c == '-') {
			return false;
		}
		bool found = false;
		if(c == '.') {
			if(dot) {
				return false;
			}
			dot = true;
			found = true;
		}
		for(auto p : permit) {
			if(c == p) {
				num = true;
				found = true;
				break;
			}
		}
		if(!found) {
			return false;
		}
	}
	if(num) {
		std::istringstream iss(word);
		iss >> value;
		return true;
	}
	return false;
}

bool parse_alias(std::string const& word, float &value)
{
	if(word == "e") {
		value = M_E;
	} else if(word == "pi") {
		value = M_PI;
	} else {
		return false;
	}
	return true;
}

bool parse_symbol(std::string const& word, char &symbol)
{
	if(word == "r") {
		symbol = 'r';
	} else if(word == "s") {
		symbol = 's';
	} else if(word == "t") {
		symbol = 't';
	} else if(word == "u") {
		symbol = 'u';
	} else if(word == "v") {
		symbol = 'v';
	} else if(word == "w") {
		symbol = 'w';
	} else {
		return false;
	}
	return true;
}

bool parse_unary(std::string const& word,
		std::string &u_op, std::string &operand)
{
	std::vector<std::string> operands = {
		"acos", "asin", "atan",
		"sec", "csc", "cot",
		"cos", "sin", "tan",
		"log"
	};
	int w_len = word.length();
	for(auto op : operands) {
		int op_len = op.length();
		if(word.substr(0, op_len) == op) {
			u_op = op;
			operand = word.substr(op_len + 1, w_len - op_len - 2);
			return true;
		}
	}
	return false;
}

bool parse_negative(std::string const& word, std::string &operand)
{
	int depth = 0;
	for(int i = 0, len = word.length(); i < len; i++) {
		char c = word[i];
		if(c == '(') {
			depth++;
		} else if(c == ')') {
			depth--;
		} else if(depth == 0 && c == '-') {
			operand = word.substr(i+1, len - i - 1);
			return true;
		}
	}
	return false;
}

bool parse_binary(std::string const& word,
		char &b_op, std::string &lhs, std::string &rhs)
{
	int len = word.length();
	std::string bin_operators = "+/*^",
		operators = "-+/*^";
	bool found = false;
	for(auto op : operators) {
		int depth = 0;
		for(int i = 0; i < len; i++) {
			char c = word[i];
			if(c == '(') {
				depth++;
			} else if(c == ')') {
				depth--;
			} else if(depth == 0 && c == op) {
				lhs = word.substr(0, i);
				rhs = word.substr(i+1, len - i - 1);
				if(lhs != "" && rhs != "" &&
						search_rtl(lhs, bin_operators) != 0) {
					b_op = op;
					return true;
				}
			}
		}
	}
	return false;
}

expr* parse_expr(std::string const& src)
{
	char c_op, sym;
	float val;
	std::string lhs, rhs, s_op;
	if(search_ltr(src, "(") == 0 && search_rtl(src, ")") == 0) {
		int len = src.length(), depth = 0;
		bool found = false, found_early = false;
		for(int i = 0; i < len; i++) {
			if(src[i] == '(') depth++;
			else if(src[i] == ')') depth--;
			if(depth == 0) {
				found = true;
				if(i < len - 1) found_early = true;
				break;
			}
		}
		if(found && !found_early) {
			std::string sub = src.substr(1, len - 2);
			return parse_expr(sub);
		}
	}
	if(parse_binary(src, c_op, lhs, rhs)) {
		auto lparse = parse_expr(lhs), rparse = parse_expr(rhs);
		if(lparse && rparse)
			return new binary_expr(c_op, lparse, rparse);
		if(lparse) delete lparse;
		if(rparse) delete rparse;
	} else if(parse_negative(src, lhs)) {
		auto lparse = parse_expr(lhs);
		if(lparse) return new negative_expr(lparse);
	} else if(parse_unary(src, s_op, lhs)) {
		auto lparse = parse_expr(lhs);
		if(lparse) return new unary_expr(s_op, lparse);
	} else if(parse_symbol(src, sym))
		return new symbol_expr(sym);
	else if(parse_constant(src, val) || parse_alias(src, val))
		return new constant_expr(val);
	return nullptr;
}

bool get_param(char c, Math::e_param &p)
{
	using namespace Math;
	switch(c) {
		case 'r': p = e_param_r; break;
		case 's': p = e_param_s; break;
		case 't': p = e_param_t; break;
		case 'u': p = e_param_u; break;
		case 'v': p = e_param_v; break;
		case 'w': p = e_param_w; break;
		default: return false;
	}
	return true;
}

Math::function* get_function(expr *e)
{
	using namespace Math;
	switch(e -> get_type()) {
		case e_constant_expr: {
			auto cast = static_cast<const constant_expr*>(e);
			return new value(cast -> val);
		} break;
		case e_symbol_expr: {
			auto cast = static_cast<const symbol_expr*>(e);
			e_param param;
			return get_param(cast -> sym, param)
				? new variable(param) : nullptr;
		} break;
		case e_negative_expr: {
			auto cast = static_cast<const negative_expr*>(e);
			auto operand = get_function(cast -> operand);
			return operand ? new negative(operand) : nullptr;
		} break;
		case e_unary_expr: {
			auto cast = static_cast<const unary_expr*>(e);
			auto op = cast -> op;
			auto operand = get_function(cast -> operand);
			if(!operand) return nullptr;
			if(op == "acos") return new arccosine(operand);
			else if(op == "asin") return new arcsine(operand);
			else if(op == "atan") return new arctangent(operand);
			else if(op == "sec") return new secant(operand);
			else if(op == "csc") return new cosecant(operand);
			else if(op == "cot") return new cotangent(operand);
			else if(op == "cos") return new cosine(operand);
			else if(op == "sin") return new sine(operand);
			else if(op == "tan") return new tangent(operand);
			else if(op == "log") return new logarithm(operand);
			delete operand;
			return nullptr;
		} break;
		case e_binary_expr: {
			auto cast = static_cast<const binary_expr*>(e);
			auto op = cast -> op;
			auto lhs = get_function(cast -> lhs);
			auto rhs = get_function(cast -> rhs);
			if(lhs && rhs) {
				switch(op) {
					case '-': return new difference(lhs, rhs);
					case '+': return new sum(lhs, rhs);
					case '*': return new product(lhs, rhs);
					case '/': return new ratio(lhs, rhs);
					case '^': return new power(lhs, rhs);
					default:
						delete lhs;
						delete rhs;
						return nullptr;
				}
			}
			if(lhs) delete lhs;
			if(rhs) delete rhs;
			return nullptr;
		} break;
		default:
			return nullptr;
	}
}

std::string preparse(std::string const& src)
{
	std::string dest;
	std::vector<int> unclosed;
	int src_level = 0, dest_level = 0;
	char prev = ')';
	for(auto it = std::begin(src); it != std::end(src); it++) {
		char cur = *it;
		if(cur == '(') {
			src_level++;
			dest += cur;
		} else if(cur == ')') {
			src_level--;
			dest += cur;
		} else if(cur == '-') {
			switch(prev) {
				case '+': case '*': case '/':
				case '^': //case '(': case ')':
					break;
				default:
					dest += '+';
					break;
			}
			dest += "-(";
			unclosed.emplace_back(src_level);
			dest_level++;
		} else {
			dest += cur;
		}
		while(unclosed.size()) {
			auto last = unclosed.back();
			if(src_level <= last) {
				if(cur == '+' || cur == ')') {
					dest_level--;
					dest += ')';
					unclosed.pop_back();
				} else {
					break;
				}
			} else {
				break;
			}
		}
		prev = cur;
	}
	while(unclosed.size()) {
		dest += ')';
		unclosed.pop_back();
	}
	return dest;
	/*std::string dest;
	char prev = ')';
	for(auto cur : src) {
		if(cur == '-') {
			switch(prev) {
				case '+': case '/': case '*':
				case '^': case '(': case ')':
					break;
				default:
					dest += '+';
					break;
			}
		}
		dest += cur;
		prev = cur;
	}
	return dest;*/
}

Math::function* parse_function(std::string const& src)
{
	auto parsed = parse_expr(preparse(src));
	if(!parsed) return nullptr;
	auto output = get_function(parsed);
	delete parsed;
	return output;
}
