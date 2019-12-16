#include "function.h"
#include <iostream>

int main()
{
	Function a(Function::Number::A);
	std::cout << a.x1 << " " << a.x2 << std::endl;

	Function b(Function::Number::B);
	std::cout << b.x1 << " " << b.x2 << std::endl;

	Function c(Function::Number::C);
	std::cout << c.x1 << " " << c.x2 << std::endl;

	Function d(Function::Number::D);
	std::cout << d.x1 << " " << d.x2 << std::endl;
}