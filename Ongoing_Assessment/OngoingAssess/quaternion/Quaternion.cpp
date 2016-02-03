/*
*	Created by Zoe Wall
*	Last modified 03/02/2016
*	Quaternion.cpp - Implementation of the quaternion header file */

#include "Quaternion.h"
#include <math.h>

Quaternion::Quaternion(float xx = 0, float yy = 0, float zz = 0, float ww = 1) : x(xx), y(yy), z(zz), w(ww)// float constructor
{

}

Quaternion::Quaternion(const Quaternion& rhs)
{
	this-> x = rhs.x;
	this-> y = rhs.y;
	this-> z = rhs.z;
	this-> w = rhs.w;
}

Quaternion Quaternion::Identity()  // no rotation
{
	return Quaternion(0, 0, 0, 1);
}

Quaternion Quaternion::Conjugate(const Quaternion& q)
{
	return Quaternion(-q.x, -q.y, -q.z, q.w);
}

float Quaternion::Length(const Quaternion& q)
{
	float len2 = LengthSq(q);
	return sqrt(len2);
}

float Quaternion::LengthSq(const Quaternion& q)
{
	float x2 = q.x*q.x;
	float y2 = q.y*q.y;
	float z2 = q.z*q.z;
	float w2 = q.w*q.w;

	return x2 + y2 + z2 + w2;
}

Quaternion Quaternion::Normalize(const Quaternion& q)
{
	float overLength = 1 / Length(q);
	Quaternion norm = q * overLength;
	return norm;
}

float Quaternion::Dot(const Quaternion& q1, const Quaternion& q2)
{
	float dot = q1.x*q2.x + q1.y*q2.y + q1.z*q2.z + q1.w*q2.w;
	return dot;
}

Quaternion Quaternion::FromAxisAngle(const Vector3& v, float angle)
{
	//return Quaternion sd;

	Quaternion rot;

	rot.x = v.x * sin(angle / 2);
	rot.y = v.y * sin(angle / 2);
	rot.z = v.z * sin(angle / 2);
	rot.w = cos(angle / 2);

	return rot;

}

float Quaternion::ToAxisAngle(const Quaternion& q, Vector3& v)
{
	float angle;

	return angle;
}

Matrix4 Quaternion::ToRotationMatrix(const Quaternion& q)
{
	Matrix4 mat = Matrix4::Identity();

	// memory locations
	// 0 4  8 12
	// 1 5  9 13
	// 2 6 10 14
	// 3 7 11 15

	mat[0] = 1 - 2 * (q.y*q.y + q.z*q.z);
	mat[5] = 1 - 2 * (q.x*q.x + q.z*q.z);
	mat[10] = 1 - 2 * (q.x*q.x + q.y*q.y);

	mat[1] = 2 * (q.x*q.y + q.z*q.w);
	mat[2] = 2 * (q.x*q.z - q.y*q.w);
	mat[6] = 2 * (q.y*q.z + q.x*q.w);

	mat[4] = 2 * (q.x*q.y - q.z*q.w);
	mat[8] = 2 * (q.x*q.z + q.y*q.w);
	mat[9]= 2 * (q.y*q.z - q.x*q.w);

	return mat;
}

Quaternion Quaternion::FromRotationMatrix(const Matrix4& m)
{
	Quaternion q;

	// memory locations
	// 0 4  8 12
	// 1 5  9 13
	// 2 6 10 14
	// 3 7 11 15

	/*mat[0] = 1 - 2 * (q.y*q.y + q.z*q.z);		m00
	mat[5] = 1 - 2 * (q.x*q.x + q.z*q.z);	    m11
	mat[10] = 1 - 2 * (q.x*q.x + q.y*q.y);		m22

	mat[1] = 2 * (q.x*q.y + q.z*q.w);			m10
	mat[2] = 2 * (q.x*q.z - q.y*q.w);			m20
	mat[6] = 2 * (q.y*q.z + q.x*q.w);			m21

	mat[4] = 2 * (q.x*q.y - q.z*q.w);			m01
	mat[8] = 2 * (q.x*q.z + q.y*q.w);			m02
	mat[9] = 2 * (q.y*q.z - q.x*q.w);			m12
	*/
	
	// w^2 = 1 - x^2 - y^2 - z^2
	// 4 * w^2 = 1 + (1 - 2*y^2 - 2*z^2) + (1 - 2*x^2 - 2*z^2) + (1 - 2*x^2 - 2*y^2)
	// 4 w^2 = 1 + m00 + m11 + m22
	q.w = 1 / 2 * sqrt(1 + m[0] + m[5] + m[10]);


	// 2*(yz + xw) - 2*(yz - xw) = 4xw
	// x = (m21 - m12)/(4* w)

	float denom = 1 / (4 * q.w);

	q.x = (m[6] - m[9])*denom;

	// 2*(xz + wy) - 2*(xz + wy) = 4wy
	// y = (m02 - m20)/(4 *w)
	q.y = (m[8] - m[2])*denom;

	// 2*(xy + wz) - 2*(xy + wz) = 4wz
	// y = (m10 - m01)/(4 *w)
	q.z = (m[1] - m[4])*denom;

	return q;
}

Quaternion operator*(const Quaternion& q, float s)
{
	Quaternion a;
	a.x = q.x * s;
	a.y = q.y * s;
	a.z = q.z * s;
	a.w = q.w * s;
	
	return a;
}

