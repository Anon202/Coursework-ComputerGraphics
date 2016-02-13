#include "intersection.h"
#include <iostream>

using namespace std;

int main()
{
	int enter;

	Vector3 point(3,1,4);
	Vector3 pointOnPlane(1, -2, 0);
	Vector3 planeNormal(2, -8, 5);

	bool a = IsPointInFrontOfPlane(point, pointOnPlane, planeNormal);

	cout << "Test if point is in front of plane (should be false) " << endl;
	if (a)
		cout << "true" << endl;
	else
		cout << "false" << endl;

	a = IsPointInFrontOfPlane(Vector3(3, 1, 5), pointOnPlane, planeNormal);
	cout << "Test if point is in front of plane (should be false) " << endl;
	if (a)
		cout << "true" << endl;
	else
		cout << "false" << endl;

	a = IsPointInFrontOfPlane(Vector3(10, 5, 1), pointOnPlane, planeNormal);
	cout << "Test if point is in front of plane (should be false) " << endl;
	if (a)
		cout << "true" << endl;
	else
		cout << "false" << endl;

	// WORKING

	// rayplane intersection
	Vector3 intersectionPoint;
	Vector3 rayOrigin(0, 1, 0);
	Vector3 rayDirection(1, 3, 1); // up y
	Vector3 planePoint(3, 1, 4);
	Vector3 planeN(2, -8, 5);

	bool b = RayPlaneIntersection(rayOrigin, rayDirection, planePoint, planeN, intersectionPoint);
	
	cout << "Ray plane intersection (same plane, with ray up y axis)" << endl;
	if (b)
	{
		cout << "true: x= " << intersectionPoint.x << " y= " << intersectionPoint.y << " z= " << intersectionPoint.z << endl;
	}


	cin >> enter;
	return 0;
}