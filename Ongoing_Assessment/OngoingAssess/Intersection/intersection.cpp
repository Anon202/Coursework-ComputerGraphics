#include "intersection.h"
#include <math.h>



#include <iostream>

bool IsPointInFrontOfPlane(const Vector3& point, const Vector3& pointOnPlane, const Vector3& planeNormal)
{
	Vector3 difference = point - pointOnPlane;
	
	float dotD = Vector3::Dot(difference, planeNormal);

	std::cout << "Dot : " << dotD << std::endl;

	if (dotD > 0)
	{
		return true;
	}
	else
	{
		
		return false;
	}

}

bool RayPlaneIntersection(const Vector3& rayOrigin, const Vector3& rayDirection,
	const Vector3& pointOnPlane, const Vector3& planeNormal,
	Vector3& outIntersectionPoint)
{
	/* plane normal dot 
	 *
   	 * Plane .. P - P' dot n = 0
	 * line .. P = dl + l'  (where d is real)   
	 * l vector in direction of line (rayDirection), l' is a point on the line (rayOrigin)
	 *
	 *position_along_line = point_on_line + some_float * line_direction;
	 *
	 * Line into plane gives... dl + l' - p' dot n = 0
	 * expand dl.n + (l' - p').n = 0
	 * therefore d = (p' - l').n / l.n 
	 *
	 * if l.n != 0 d can be calculated
	 * then find point of intersection by line equation 
	 */
	
	float dotD = Vector3::Dot((pointOnPlane - rayOrigin), planeNormal);

	float dotL = Vector3::Dot(rayDirection, planeNormal); // denom

	float d;

	if (dotL == 0) // line and plane is parallel as normal dot direction = 0 (perpendicular)
	{
		return false;
	}
	else
	{

		d = dotD / dotL;

		Vector3 out = rayDirection;
		out *= d;
		out += rayOrigin;												// fixed error 

		outIntersectionPoint = out;
		return true;
	}
	
}


// Returns true if the ray-sphere intersect - also calculates the intersection point
bool RaySphereIntersect(const Vector3& sphereCenter, const float sphereRadius,
	const Vector3& rayOrigin, const Vector3& rayDirection,
	Vector3& outIntersectionPoint)
{
	Vector3 difference = sphereCenter - rayOrigin;
	float tc = Vector3::Dot(difference, rayDirection);
	
	if (tc < 0)			// tc is distance in direction of the ray to the right angle of the origin. so if zero, could be behind rays origin as difference + ray will point in different directions
		return false;

	// d^2 * tc^2 = difference^2 pythag
	float difSq = Vector3::Dot(difference, difference);

	float d = sqrt(difSq - tc*tc);
	if (d < 0)
		return false;

	if (d > sphereRadius)
		return false;

	// d^2 + tx^2 = radius^2
	float tx = sqrt(sphereRadius*sphereRadius - d*d);
	
	float t0 = tc - tx;
	Vector3 out = rayDirection;
	out *= t0;
	out += rayOrigin;
	outIntersectionPoint = out;
	return true;
}

// Returns true if the ray-triangle intersect - also calculates the intersection point
bool RayTriangleIntersection(const Vector3& rayOrigin, const Vector3& rayDirection,
	const Vector3& triPointA, const Vector3& triPointB, const Vector3& triPointC,
	Vector3& outIntersectionPoint)
{
	// barycentric coordinates
	// Triangle ABC, ABP - v, BCP - w, CAP - u
	
	// treat ABC as a plane and get it's normal
	Vector3 ab = triPointB - triPointA;
	Vector3 ac = triPointC - triPointA;


	// find normal
	Vector3 planeNormal = Vector3::Cross(ab, ac);
	planeNormal = Vector3::Normalize(planeNormal);

	// find intersection point
	// R(t) = P + td
	// Intersection = rayOrigin + tdir
	// n.R(t) = d
	// n.P + tn.dir = d
	// t = d - n.P /n.dir

	// where d = n.point on plane (triPointA)

	float d = Vector3::Dot(planeNormal, triPointA);
	float t = (d - (Vector3::Dot(planeNormal, rayOrigin))) / (Vector3::Dot(planeNormal, rayDirection));

	// intersection = P + tD 
	Vector3 intersection = rayDirection;
	intersection *= t;
	intersection += rayOrigin;

	// normal dot ray >= 0 
	float nDotRayDir = Vector3::Dot(rayDirection, planeNormal);

	if (nDotRayDir >= 0)  // PARALLEL !!
		return false;
	

	// inside outside testing

	// edge one
	float e1 = Vector3::Dot(Vector3::Cross(ab, (intersection - triPointA)), planeNormal);

	float e2 = Vector3::Dot(Vector3::Cross((triPointC - triPointB), (intersection - triPointB)), planeNormal);

	float e3 = Vector3::Dot(Vector3::Cross(ac, (intersection - triPointC)), planeNormal);


	if (e1 < 0 || e2 < 0 || e3 < 0)
		return false;


	outIntersectionPoint = intersection;
	return true;

}


// Returns the closest position on a plane for a given point
Vector3 ClosestPointToPlane(const Vector3& point,
	const Vector3& pointOnPlane, const Vector3& planeNormal)
{
	// closest positon on a plabne is a projection of the normal vector
	// R = p +tn 
	
	// find intersection point
	// R(t) = P + td
	// Intersection = rayOrigin + tdir
	// n.R(t) = d
	// n.P + tn.dir = d
	// t = d - n.P /n.dir

	// where d = n.point on plane (triPointA)
	float d = Vector3::Dot(planeNormal, pointOnPlane); // plane eq
	float t = (d - Vector3::Dot(planeNormal, point)) / (Vector3::Dot(planeNormal, planeNormal));

	Vector3 P = planeNormal;
	P *= t;
	P += point;

	return P;
}

