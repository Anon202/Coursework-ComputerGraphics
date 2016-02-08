/*
*	Created by Zoe Wall
*	Last modified 03/02/2016
*	Quaternion.cpp - Implementation of the quaternion header file */

#include "Quaternion.h"
#include <math.h>

Quaternion::Quaternion(float xx, float yy, float zz, float ww) : x(xx), y(yy), z(zz), w(ww)// float constructor
{
}

Quaternion::Quaternion(const Quaternion& rhs) : x(rhs.x), y(rhs.y), z(rhs.z), w(rhs.w)   // rhs constructor
{
}

Quaternion& Quaternion::operator = (const Quaternion& rhs)
{
	this->x = rhs.x;
	this->y = rhs.y;
	this->z = rhs.z;
	this->w = rhs.w;

	return *this;
}

Quaternion Quaternion::Identity()  // no rotation
{
	Quaternion I; // use default constructor (0, 0, 0, 1)
	return I;
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
	Vector3 vNorm = Vector3::Normalize(v);

	Quaternion rot;

	rot.x = vNorm.x * sin(angle / 2);
	rot.y = vNorm.y * sin(angle / 2);
	rot.z = vNorm.z * sin(angle / 2);
	rot.w = cos(angle / 2);

	return rot;

}


float Quaternion::ToAxisAngle(const Quaternion& q, Vector3& v)
{
	float angle;
	
	Quaternion qNorm = Normalize(q);
	angle = 2 * acos(qNorm.w);
	float wSqrt = sqrt(1 - (qNorm.w*qNorm.w));

	if (wSqrt < 0.001) // avoids divide by zero
	{
		v.x = qNorm.x;
		v.y = qNorm.y;
		v.z = qNorm.z;
	}
	else
	{
		v.x = qNorm.x / wSqrt;
		v.y = qNorm.y / wSqrt;
		v.z = qNorm.z / wSqrt;
	}

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

	float trace = m[0] + m[5] + m[10];
	
	if (trace > 0)  // stops division by zero
	{
		// w^2 = 1 - x^2 - y^2 - z^2
		// 4 * w^2 = 1 + (1 - 2*y^2 - 2*z^2) + (1 - 2*x^2 - 2*z^2) + (1 - 2*x^2 - 2*y^2)
		// 4 w^2 = 1 + m00 + m11 + m22
		q.w = 1 / 2 * sqrt(trace + 1);

		float denom = 1 / (4 * q.w);

		// 2*(yz + xw) - 2*(yz - xw) = 4xw
		// x = (m21 - m12)/(4* w)
		q.x = (m[6] - m[9])*denom;

		// 2*(xz + wy) - 2*(xz + wy) = 4wy
		// y = (m02 - m20)/(4 *w)
		q.y = (m[8] - m[2])*denom;

		// 2*(xy + wz) - 2*(xy + wz) = 4wz
		// z = (m10 - m01)/(4 *w)
		q.z = (m[1] - m[4])*denom;

	}
	else if ((m[0] > m[5]) && (m[0] > m[10])) // if m[0] is the largest use X 
	{
		q.x = 1 / 2 * sqrt(1 + m[0] - m[5] - m[10]);
		float denom = 1 / (4 * q.x);

		q.w = (m[6] - m[9])*denom;
		q.y = (m[1] + m[4])*denom;
		q.z = (m[8] + m[2])*denom;

	}
	else if (m[5] > m[10]) // 5 bigger than 0 so if bigger than m[10] 5 is the largest so use y
	{
		q.y = 1 / 2 * sqrt(1 + m[5] - m[0] - m[10]);
		float denom = 1 / (4 * q.y);

		q.w = (m[8] - m[2]) * denom;
		q.x = (m[1] + m[4]) * denom;
		q.z = (m[6] + m[9]) * denom;

	}
	else // m10 is largest so use z
	{
		q.z = 1 / 2 * sqrt(1 + m[10] - m[0] - m[5]);

		float denom = 1 / (4 * q.z);

		q.w = (m[1] - m[4]) * denom;
		q.x = (m[8] + m[2]) * denom;
		q.y = (m[6] + m[9]) * denom;

	}

	return q;
}


Quaternion Quaternion::Slerp(float t, const Quaternion& p, const Quaternion& q)
{
	Quaternion result;
	Quaternion pNorm = Normalize(p);
	Quaternion qNorm = Normalize(q);
	float dotP = Dot(pNorm, qNorm);

	float halfAngle = acos(dotP) / 2;

	float denom = sin(halfAngle);

	if (denom = 0)
		result = p;
	else
		result = (pNorm*sin((1 - t)*halfAngle) + qNorm*sin(t*halfAngle) )* denom;


	return result;
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

Quaternion operator*(const Quaternion& q1, const Quaternion& q2)
{
	Quaternion result;

	Vector3 qV1(q1.x, q1.y, q1.z);
	Vector3 qV2(q2.x, q2.y, q2.z);

	float dot = Vector3::Dot(qV1, qV2);

	Vector3 cross = Vector3::Cross(qV1, qV2);

	result.w = (q1.w * q2.w) - dot;
	result.x = (q1.w * qV2.x) + (q2.w * qV1.x) + cross.x;
	result.y = (q1.w * qV2.y) + (q2.w * qV1.y) + cross.y;
	result.z = (q1.w * qV2.z) + (q2.w * qV1.z) + cross.z;

	return result;
}

Quaternion operator*(float s, const Quaternion& q)
{
	Quaternion a;
	a.x = s * q.x;
	a.y = s * q.y;
	a.z = s * q.z;
	a.w = s * q.w;

	return a;

}

Quaternion operator+(const Quaternion& q1, const Quaternion& q2)
{
	Quaternion result;

	result.w = q1.w + q2.w;
	result.x = q1.x + q2.x;
	result.y = q1.y + q2.y;
	result.z = q1.z + q2.z;


	return result;
}


