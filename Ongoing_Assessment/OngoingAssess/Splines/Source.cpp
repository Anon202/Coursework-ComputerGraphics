
#include "splines.h"
#include <iostream>
using namespace std;

void main()
{
	int enter;
	Vector3 a(1.0f, 1.0f, 1.0f);
	Vector3 b(2.0f, 2.0, 1.0f);
	float t = 0.5;

	Vector3 c = Lerp(a, b, t);

	cout << "test 1: LERP " << 
		c.x << ", " << c.y << ", " << c.z << endl;

	vector<Vector3> v;
	v.push_back(a);
	v.push_back(b);
	v.push_back(c);
	Vector3 d = QuadraticBezier(v, t);

	cout << "test 2: QuadBez " <<
		d.x << ", " << d.y << ", " << d.z << endl;

	cin >> enter;
}