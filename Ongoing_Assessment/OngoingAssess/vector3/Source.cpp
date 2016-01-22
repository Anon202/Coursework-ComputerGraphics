
#include "vector3.h"
#include <iostream>

int main()
{
	int enter; // stops from closing debug

	Vector3 a;
	Vector3 b(1.0f, 10.0f, 1.0f);

	Vector3 c = a + b;

	std::cout << "Vector a " << a.x << ", " << a.y << ", " << a.z << std::endl;

	std::cout << "Vector b " << b.x << ", " << b.y << ", " << b.z << std::endl;

	std::cout << "Vector c " << c.x << ", " << c.y << ", " << c.z << std::endl;

	Vector3 d = b * 3;

	std::cout << "Vector d (b * 3) " << d.x << ", " << d.y << ", " << d.z << std::endl;


	Vector3 f(b);
	std::cout << "Vector f f(b) " << f.x << ", " << f.y << ", " << f.z << std::endl;

	Vector3 g(1.0f, 2.0f, 3.0f);

	
	std::cout << "Vector a " << a.x << ", " << a.y << ", " << a.z << std::endl;

	a += g;
	std::cout << "Vector a+=g " << a.x << ", " << a.y << ", " << a.z << std::endl;

	a -= b;
	std::cout << "Vector a-=b " << a.x << ", " << a.y << ", " << a.z << std::endl;

	a *= 5;
	std::cout << "Vector a*=5 " << a.x << ", " << a.y << ", " << a.z << std::endl;


	Vector3 e = b / 2;
	std::cout << "Vector e (b/2) " << e.x << ", " << e.y << ", " << e.z << std::endl; // 0.5, 5, 0.5

	g = g * 2;
	std::cout << "Vector g = g*2 " << g.x << ", " << g.y << ", " << g.z << std::endl;
	
	g = g - b;
	std::cout << "Vector g = g- b " << g.x << ", " << g.y << ", " << g.z << std::endl;

	Vector3 gMin = -g;
	std::cout << "Vector g minus " << gMin.x << ", " << gMin.y << ", " << gMin.z << std::endl;

	Vector3 r(2.0f, -7.0f, 1.0f);
	Vector3 p(-3.0f, 0.0f, 4.0f);

	Vector3 v = Vector3::Cross(p, r);

	std::cout << "Vector v (p x r) " << v.x << ", " << v.y << ", " << v.z << std::endl;  // answer should be 28, 11, 21
	
	v = Vector3::Cross(r, p);
	std::cout << "Vector v (r x p) " << v.x << ", " << v.y << ", " << v.z << std::endl;  // answer should be -28, -11, -21

	float w;
	w = Vector3::Dot(r, p);
	std::cout << "r dot p = " << w << std::endl;  // should be -2

	w = Vector3::Dot(p, r);
	std::cout << "p dot r = " << w << std::endl;  // should be -2

	w = Vector3::Dot(p, p);
	std::cout << "p dot p = " << w << std::endl; // should be 25

	w = Vector3::Dot(p, v);
	std::cout << "p dot v " << w << std::endl; // should be zero


	// normalise
	Vector3 pN = Vector3::Normalize(p);
	std::cout << "normalised p = " << pN.x << ", " << pN.y << ", " << pN.z << std::endl; // -0.6, 0, 0.8

	float pL = Vector3::Length(p);
	std::cout << "length of p " << pL << std::endl;			// should be 5

	float pL2 = Vector3::LengthSq(p);
	std::cout << "length squared p " << pL2 << std::endl;  // should be 25

	std::cin >> enter;

	return 0;
}