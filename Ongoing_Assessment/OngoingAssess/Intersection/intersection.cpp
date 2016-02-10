#include "intersection.h"
#include <math.h>

bool IsPointInFrontOfPlane(const Vector3& point, const Vector3& pointOnPlane, const Vector3& planeNormal)
{
	Vector3 difference = point - pointOnPlane;
	
	float dotD = Vector3::Dot(difference, planeNormal);

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
	 * Line into plane gives... dl + l' - p' dot n = 0
	 * expand dl.n + (l' - p').n = 0
	 * therefore d = (p' - l').n / l.n 
	 *
	 * if l.n != 0 d can be calculated
	 * then find point of intersection by line equation 
	 */

	float dotP = Vector3::Dot((pointOnPlane - rayOrigin), planeNormal);

	float dotL = Vector3::Dot(rayDirection, planeNormal);

	float d;

	if (dotL == 0) // line and plane is parallel
	{
		return false;
	}
	else
	{
		
		d = dotP / dotL;

		Vector3 out = rayDirection;
		out *= d;
		out + rayOrigin;

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
	out + rayOrigin;
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
	Vector3 bc = triPointC - triPointB;

	Vector3 planeNormal = Vector3::Cross(ab, bc);

	float denom = Vector3::Dot(planeNormal, planeNormal);

	// area of a triangle is half the length of the normal (NOT NORMALIZED)
	float areaABC = Vector3::Length(planeNormal) / 2;

	float nDotRayDir = Vector3::Dot(rayDirection, planeNormal);
	
	if (nDotRayDir == 0)  // PARALLEL !!
		return false;


	////////////

	float d = Vector3::Dot(planeNormal, triPointA);

	float t = Vector3::Dot(planeNormal, rayOrigin) + d / nDotRayDir;

	if (t < 0)	// triangle is behind
		return false;

	// compute intersepction point;
	Vector3 ray = rayDirection;
	ray *= t;

	Vector3 P = rayOrigin + ray; // middle of triangle


	Vector3 C; // perpendicular to plane

	/* ||AB x AP|| . N
	 *		N . N
	 * 
	 */

	// edge 1 AB
	Vector3 edge1 = ab;
	Vector3 ap = P - triPointA; 

	C = Vector3::Cross(edge1, ap);

	float vf = Vector3::Dot(planeNormal, C);
	if ( vf < 0) // not intersecting
		return false;

	vf /= denom;

	// edge 2 BC
	Vector3 edge2 = bc;
	Vector3 bp = P - triPointB;

	C = Vector3::Cross(edge2, bp);

	float uf = Vector3::Dot(planeNormal, C);
	if (uf< 0)
		return false;

	uf  /= denom;

	// edge 3 CA
	Vector3 edge3 = triPointA - triPointC; // ca
	Vector3 cp = P - triPointC;

	C = Vector3::Cross(edge3, cp);


	float wf =Vector3::Dot(planeNormal, C);
	if ( wf < 0)
		return false;

	wf /= denom;

	outIntersectionPoint.x = vf;
	outIntersectionPoint.y = uf;
	outIntersectionPoint.z = wf;


	return true;

}


// Returns the closest position on a plane for a given point
Vector3 ClosestPointToPlane(const Vector3& point,
	const Vector3& pointOnPlane, const Vector3& planeNormal)
{

	// ax + by + cz = d

	//Vector3 P = vd / v^2
	
	// v dot n = d
	float d = Vector3::Dot(planeNormal, (point - pointOnPlane));


	
	return P;
}

