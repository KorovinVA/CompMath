#include <iostream>
#include "function.h"

const auto EXP_ = std::exp(1);

Function::Function(Function::Number num)
{
	FNumber = num;
	switch (num)
	{
	case A:
		Xmax = 1.5;
		Fmax = 1 / EXP_;
		break;
	case B:
		Xmax = 1 / std::sqrt(2);
		Fmax = std::sqrt(0.5) / std::pow(EXP_, 0.5);
		break;
	case C:
		Xmax = std::pow(EXP_, 0.5);
		Fmax = 1 / (2 * EXP_);
		break;
	case D:
		Xmax = EXP_;
		Fmax = 1 / EXP_;
		break;
	case E:
		Xmax = 1 / 2;
		Fmax = std::sqrt(0.5) * std::pow(EXP_, -1 / 2);
		break;
	default:
		break;
	}
	FindDx();
}

void Function::FindDx()
{
	double f = Fmax / 2;
	double x = Xmax;
	double prev = x;
	switch (FNumber)
	{
	case A:
	{
		while (true)
		{
			prev = x;
			x = x - x * std::pow(EXP_, 1 / (x - 2)) + f;
			if (x - prev < 0.001 && x - prev >  -0.001)
			{
				x1 = x;
				break;
			}
		}
		x = Xmax;
		while (true)
		{
			prev = x;
			x = x + (x * std::pow(EXP_, 1 / (x - 2)) - f) / (x - x1);
			if (x - prev < 0.001 && x - prev >  -0.001)
			{
				x2 = x;
				break;
			}
		}
		break;
	}
	case B:
	{
		while (true)
		{
			prev = x;
			x = x - x * std::pow(EXP_, -std::pow(x, 2)) + f;
			if (x - prev < 0.001 && x - prev >  -0.001)
			{
				x1 = x;
				break;
			}
		}
		x = Xmax;
		while (false)
		{
			prev = x;
			x = x - 4 * (x * std::pow(EXP_, -x * x) + f);
			if (x - prev < 0.001 && x - prev >  -0.001)
			{
				x2 = x;
				break;
			}
		}
		break;
	}
	case C:
	{
		while (true)
		{
			prev = x;
			x = x - std::log(x)/std::pow(x, 2) + f;
			if (x - prev < 0.001 && x - prev >  -0.001)
			{
				x1 = x;
				break;
			}
		}
		x = Xmax;
		while (true)
		{
			prev = x;
			x = 4 * EXP_ * std::log(x) / x;
			if (x - prev < 0.001 && x - prev >  -0.001)
			{
				x2 = x;
				break;
			}
		}
		break;
	}
	case D:
	{
		while (true)
		{
			prev = x;
			x = x - std::log(x)/x + f;
			if (x - prev < 0.001 && x - prev >  -0.001)
			{
				x1 = x;
				break;
			}
		}
		while (true)
		{
			prev = x;
			x = 2 * EXP_ *std::log(x);
			if (x - prev < 0.001 && x - prev >  -0.001)
			{
				x2 = x;
				break;
			}
		}
		break;
	}
	default:
		break;
	}
}
