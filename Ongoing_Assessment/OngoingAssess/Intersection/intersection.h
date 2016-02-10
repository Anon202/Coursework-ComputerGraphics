


// Simple intersection tests

#pragma once

#include "vector3.h"


// For further reading - see http://www.realtimerendering.com/intersections.html



// Returns true if the point is in front of the plane
bool IsPointInFrontOfPlane(const Vector3& point,
	const Vector3& pointOnPlane, const Vector3& planeNormal);


// Returns true if the ray-plane intersect - also calculates intersection point
bool RayPlaneIntersection(const Vector3& rayOrigin, const Vector3& rayDirection,
	const Vector3& pointOnPlane, const Vector3& planeNormal,
	Vector3& outIntersectionPoint);

// Returns true if the ray-sphere intersect - also calculates the intersection point
bool RaySphereIntersect(const Vector3& sphereCenter, const float sphereRadius,
	const Vector3& rayOrigin, const Vector3& rayDirection,
	Vector3& outIntersectionPoint);

// Returns true if the ray-triangle intersect - also calculates the intersection point
bool RayTriangleIntersection(const Vector3& rayOrigin, const Vector3& rayDirection,
	const Vector3& triPointA, const Vector3& triPointB, const Vector3& triPointC,
	Vector3& outIntersectionPoint);


// Returns the closest position on a plane for a given point
Vector3 ClosestPointToPlane(const Vector3& point,
	const Vector3& pointOnPlane, const Vector3& planeNormal);

