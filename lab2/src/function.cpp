#include "function.h"
#include <iostream>
#include <armadillo>
#include <cmath>

void Function::IndefCoeff()
{
	arma::mat::fixed<5, 5> A;
	double h = 0;
	std::vector<double> coeff;
	for (int i = 0; i < 5; ++i)
	{
		coeff.push_back(f[i].first - x);
	}
	for (int j = 0; j < 5; j++)
	{
		for (int i = 0; i < 5; i++)
		{
			A(j, i) = pow(coeff[i], j);
		}
	}
	arma::vec B(5, arma::fill::zeros);
	B(1) = 1;
	arma::vec X = arma::solve(A, B);
	for (int i = 0; i < 5; i++)
	{
		IndefResult += X(i) * f[i].second;
	}
}

void Function::FindDerivative()
{
	double eps_1 = (e[0].first + e[1].first);
	double eps_2 = (e[0].first * (e[1].first + e[2].first) + e[1].first * e[2].first);
	double eps_3 = (e[0].first * e[2].first * (e[1].first + e[3].first) + e[1].first * e[3].first *
		(e[0].first + e[2].first));
	NewtonResult =  FindDivDiff(1) + FindDivDiff(2) * eps_1 + FindDivDiff(3) * eps_2 + FindDivDiff(4) * eps_3;

}

double Function::FindDivDiff(int IterationNum)
{
	double temp = 1;
	double sum = 0;
	for (int p = 0; p <= IterationNum; ++p)
	{
		for (int i = 0; i <= IterationNum; ++i)
		{
			if (i != p)
				temp *= 1 / (f[p].first - f[i].first);
		}
		sum += f[p].second * temp;
		temp = 1;
	}
	return sum;
}

Function::Function(std::vector<std::pair<double, double>> f_, double x_)
{
	f = f_;
	x = x_;
	std::pair<double, double> temp;
	for (auto it : f)
	{
		temp.first = x - it.first;
		temp.second = it.second;
		e.push_back(temp);
	}
	IndefCoeff();
	FindDerivative();
}

void Function::Print()
{
	std::cout << "Newton method result: " << NewtonResult << std::endl;
	std::cout << "Indef coeff method result: " << IndefResult << std::endl << std::endl;
}

Function::~Function()
{}
