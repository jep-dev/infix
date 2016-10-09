#ifndef EXPRESSION_HPP
#define EXPRESSION_HPP

#include <string>

typedef enum {
	e_constant_expr = 0,
	e_symbol_expr,
	e_negative_expr,
	e_unary_expr,
	e_binary_expr
} e_expr_type;

struct expr {
	expr(void);
	virtual e_expr_type get_type(void) const = 0;
	virtual ~expr(void);
};

struct constant_expr : public expr {
	float val;
	virtual e_expr_type get_type(void) const;
	constant_expr(float);
	virtual ~constant_expr(void);
};

struct symbol_expr : public expr {
	char sym;
	virtual e_expr_type get_type(void) const;
	symbol_expr(char);
	virtual ~symbol_expr(void);
};

struct negative_expr : public expr {
	expr *operand;
	virtual e_expr_type get_type(void) const;
	negative_expr(expr*);
	virtual ~negative_expr(void);
};

struct unary_expr : public expr {
	std::string op;
	expr *operand;
	virtual e_expr_type get_type(void) const;
	unary_expr(std::string, expr*);
	virtual ~unary_expr(void);
};

struct binary_expr : public expr {
	char op;
	expr *lhs, *rhs;
	virtual e_expr_type get_type(void) const;
	binary_expr(char, expr*, expr*);
	virtual ~binary_expr(void);
};

#endif
