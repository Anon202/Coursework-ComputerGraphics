/* 
 *	Created by Zoe Wall
 *	Last modified 28/01/2016
 *	Matrix4.cpp - Implementation of the matrix4 header file */

#include "matrix4.h"
#include <math.h>

Matrix4::Matrix4() // empty Constructor. set to zero?
{ 
	for (int i = 0; i <= 16; ++i)
	{
		this->m[i] = 0.0f;
	} 
}

Matrix4::Matrix4(const Matrix4& rhs)  // rhs constructor, 
{
	for (int i = 0; i < 16; ++i)
	{
		this->m[i] = rhs[i];
	}
}

Matrix4::Matrix4(float _00, float _10, float _20, float _30,
				 float _01, float _11, float _21, float _31,
				 float _02, float _12, float _22, float _32,
				 float _03, float _13, float _23, float _33)
{
	this->m[0] = _00;
	this->m[1] = _10;
	this->m[2] = _20;
	this->m[3] = _30;
	this->m[4] = _01;
	this->m[5] = _11;
	this->m[6] = _21;
	this->m[7] = _31;
	this->m[8] = _02;
	this->m[9] = _12;
	this->m[10] = _22;
	this->m[11] = _32;
	this->m[12] = _03;
	this->m[13] = _13;
	this->m[14] = _23;
	this->m[15] = _33;

}

Matrix4 operator* (const Matrix4& lhs, const Matrix4& rhs)
{
	float lhsRow1[4] = { lhs[0], lhs[4], lhs[8], lhs[12] };
	float lhsRow2[4] = { lhs[1], lhs[5], lhs[9], lhs[13] };
	float lhsRow3[4] = { lhs[2], lhs[6], lhs[10], lhs[14] };
	float lhsRow4[4] = { lhs[3], lhs[7], lhs[11], lhs[15] };

	float rhsCol1[4] = { rhs[0], rhs[1], rhs[2], rhs[3] };
	float rhsCol2[4] = { rhs[4], rhs[5], rhs[6], rhs[7] };
	float rhsCol3[4] = { rhs[8], rhs[9], rhs[10], rhs[11] };
	float rhsCol4[4] = { rhs[12], rhs[13], rhs[14], rhs[15] };

	float m0 = 0.0f;
	for (int i = 0; i < 4; ++i)
		m0+= lhsRow1[i] * rhsCol1[i];

	float m1 = 0.0f;
	for (int i = 0; i < 4; ++i)
		m1 += lhsRow2[i] * rhsCol1[i];

	float m2 = 0.0f;
	for (int i = 0; i < 4; ++i)
		m2 += lhsRow3[i] * rhsCol1[i];

	float m3 = 0.0f;
	for (int i = 0; i < 4; ++i)
		m3 += lhsRow4[i] * rhsCol1[i];

	float m4 = 0.0f;
	for (int i = 0; i < 4; ++i)
		m4 += lhsRow1[i] * rhsCol2[i];

	float m5 = 0.0f;
	for (int i = 0; i < 4; ++i)
		m5 += lhsRow2[i] * rhsCol2[i];

	float m6 = 0.0f;
	for (int i = 0; i < 4; ++i)
		m6 += lhsRow3[i] * rhsCol2[i];
	
	float m7 = 0.0f;
	for (int i = 0; i < 4; ++i)
		m7 += lhsRow4[i] * rhsCol2[i];

	float m8 = 0.0f;
	for (int i = 0; i < 4; ++i)
		m8 += lhsRow1[i] * rhsCol3[i];

	float m9 = 0.0f;
	for (int i = 0; i < 4; ++i)
		m9 += lhsRow2[i] * rhsCol3[i];

	float m10 = 0.0f;
	for (int i = 0; i < 4; ++i)
		m10 += lhsRow3[i] * rhsCol3[i];

	float m11 = 0.0f;
	for (int i = 0; i < 4; ++i)
		m11 += lhsRow4[i] * rhsCol3[i];

	float m12 = 0.0f;
	for (int i = 0; i < 4; ++i)
		m12 += lhsRow1[i] * rhsCol4[i];

	float m13 = 0.0f;
	for (int i = 0; i < 4; ++i)
		m13 += lhsRow2[i] * rhsCol4[i];

	float m14 = 0.0f;
	for (int i = 0; i < 4; ++i)
		m14 += lhsRow3[i] * rhsCol4[i];

	float m15 = 0.0f;
	for (int i = 0; i < 4; ++i)
		m15 += lhsRow4[i] * rhsCol4[i];

	Matrix4 result(m0, m1, m2, m3, m4, m5, m6, m7, m8, m9, m10, m11, m12, m13, m14, m15);

	return result; 
}

