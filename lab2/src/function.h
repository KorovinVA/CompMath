#include <vector>
#include <utility>

class Function
{
	std::vector<std::pair<double, double>> f;
	std::vector<std::pair<double, double>> e;
	double x;
	double NewtonResult = 0;
	double IndefResult = 0;
public:
	void IndefCoeff();
	void FindDerivative();
	void Print();
	double FindDivDiff(int IterationNum);
	Function(std::vector<std::pair<double, double>>, double);
	Function() = delete;
	Function(const Function&) = delete;
	~Function();
};
