#pragma once
#include "Includes.h"
#define EXPRTK_INIT() \
exprtk::symbol_table<double> symbol_table; \
exprtk::expression<double> expression; \
exprtk::parser<double> parser; \
add_constants(symbol_table)
#define int long long
#define double long double

const double e = 2.718281828459045235360287471352;
const double pi = 3.141592653589793238462643383279;
const double tau = 6.283185307179586476925286766559;
const double phi = 1.618033988749894848204586834365;

static struct ErrorType {}; // It's literally nothing
static bool IsError(std::variant<double, ErrorType> res)
{
	try {
		double k = std::get<double>(res);
	}
	catch (const std::bad_variant_access& ex) {
		return true;
	}
	return false;
}
static bool operator!= (std::variant<double, ErrorType> a, std::variant<double, ErrorType> b)
{
	if (a.index() != b.index()) return true;
	try {
		double da = std::get<double>(a), db = std::get<double>(b);
		return abs(da - db) > 1e-5;
	}
	catch (const std::bad_variant_access& ex) {
		return false;
	}
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
	static inline void add_constants(exprtk::symbol_table<double>& st)
	{
		st.add_constant("e", e);
		st.add_constant("pi", pi);
		st.add_constant("tau", tau);
		st.add_constant("phi", phi);
	}
	static inline bool is_const_expr(const std::string& expr)
	{
		EXPRTK_INIT();
		expression.register_symbol_table(symbol_table);
		return parser.compile(expr, expression);
	}
	static inline std::variant<double, ErrorType> evaluate_expr(const std::string& expr)
	{
		EXPRTK_INIT();
		expression.register_symbol_table(symbol_table);
		if (!parser.compile(expr, expression)) return ErrorType();
		double res = expression.value();
		if (std::isfinite(res)) return res;
		else return ErrorType();
	}
	static inline std::variant<double, ErrorType> evaluate_expr(const std::string& expr, const double& x)
	{
		double var_x;
		EXPRTK_INIT();
		symbol_table.add_variable("x", var_x);
		expression.register_symbol_table(symbol_table);
		var_x = x;
		if (!parser.compile(expr, expression)) return ErrorType();
		double value = expression.value();
		if (std::isfinite(value)) return value;
		else return ErrorType();
	}
	static inline std::vector<std::variant<double, ErrorType>> evaluate_expr(const std::string& expr, const std::vector<double>& x)
	{
		double var_x;
		EXPRTK_INIT();
		symbol_table.add_variable("x", var_x);
		expression.register_symbol_table(symbol_table);
		std::vector<std::variant<double, ErrorType>> res;
		if (!parser.compile(expr, expression))
		{
			for (double var : x)
				res.push_back(ErrorType());
			return res;
		}
		for (double var : x)
		{
			var_x = var;
			double value = expression.value();
			if (std::isfinite(value)) res.push_back(value);
			else res.push_back(ErrorType());
		}
		return res;
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
		{"Correct format", "Common time complexity function: 1e6 -> 2e7", "Similar, but grows faster than exp"}
	};
	const std::vector<std::vector<std::string>> exprs = {
		{"x", "1/x", "sin(x)", "log(x)", "exp(x)", "sin(cos(x))", "sqrt(sqrt(x))", "atan(x)", "sin(log(x))"},
		{"x*x", "x*log2(x)", "x^x"}
	};
};
