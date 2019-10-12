#include <iostream>

#include "Matrix.h"

int main()
{
	Matrix matrix;
	matrix.FindEigenValue();
	matrix.ApplyGauss();
	matrix.ApplySeidel();
	matrix.FindConditionNumber();
}