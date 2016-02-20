/*
 * Created by Zoe Wall
 * Last modified 23/01/2016
 * Vector3.cpp - Implementation of the vector3 header file */

#include "vector3.h"	 // include header - vector 3 interface
#include <math.h>		 // include Math header to enable use of sqrt() function.

Vector3::Vector3() : x(0), y(0), z(0)  // empty Vector3 constructor, create 0, 0, 0 vector
{}

Vector3::Vector3(float x, float y, float z) : x(x), y(y), z(z)  // x, y, z constructor
{}

Vector3::Vector3(const Vector3& rhs) : x(rhs.x), y(rhs.y), z(rhs.z) // rhs constructor Vector3 a(rhs)
{}

void Vector3::operator+= (const Vector3 &v)  // overload operator - give it a new meaning += will add vector components together
{
	this->x += v.x;		// this instance of x + arg v's x component and so on
	this->y += v.y;
	this->z += v.z;
}

void Vector3::operator-= (const Vector3 &v)  // change this vector, subtract arg v components.
{
	this->x -= v.x;
	this->y -= v.y;
	this->z -= v.z;
}

void Vector3::operator*= (const float s)  // multiply each component by a scalar s.
{
	this->x *= s;
	this->y *= s;
	this->z *= s;
}

Vector3 Vector3::operator/ (const float s) const
{
	return Vector3(x/s, y/s, z/s);						// divide components by s return new vector
}

Vector3 Vector3::operator- (const Vector3 &v) const     // subtract corresponding components and return new vector
{
	return Vector3(x - v.x, y - v.y, z - v.z);
}

Vector3 Vector3::operator+  (const Vector3 &v) const	// add corresponding components and return new vector.
{
	return Vector3(x + v.x, y + v.y, z + v.z);
}

Vector3 Vector3::operator*  (const float s) const      // multiply components of a vector and store in new vector.
{
	return Vector3(x*s, y*s, z*s);
}

Vector3 Vector3::operator- () const   // unary minus, return negative of vector.
{
	return Vector3(-x, -y, -z);
}

Vector3 Vector3::Cross(const Vector3 &vA, const Vector3& vB) // vector cross product, returns new orthogonal vector
{
	return Vector3((vA.y*vB.z) - (vB.y*vA.z), -((vA.x*vB.z)-(vB.x*vA.z)), (vA.x*vB.y)-(vB.x*vA.y));
}

float Vector3::Dot(const Vector3 &vA, const Vector3 &vB)	// vector dot product, returns scalar (float)
{
	return (vA.x*vB.x) + (vA.y*vB.y) + (vA.z*vB.z);
}

Vector3 Vector3::Normalize(const Vector3 &v)  // returns normalised vector, (unit length = 1), same direction.
{
	return v / Length(v);
}

float Vector3::Length(const Vector3 &v)	// returns magnitude of vector sqrt of components squared
{
	return sqrt(LengthSq(v));
}

float Vector3::LengthSq(const Vector3 &v) // returns each component squared. (length of a vector squared) [sqrt(x^2 + y^2)]^2 = x^2 + y^2 etc.
{
	return v.x*v.x + v.y*v.y + v.z*v.z;
}
