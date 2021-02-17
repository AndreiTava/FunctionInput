
#include <iostream>
#include <fstream>
#include <string>
#include <cmath>


class node
{

private:
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
		functions func = functions::invalid;
		char op;
		float nr;
		data()
		{
			op = '\0';
			nr = 0;
		};
		~data() {};
	};

private:
	options type = options::oprator;
	data field;
    node* left=nullptr;
    node* right=nullptr;

public:
	node(std::string);
	~node()
	{
		delete left;
		delete right;
	}
	float const computeTree(float parameter);
	void simplifyTree();
	static functions resolveFunction(std::string fnc);
};

unsigned int findPair(std::string str, unsigned int pos, bool open)
{
	unsigned int count = 0;
	if (open)
	{
		for (unsigned int i = pos; i < str.length(); i++)
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
	}
	else
	{
		for (unsigned int i = pos; i > 0; i--)
		{
			switch (str[i])
			{
			case '(':
			{
				count--;
				break;
			}
			case ')':
			{
				count++;
				break;
			}

			default:
				break;
			}
			if (count == 0)
				return i;
		}
	}
	return 0;
}


std::string processExpr(std::string expr)
{
	
    unsigned int Pos = expr.find('-');
	while (Pos > 0 && Pos !=std::string::npos && expr[Pos - 1] != '(')
	{
		expr.insert(Pos, 1, '+');
		Pos += 2;
		Pos = expr.find('-', Pos);
	}
	return expr;
};

node::functions node::resolveFunction(std::string fnc)
{
	if (fnc == "abs")
		return node::functions::abs;
	if (fnc == "sin")
		return node::functions::sin;
	if (fnc == "cos")
		return node::functions::cos;
	if (fnc == "tan")
		return node::functions::tan;
	if (fnc == "sqrt")
		return node::functions::sqrt;
	if (fnc == "log")
		return node::functions::log;
	if (fnc == "exp")
		return node::functions::exp;
	if (fnc == "ceil")
		return node::functions::ceil;
	if (fnc == "floor")
		return node::functions::floor;
	if (fnc == "trunc")
		return node::functions::trunc;
	if (fnc == "round")
		return node::functions::round;
	if (fnc == "cbrt")
		return node::functions::cbrt;
	if (fnc == "asin")
		return node::functions::asin;
	if (fnc == "acos")
		return node::functions::acos;
	if (fnc == "atan")
		return node::functions::atan;
	if (fnc == "sinh")
		return node::functions::sinh;
	if (fnc == "cosh")
		return node::functions::cosh;
	if (fnc == "tanh")
		return node::functions::tanh;
	if (fnc == "asinh")
		return node::functions::asinh;
	if (fnc == "acosh")
		return node::functions::acosh;
	if (fnc == "atanh")
		return node::functions::atanh;
	if (fnc == "erf")
		return node::functions::erf;
	if (fnc == "erfc")
		return node::functions::erfc;
	if (fnc == "tgamma")
		return node::functions::tgamma;
	return node::functions::invalid;

}

node::node(std::string expr)
{
    unsigned int Pos = expr.find("(");
	if (Pos != std::string::npos)
	{
		unsigned int closed = findPair(expr, Pos, 1);
		if (closed < expr.length() - 1)
		{
			type = options::oprator;
			field.op = expr[closed + 1];

			std::string leftexpr = expr.substr(Pos + 1, closed - Pos - 1);

			Pos = closed + 2;
			closed = findPair(expr, Pos, 1);
			std::string rightexpr = expr.substr(Pos + 1, closed - Pos - 1);

			left = new node(leftexpr);
			right = new node(rightexpr);

		}
		else
		{
			type = options::function;
			field.func = resolveFunction(expr.substr(0, Pos));

			std::string rightexpr = expr.substr(Pos + 1, closed - Pos - 1);
			right = new node(rightexpr);

		}
	}
	else
	{
		if (expr == "x")
		{
			type = options::parameter;
		}
		else
		{
			type = options::constant;
			field.nr = stof(expr);
		}
	}
}

void node::simplifyTree()
{
	if (left != nullptr && right != nullptr)
	{
		left->simplifyTree();
		right->simplifyTree();
		if (left->type == options::constant && right->type == options::constant)
		{
			field.nr = this->computeTree(0);
			type = options::constant;
			delete left;
			left = nullptr;
			delete right;
			right = nullptr;
		}
	}
	else
	{
		if (right != nullptr)
		{
			right->simplifyTree();
			if (right->type == options::constant)
			{
				field.nr = this->computeTree(0);
				type = options::constant;
				delete right;
				right = nullptr;
			}
		}
	}
}

float const node::computeTree(float parameter)
{
	switch (type)
	{
	case options::constant:
	{
		return field.nr;
	}
	case options::parameter:
	{
		return parameter;
	}
	case options::oprator:
	{
		switch (field.op)
		{
		case '^':
		{
			return pow(left->computeTree(parameter), right->computeTree(parameter));
		}
		case '*':
		{
			return left->computeTree(parameter) * right->computeTree(parameter);
		}
		case '/':
		{
			return left->computeTree(parameter) / right->computeTree(parameter);
		}
		case '+':
		{
			return left->computeTree(parameter) + right->computeTree(parameter);
		}
		default:
		{
			return 0;
		}
		}
	}
	case options::function:
	{
		switch (field.func)
		{
		case functions::invalid:
		{
			return 0;
		}
		case functions::abs:
		{
			return abs(right->computeTree(parameter));
		}
		case functions::sin:
		{
			return sin(right->computeTree(parameter));
		}
		case functions::cos:
		{
			return cos(right->computeTree(parameter));
		}
		case functions::tan:
		{
			return tan(right->computeTree(parameter));
		}
		case functions::sqrt:
		{
			return sqrt(right->computeTree(parameter));
		}
		case functions::log:
		{
			return log(right->computeTree(parameter));
		}
		case functions::exp:
		{
			return exp(right->computeTree(parameter));
		}
		case functions::ceil:
		{
			return ceil(right->computeTree(parameter));
		}
		case functions::floor:
		{
			return floor(right->computeTree(parameter));
		}
		case functions::trunc:
		{
			return trunc(right->computeTree(parameter));
		}
		case functions::round:
		{
			return round(right->computeTree(parameter));
		}
		case functions::cbrt:
		{
			return cbrt(right->computeTree(parameter));
		}
		case functions::asin:
		{
			return asin(right->computeTree(parameter));
		}
		case functions::acos:
		{
			return acos(right->computeTree(parameter));
		}
		case functions::atan:
		{
			return atan(right->computeTree(parameter));
		}
		case functions::sinh:
		{
			return sinh(right->computeTree(parameter));
		}
		case functions::cosh:
		{
			return cosh(right->computeTree(parameter));
		}
		case functions::tanh:
		{
			return tanh(right->computeTree(parameter));
		}
		case functions::asinh:
		{
			return asinh(right->computeTree(parameter));
		}
		case functions::acosh:
		{
			return acosh(right->computeTree(parameter));
		}
		case functions::atanh:
		{
			return atanh(right->computeTree(parameter));
		}
		case functions::erf:
		{
			return erf(right->computeTree(parameter));
		}
		case functions::erfc:
		{
			return erfc(right->computeTree(parameter));
		}
		case functions::tgamma:
		{
			return tgamma(right->computeTree(parameter));
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
	std::string expr = "((((-1)*(sin(x)))/(2))+(((-1)*(log((2)^(x))))*((-2)+((x)^(3)))))";
	node operation(expr);
	std::cout << operation.computeTree(0.5);
}

