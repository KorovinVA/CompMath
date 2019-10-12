#pragma once
#include <array>
#include <armadillo>

constexpr int MATRIX_LEN = 101;
constexpr int MATRIX_HIGH = 100;
constexpr double ERR = 0.0001;

class Matrix
{
	arma::mat::fixed<MATRIX_HIGH, MATRIX_LEN> ExtendedSystem;
	arma::mat::fixed<MATRIX_HIGH, MATRIX_LEN - 1> System;
	arma::vec::fixed<MATRIX_HIGH> f;
public:
	Matrix();
	void print();
	void FindEigenValue();
	void ApplyGauss();
	void ApplySeidel();
	void FindConditionNumber();
};