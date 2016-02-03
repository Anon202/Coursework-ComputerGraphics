



#pragma once


// Minimal Quaternion class


#include "vector3.h"
#include "matrix4.h"


class Quaternion
{
public:
	float x, y, z, w;

	Quaternion(float xx = 0, float yy = 0, float zz = 0, float ww = 1);
	Quaternion(const Quaternion& rhs);

	Quaternion& operator = (const Quaternion& rhs);

	// convenience function for the identity quaternion
	static Quaternion 	Identity();
	static Quaternion 	Conjugate(const Quaternion& q);

	static float 		Length(const Quaternion& q);
	static float 		LengthSq(const Quaternion& q);
	static Quaternion 	Normalize(const Quaternion& q);
	static float 		Dot(const Quaternion& q1, const Quaternion& q2);

	static Quaternion 	FromAxisAngle(const Vector3&    v, float angle);
	static float 	  	ToAxisAngle(const Quaternion& q, Vector3& v);

	static Matrix4 		ToRotationMatrix(const Quaternion& q);
	static Quaternion 	FromRotationMatrix(const Matrix4&    m);

	static Quaternion 	Slerp(float t, const Quaternion& p, const Quaternion& q);

}; // End Quaternion(...)



Quaternion operator*(const Quaternion& q1, const Quaternion& q2);

Quaternion operator*(const Quaternion& q, float s);

Quaternion operator*(float s, const Quaternion& q);

Quaternion operator+(const Quaternion& q1, const Quaternion& q2);




