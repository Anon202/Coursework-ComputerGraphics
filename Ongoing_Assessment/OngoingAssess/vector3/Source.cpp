
#include "vector3.h"
#include <iostream>

int main()
{
	int enter; // stops from closing debug

	Vector3 a;
	Vector3 b(1.0f, 1.0f, 1.0f);

	Vector3 c = a + b;

	std::cout << "Vector c " << std::endl;

	std::cin >> enter;

	return 0;
}