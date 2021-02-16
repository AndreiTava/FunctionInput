
#include <iostream>
#include <fstream>
#include <string>
#include <cmath>

enum class options
{
	function,
	oprator,
	parameter,
	constant
};

enum class functions
{
	invalid,
	abs,
	ceil,
	floor,
	trunc,
	round,
	exp,
	log,
	sqrt,
	cbrt,
	sin,
	cos,
	tan,
	asin,
	acos,
	atan,
	sinh,
	cosh,
	tanh,
	asinh,
	acosh,
	atanh,
	erf,
	erfc,
	tgamma
};

union data
{
	std::string func="\0";
	char op;
	float nr;
	data() 
	{
		op = '\0';
		nr = 0;
	};
	~data() {};
};

struct node
{
	options type = options::oprator;
	data field;
    node* left=nullptr;
    node* right=nullptr;
};

unsigned int findClosed(std::string str, unsigned int openPos)
{
	unsigned int count = 0;
	for (unsigned int i = openPos; i < str.length(); i++)
	{
		switch (str[i])
		{
		case '(':
		{
			count++;
			break;
		}
		case ')':
		{
			count--;
			break;
		}

		default:
			break;
		}
		if (count == 0)
			return i;
	}
	return 0;
}

functions resolveFunction(std::string fnc)
{
	if (fnc == "abs")
		return functions::abs;
	if (fnc == "sin")
		return functions::sin;
	if (fnc == "cos")
		return functions::cos;
	if (fnc == "tan")
		return functions::tan;
	if (fnc == "sqrt")
		return functions::sqrt;
	if (fnc == "log")
		return functions::log;
	if (fnc == "exp")
		return functions::exp;
	if (fnc == "ceil")
		return functions::ceil;
	if (fnc == "floor")
		return functions::floor;
	if (fnc == "trunc")
		return functions::trunc;
	if (fnc == "round")
		return functions::round;
	if (fnc == "cbrt")
		return functions::cbrt;
	if (fnc == "asin")
		return functions::asin;
	if (fnc == "acos")
		return functions::acos;
	if (fnc == "atan")
		return functions::atan;
	if (fnc == "sinh")
		return functions::sinh;
	if (fnc == "cosh")
		return functions::cosh;
	if (fnc == "tanh")
		return functions::tanh;
	if (fnc == "asinh")
		return functions::asinh;
	if (fnc == "acosh")
		return functions::acosh;
	if (fnc == "atanh")
		return functions::atanh;
	if (fnc == "erf")
		return functions::erf;
	if (fnc == "erfc")
		return functions::erfc;
	if (fnc == "tgamma")
		return functions::tgamma;
	return functions::invalid;

}

std::string processExpr(std::string expr)
{
	std::string newstr;
    unsigned int Pos = expr.find_first_of("0123456789");
    while (Pos != std::string::npos)
    {
        Pos = expr.find_first_of("0123456789", Pos);
    }
	return newstr;
};

node* extract(std::string expr)
{
    node* elem = new node;
    unsigned int Pos = expr.find("(");
	if (Pos != std::string::npos)
	{
		unsigned int closed = findClosed(expr, Pos);
		if (closed < expr.length() - 1)
		{
			elem->type = options::oprator;
			elem->field.op = expr[closed + 1];

			std::string left = expr.substr(Pos + 1, closed - Pos - 1);

			Pos = closed + 2;
			closed = findClosed(expr, Pos);
			std::string right = expr.substr(Pos + 1, closed - Pos - 1);

			elem->left = extract(left);
			elem->right = extract(right);

		}
		else
		{
			elem->type = options::function;
			elem->field.func= expr.substr(0, Pos);

			std::string right = expr.substr(Pos + 1, closed - Pos - 1);
			elem->right = extract(right);

		}
	}
	else
	{
		if (expr == "x")
		{
			elem->type = options::parameter;
		}
		else
		{
			elem->type = options::constant;
			elem->field.nr = stof(expr);
		}
	}
	return elem;
}
float computeTree(const node* start, float parameter)
{
	switch (start->type)
	{
	case options::constant:
	{
		return start->field.nr;
	}
	case options::parameter:
	{
		return parameter;
	}
	case options::oprator:
	{
		switch (start->field.op)
		{
		case '^':
		{
			return pow(computeTree(start->left, parameter), computeTree(start->right, parameter));
		}
		case '*':
		{
			return computeTree(start->left, parameter) * computeTree(start->right, parameter);
		}
		case '/':
		{
			return computeTree(start->left, parameter) / computeTree(start->right, parameter);
		}
		case '+':
		{
			return computeTree(start->left, parameter) + computeTree(start->right, parameter);
		}
		default:
		{
			return 0;
		}
		}
	}
	case options::function:
	{
		switch (resolveFunction(start->field.func))
		{
		case functions::invalid:
		{
			return 0;
		}
		case functions::abs:
		{
			return abs(computeTree(start->right, parameter));
		}
		case functions::sin:
		{
			return sin(computeTree(start->right, parameter));
		}
		case functions::cos:
		{
			return cos(computeTree(start->right, parameter));
		}
		case functions::tan:
		{
			return tan(computeTree(start->right, parameter));
		}
		case functions::sqrt:
		{
			return sqrt(computeTree(start->right, parameter));
		}
		case functions::log:
		{
			return log(computeTree(start->right, parameter));
		}
		case functions::exp:
		{
			return exp(computeTree(start->right, parameter));
		}
		case functions::ceil:
		{
			return ceil(computeTree(start->right, parameter));
		}
		case functions::floor:
		{
			return floor(computeTree(start->right, parameter));
		}
		case functions::trunc:
		{
			return trunc(computeTree(start->right, parameter));
		}
		case functions::round:
		{
			return round(computeTree(start->right, parameter));
		}
		case functions::cbrt:
		{
			return cbrt(computeTree(start->right, parameter));
		}
		case functions::asin:
		{
			return asin(computeTree(start->right, parameter));
		}
		case functions::acos:
		{
			return acos(computeTree(start->right, parameter));
		}
		case functions::atan:
		{
			return atan(computeTree(start->right, parameter));
		}
		case functions::sinh:
		{
			return sinh(computeTree(start->right, parameter));
		}
		case functions::cosh:
		{
			return cosh(computeTree(start->right, parameter));
		}
		case functions::tanh:
		{
			return tanh(computeTree(start->right, parameter));
		}
		case functions::asinh:
		{
			return asinh(computeTree(start->right, parameter));
		}
		case functions::acosh:
		{
			return acosh(computeTree(start->right, parameter));
		}
		case functions::atanh:
		{
			return atanh(computeTree(start->right, parameter));
		}
		case functions::erf:
		{
			return erf(computeTree(start->right, parameter));
		}
		case functions::erfc:
		{
			return erfc(computeTree(start->right, parameter));
		}
		case functions::tgamma:
		{
			return tgamma(computeTree(start->right, parameter));
		}
		}

	}
	default:
	{
		return 0;
	}
	}
}
int main()
{
	std::string expr = "(sin((x)+(2)))+((-1)*(log(x)))";
	node* operation = extract(expr);
	float result = computeTree(operation, 5.f);
	std::cout << result;
}


