
#include "vector3.h"
#include <iostream>

int main()
{
	int enter; // stops from closing debug

	Vector3 a;
	Vector3 b(1.0f, 10.0f, 1.0f);

	// need to ask about RHS?

	Vector3 c = a + b;

	std::cout << "Vector a " << a.x << ", " << a.y << ", " << a.z << std::endl;

	std::cout << "Vector b " << b.x << ", " << b.y << ", " << b.z << std::endl;

	std::cout << "Vector c " << c.x << ", " << c.y << ", " << c.z << std::endl;



	Vector3 r(2.0f, -7.0f, 1.0f);
	Vector3 p(-3.0f, 0.0f, 4.0f);

	Vector3 v;
	v = v.Cross(p, r);

	std::cout << "Vector v " << v.x << ", " << v.y << ", " << v.z << std::endl;  // answer should be 28, 11, 21





	std::cin >> enter;

	return 0;
}