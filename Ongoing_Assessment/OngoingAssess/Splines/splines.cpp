#include "splines.h"

#include <vector>
using namespace std;

#include "vector3.h"


// Four simple interpolation functions:

// ** 1 **
// a,b   - start and end points
// t     - interpolation between 0.0 and 1.0
Vector3 Lerp(const Vector3& a, const Vector3& b, float t)
{
	Vector3 lerp = a*(1 - t) + b*t;
	return lerp;
}


// ref: https://en.wikipedia.org/wiki/B%C3%A9zier_curve

// ** 2 ** 
// bezCurve - array of three control points
// t        - interpolation between 0.0 and 1.0
Vector3 QuadraticBezier(const vector<Vector3>& bezCurve, float t)
{
	Vector3 p0 = bezCurve[0];
	Vector3 p1 = bezCurve[1];
	Vector3 p2 = bezCurve[2];

	float mult = (1 - t);
	float multSq = mult*mult;

	Vector3 result = p0*multSq + (p1*2*t * mult) + p2*t*t;

	return result;
}


// ** 3 **
// bezCurve - array of four control points
// t        - interpolation between 0.0 and 1.0
Vector3 CubicBezier(const vector<Vector3>& bezCurve, float t)
{
	Vector3 p0 = bezCurve[0];
	Vector3 p1 = bezCurve[1];
	Vector3 p2 = bezCurve[2];
	Vector3 p3 = bezCurve[3];

	float mult = 1 - t;
	float multSq = mult*mult;
	float multCub = mult*multSq;
	
	Vector3 result = (p0*multCub) + (p1 * 3 * t*multSq) + (p2 * 3 * mult*t*t) + (p3*t*t*t);
	
	return result;
}


// ref: https://en.wikipedia.org/wiki/Centripetal_Catmull%E2%80%93Rom_spline

// Catmull-Rom splines have the advantage that the trajectory path 
// passes through all of the control points.
// ** 4 **
// controlPoints - array of four control points
// t             - interpolation between 0.0 and 1.0
Vector3 CubicCatmullRom(const vector<Vector3>& controlPoints, float t)
{
	Vector3 p0 = controlPoints[0];
	Vector3 p1 = controlPoints[1];
	Vector3 p2 = controlPoints[2];
	Vector3 p3 = controlPoints[3];

	float tSq = t*t;
	float tCub = tSq*t;


	Vector3 result = 
		((p1 * 2) +
		(-p0 + p2) * t +
		((p0*2)- (p1*5) + (p2*4) - p3) * tSq +
		(-p0 + (p1*3) - (p2*3) + p3) * tCub)
		* 0.5;

	return result;
}

