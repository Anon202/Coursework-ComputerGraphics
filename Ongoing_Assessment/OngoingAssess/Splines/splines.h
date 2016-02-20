


// Simple spline/interpolation functions


#pragma once

#include <vector>
using namespace std;

#include "vector3.h"


// Four simple interpolation functions:

// ** 1 **
 // a,b   - start and end points
 // t     - interpolation between 0.0 and 1.0
Vector3 Lerp( const Vector3& a, const Vector3& b, float t );


// ref: https://en.wikipedia.org/wiki/B%C3%A9zier_curve

// ** 2 ** 
 // bezCurve - array of three control points
 // t        - interpolation between 0.0 and 1.0
Vector3 QuadraticBezier( const vector<Vector3>& bezCurve, float t );


// ** 3 **
 // bezCurve - array of four control points
 // t        - interpolation between 0.0 and 1.0
Vector3 CubicBezier( const vector<Vector3>& bezCurve, float t );
 
 
// ref: https://en.wikipedia.org/wiki/Centripetal_Catmull%E2%80%93Rom_spline
 
 // Catmull-Rom splines have the advantage that the trajectory path 
 // passes through all of the control points.
 // ** 4 **
 // controlPoints - array of four control points
 // t             - interpolation between 0.0 and 1.0
 Vector3 CubicCatmullRom( const vector<Vector3>& controlPoints, float t );


 
 
 