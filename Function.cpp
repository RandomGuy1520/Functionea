#pragma once
#include "Includes.cpp"
#include "exprtk/exprtk.hpp"
#define int long long
#define double long double

static struct ErrorType {};
static bool operator!= (ErrorType a, ErrorType b)
{
	return false;
}

namespace Functions
{
	static inline std::ostream& operator<< (std::ostream& os, ErrorType err)
	{
		os << "Error";
		return os;
	}
	static inline std::ostream& operator<< (std::ostream& os, std::variant<double, ErrorType> res)
	{
		try {
			os << std::get<double>(res);
		}
		catch (const std::bad_variant_access& ex) {
			os << std::get<ErrorType>(res);
		}
		return os;
	}
	static inline std::variant<double, ErrorType> evaluate_expr(const std::string& expr, const double& x)
	{
		double var_x;
		exprtk::symbol_table<double> symbol_table;
		exprtk::expression<double> expression;
		exprtk::parser<double> parser;
		symbol_table.add_variable("x", var_x, true);
		expression.register_symbol_table(symbol_table);
		var_x = x;
		parser.compile(expr, expression);
		double res = expression.value();
		if (std::isfinite(res)) return res;
		else return ErrorType();
	}
	const int MAXLEVEL = 5;
	enum Level { LEVEL1, LEVEL2, LEVEL3, LEVEL4, LEVEL5 };
	const int num[MAXLEVEL] = { 9, 10, 0, 0, 0 };
	const std::vector<std::vector<int>> points = {
		{ 1, 1, 1, 1, 1, 2, 2, 3 },
		{ 2, 2, 2, 2, 2, }
	};
	const std::vector<std::vector<std::string>> clues = {
		{"Waste of stars", "Reciprocal", "Maybe some kind of trigonometry?", "f : f(xy) = f(x) + f(y)", "Exponential", "Trigonometric trigonometry", "Why is the root of a tree a square?", "Yrtemonogirt", "Yes, contains logarithm"},
		{"Correct format", "Common time complexity function: 1e6 -> 2e7", "Grows faster than exp"}
	};
	const std::vector<std::vector<std::string>> exprs = {
		{"x", "1/x", "sin(x)", "log(x)", "exp(x)", "sin(cos(x))", "sqrt(sqrt(x))", "atan(x)", "sin(log(x))"},
		{"x*x", "x*log2(x)", "x^x", ""}
	};
};
