#include <iostream>
#include "function.h"

int main()
{
	Function a1({ {0, 0.5}, {1, 0.3}, {3, 0.3}, {4, 0.2}, {5, 0.1} }, 5);
	a1.Print();

	Function a2({ {0, 5}, {0.1, 2.5}, {0.2, 3}, {0.3, -2.5}, {0.4, -0.2} }, 0.3);
	a2.Print();

	Function a3({ {0, 4}, {1, 2.5}, {2, 1}, {3, -1}, {4, -2} }, 3);
	a3.Print();

	Function a4({ {0, 1}, {1, 0.5}, {2, 0.3}, {5, 0.2}, {7, 0.1} }, 2);
	a4.Print();

	Function a5({ {0, -1}, {2, 0}, {3, 2}, {5, 3}, {7, 5} }, 3);
	a5.Print();
}