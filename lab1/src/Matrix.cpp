#include "Matrix.h"
#include <iostream>
#include <cmath>


Matrix::Matrix()
{
	ExtendedSystem.zeros();
	int current = 1;	//current number in a column f (A * x = f)
	for (int j = 0; j < MATRIX_HIGH; j++)
	{
		for (int i = 0; i < MATRIX_LEN - 1; i++)
		{
			if (i < j - 4 || i > j + 4)
			{
				ExtendedSystem(j, i) = 0;
				System(j, i) = 0;
			}
			else if (i != j)
			{
				ExtendedSystem(j, i) = 1;
				System(j, i) = 1;
			}
			else
			{
				ExtendedSystem(j, i) = 10;
				System(j, i) = 10;
			}
		}
		ExtendedSystem(j, MATRIX_LEN - 1) = current;
		++current;
	}
	for (int i = 0; i < MATRIX_HIGH; ++i)
	{
		f(i) = i + 1;
	}
	std::cout << "The matrix of the system is successfully created." << std::endl << std::endl;
}

void Matrix::print()
{
	ExtendedSystem.print();
}

void Matrix::FindEigenValue()
{
	std::cout << "Trying to find eigenvalues..." << std::endl;
	arma::vec eigval(MATRIX_HIGH, arma::fill::zeros);
	arma::eig_sym(eigval, System);
	std::cout << "Lambda(min) = " << eigval[0] << std::endl;
	std::cout << "Lambda(max) = " << eigval.max() << std::endl << std::endl;
}

void Matrix::ApplyGauss()
{
	std::cout << "Using Gaussian elimination with total pivoting..." << std::endl;

	arma::mat::fixed<MATRIX_HIGH, MATRIX_LEN> A(ExtendedSystem);
	int pivot = 0;	//Number of the row with a pivot element

	//Make upper triangular matrix
	for (int j = 0, i = 0; j < MATRIX_HIGH - 1 && i < MATRIX_LEN; ++j, ++i)
	{
		pivot = j;
		for (int n = j; n < MATRIX_HIGH; ++n)
		{
			if (A(n, i) > A(pivot, i))
			{
				pivot = n;
			}
		}
		A.swap_rows(j, pivot);

		for (int n = j + 1; n < MATRIX_HIGH; ++n)
		{
			double factor = A(n, i) / A(j, i);
			for (int m = i; m < MATRIX_LEN; ++m)
			{
				A(n, m) -= A(j, m) * factor;
			}
		}
	}

	//X vector calculation
	arma::vec X(MATRIX_LEN - 1, arma::fill::zeros);
	for (int i = 0, n = MATRIX_HIGH - 1; i < MATRIX_LEN - 1 && n >= 0; ++i, --n)
	{
		for (int k = i - 1, m = MATRIX_LEN - 1 - i; k >= 0 &&  m <= MATRIX_LEN - 2; --k, ++m)
		{
			X(i) -= X(k) * A(n, m);
		}

		X(i) += A(n, MATRIX_LEN - 1);
		X(i) = X(i) / A(n, MATRIX_LEN - 2 - i);
	}
	arma::vec solution(MATRIX_LEN - 1, arma::fill::zeros);
	for (int i = 0; i < MATRIX_LEN - 1; i++)
	{
		solution(i) = X(MATRIX_LEN - 2 - i);
	}

	std::cout << "Success!The soulution vector : " << std::endl;
	solution.print();
	std::cout << std::endl;

	arma::vec r = f - System * solution;
	std::cout << "The vector of discrepancys for Gaussian elimination:" << std::endl;
	r.print();
	std::cout << std::endl;
}

void Matrix::ApplySeidel()
{
	std::cout << "Using Seidel method..." << std::endl;

	//Creating all matrices we will work with
	arma::mat L = arma::trimatl(System);
	L.diag().fill(0);
	arma::mat U = arma::trimatu(System);
	U.diag().fill(0);
	arma::mat D = arma::diagmat(System);
	arma::mat LD = L + D;
	arma::mat invLD = arma::inv(LD);

	//We will need two vectors to store our solution
	arma::vec u(MATRIX_LEN - 1, arma::fill::zeros);
	arma::vec prev(MATRIX_LEN - 1, arma::fill::zeros);

	for (; ;)
	{
		prev = u;
		u = -invLD * U * u + invLD * f;
		if (std::abs((u(0) - prev(0)) / u(0)) <= ERR)
		{
			break;
		}
	}
	std::cout << "Success!The soulution vector : " << std::endl;
	u.print();
	std::cout << std::endl;
	
	arma::vec r(MATRIX_LEN - 1, arma::fill::zeros);
	r = f - System * u;
	std::cout << "The vector of discrepancys for Seidel method:" << std::endl;
	r.print();
	std::cout << std::endl;
}

void Matrix::FindConditionNumber()
{
	std::cout << "Finding condition number..." << std::endl;
	double Norm = 0;
	double invNorm = 0;
	arma::mat invSystem = arma::inv(System);
	for (int i = 0; i < MATRIX_HIGH; ++i)
	{
		Norm += std::abs(System(i, 0));
		invNorm += std::abs(invSystem(i, 0));
	}
	std::cout << "Success!The condition number : " << std::endl;
	std::cout << Norm * invNorm << std::endl;
	std::cout << std::endl;
}
