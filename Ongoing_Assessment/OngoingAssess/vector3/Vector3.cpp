#include "vector3.h"
#include <math.h>

Vector3::Vector3() : x(0), y(0), z(0)
{}

Vector3::Vector3(float x, float y, float z) : x(x), y(y), z(z)
{}

Vector3::Vector3(const Vector3& rhs) : x(rhs.x), y(rhs.y), z(rhs.z)
{}

void Vector3::operator+= (const Vector3 &v)
{
	this->x += v.x;
	this->y += v.y;
	this->z += v.z;
}

void Vector3::operator-= (const Vector3 &v)
{
	this->x -= v.x;
	this->y -= v.y;
	this->z -= v.z;
}

void Vector3::operator*= (const float s)
{
	this->x *= s;
	this->y *= s;
	this->z *= s;
}

Vector3 Vector3::operator/ (const float s) const
{
	return Vector3(x/s, y/s, z/s);						// divide components by s
}

Vector3 Vector3::operator- (const Vector3 &v) const     // subtract components
{
	return Vector3(x - v.x, y - v.y, z - v.z);
}

Vector3 Vector3::operator+  (const Vector3 &v) const
{
	return Vector3(x + v.x, y + v.y, z + v.z);
}

Vector3 Vector3::operator*  (const float s) const
{
	return Vector3(x*s, y*s, z*s);
}

Vector3 Vector3::operator- () const   
{
	return Vector3(-x, -y, -z);
}

Vector3 Vector3::Cross(const Vector3 &vA, const Vector3& vB)
{
	return Vector3((vA.y*vB.z) - (vB.y*vA.z), -((vA.x*vB.z)-(vB.x*vA.z)), (vA.x*vB.y)-(vB.x*vA.y));
}

float Vector3::Dot(const Vector3 &vA, const Vector3 &vB)
{
	return (vA.x*vB.x) + (vA.y*vB.y) + (vA.z*vB.z);
}

Vector3 Vector3::Normalize(const Vector3 &v)
{
	return v / Length(v);
}

float Vector3::Length(const Vector3 &v)
{
	return sqrt(LengthSq(v));
}

float Vector3::LengthSq(const Vector3 &v)
{
	return v.x*v.x + v.y*v.y + v.z*v.z;
}
