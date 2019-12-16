#pragma once
#include <vector>
#include <string>
#include <cmath>

//std::vector<std::string> FunctionName {"x*exp(1/(x-2)", "x*exp(-x^2)", "ln(x)/x^2", "ln(x)/x", "x^(0.5)*exp^(-x)"};

class Function
{
public:
	enum Number
	{
		A,
		B,
		C,
		D,
		E
	};
private:
	double Xmax;
	double Fmax;
	double dx = 0;
	Number FNumber;
public:
	Function() = delete;
	Function(Function::Number);
	void FindDx();
	double x1 = 0;
	double x2 = 0;
};