float& Matrix4::operator[] (int index)
{
	if (index > 16)
	{
		//std::cout << "Index out of bounds" << std::endl;
		return m[0];
	}

	return m[index];
}

const float& Matrix4::operator[] (int index) const
{
	if (index > 16)
	{
		//std::cout << "Index out of bounds" << std::endl;
		return m[0];
	}

	return m[index];
}

Matrix4 Matrix4::Zero()
{
	return Matrix4();			// zero matrix return empty constructor matrix (all elements set to zero)
}

Matrix4 Matrix4::Identity()
{
	return Matrix4(1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1); 
}

Matrix4 Matrix4::Transpose(const Matrix4& mat)
{
	Matrix4 T = mat; // diagonal is the same so set equal first.


	  
	return T;
}

Matrix4 Matrix4::SetTranslation(const Vector3& translation)
{
	Matrix4 T = Matrix4::Identity();

	T[12] = translation.x;
	T[13] = translation.y;
	T[14] = translation.z;

	return Matrix4(T);
}


Vector3 Matrix4::GetTranslation(const Matrix4& mat)
{

	return Vector3(mat[12], mat[13], mat[14]);
}

Matrix4 Matrix4::SetScale(const Vector3& scale)
{
	Matrix4 S = Matrix4::Identity();

	S[0] = scale.x;
	S[5] = scale.y;
	S[10] = scale.z;

	return Matrix4(S);
}


// ref: https://en.wikipedia.org/wiki/Rotation_matrix#Rotation_matrix_from_axis_and_angle
Matrix4 Matrix4::SetRotationAxis(const Vector3& axis, float angle)
{
	Vector3 unitAxis = Vector3::Normalize(axis);  // normalise the axis vector

	float cosTheta = cos(angle);
	float negCos = 1 - cosTheta;
	float sinTheta = sin(angle);


	float m00 = cosTheta + unitAxis.x*unitAxis.x*negCos;
	float m01 = unitAxis.x*unitAxis.y*negCos - unitAxis.z*sinTheta;
	float m02 = unitAxis.x*unitAxis.z*negCos + unitAxis.y*sinTheta;
	float m10 = unitAxis.y*unitAxis.x*negCos + unitAxis.z*sinTheta;
	float m11 = cosTheta + unitAxis.y*unitAxis.y*negCos;
	float m12 = unitAxis.y*unitAxis.z*negCos - unitAxis.x*sinTheta;
	float m20 = unitAxis.z*unitAxis.x*negCos - unitAxis.y*sinTheta;
	float m21 = unitAxis.z*unitAxis.y*negCos + unitAxis.x*sinTheta;
	float m22 = cosTheta + unitAxis.z*unitAxis.z*negCos;

	return Matrix4(m00, m10, m20, 0.0f, m01, m11, m21, 0.0f, m02, m12, m22, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f);

}

Vector3 Matrix4::TransformPoint(const Matrix4& mat, const Vector3& p)
{
	float x1 = p.x*mat[0] + p.y*mat[1] + p.z*mat[2] + mat[3];
	float y1 = p.x*mat[4] + p.y*mat[5] + p.z*mat[6] + mat[7];
	float z1 = p.x*mat[8] + p.y*mat[9] + p.z*mat[10] + mat[11];

	return Vector3(x1, y1, z1);
}


Vector3 Matrix4::TransformDirection(const Matrix4& mat, const Vector3& n)
{
	float x1 = n.x*mat[0] + n.y*mat[1] + n.z*mat[2];
	float y1 = n.x*mat[4] + n.y*mat[5] + n.z*mat[6];
	float z1 = n.x*mat[8] + n.y*mat[9] + n.z*mat[10];

	return Vector3(x1, y1, z1);
}