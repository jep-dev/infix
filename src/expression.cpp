#include "expression.hpp"

expr::expr(void) {}
expr::~expr(void) {}

e_expr_type constant_expr::get_type(void) const
{
	return e_constant_expr;
}
constant_expr::constant_expr(float val):
	val(val), expr() {}
constant_expr::~constant_expr(void) {}

e_expr_type symbol_expr::get_type(void) const
{
	return e_symbol_expr;
}
symbol_expr::symbol_expr(char sym):
	sym(sym), expr() {}
symbol_expr::~symbol_expr(void) {}

e_expr_type unary_expr::get_type(void) const
{
	return e_unary_expr;
}
unary_expr::unary_expr(std::string op, expr *operand):
	op(op), operand(operand), expr() {}
unary_expr::~unary_expr(void)
{
	if(operand) delete operand;
}

e_expr_type negative_expr::get_type(void) const
{
	return e_negative_expr;
}
negative_expr::negative_expr(expr *operand):
	operand(operand), expr() {}
negative_expr::~negative_expr(void)
{
	if(operand) delete operand;
}

e_expr_type binary_expr::get_type(void) const
{
	return e_binary_expr;
}
binary_expr::binary_expr(char op, expr *lhs, expr *rhs):
	op(op), lhs(lhs), rhs(rhs), expr() {}
binary_expr::~binary_expr(void)
{
	if(lhs) delete lhs;
	if(rhs) delete rhs;
}
