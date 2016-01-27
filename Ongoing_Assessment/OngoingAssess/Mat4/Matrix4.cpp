/* 
 *	Created by Zoe Wall
 *	Last modified 27/01/2016
 *	Matrix4.cpp - Implementation of the matrix4 header file */

#include "matrix4.h"

Matrix4::Matrix4() // empty Constructor. set to zero?
{
	for (int i = 0; i < 16; ++i)
	{
		this->m[i] = 0;
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
	this->m[1] = _01;
	this->m[2] = _02;
	this->m[3] = _03;
	this->m[4] = _10;
	this->m[5] = _11;
	this->m[6] = _12;
	this->m[7] = _13;
	this->m[8] = _20;
	this->m[9] = _21;
	this->m[10] = _22;
	this->m[11] = _23;
	this->m[12] = _30;
	this->m[13] = _31;
	this->m[14] = _32;
	this->m[15] = _33;

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
	return Matrix4();
}

Matrix4 Matrix4::Identity()
{
	float z[16];

	for (int i = 0; i < 16; ++i)
	{
		z[i] = 0;
	}

	z[0] = 1;
	z[5] = 1;
	z[10] = 1;
	z[15] = 1;

	return Matrix4(1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1); // Matrix4(z[0], z[1], z[2], z[3], z[4], z[5], z[6], z[7], z[8], z[9], z[10]);
}

Matrix4 Matrix4::Transpose(const Matrix4& mat)
{
	return Matrix4();
}

static Matrix4 SetTranslation(const Vector3& translation) {}
static Vector3 GetTranslation(const Matrix4& mat) {}

static Matrix4 SetScale(const Vector3& scale);

// ref: https://en.wikipedia.org/wiki/Rotation_matrix#Rotation_matrix_from_axis_and_angle
static Matrix4 SetRotationAxis(const Vector3& axis, float angle);

// ref: https://en.wikibooks.org/wiki/GLSL_Programming/Applying_Matrix_Transformations
static Vector3 TransformPoint(const Matrix4& mat, const Vector3& p);
static Vector3 TransformDirection(const Matrix4& mat, const Vector3& n);

/*
// Minimal Matrix4 class

/*
* Column-major 4x4 matrix
*
* Layout:
*			0  4  8  12
*			1  5  9  13
*			2  6  10 14
*			3  7  11 15
*
*  3x3 Rotation Matrix Indices
*			0  4  8
*			1  5  9
*			2  6  10
*
*  3x1 Translation Indices
*			12
*			13
*			14
*

// Note column-major order also means the indices of 
// consecutive memory places are mapped onto a matrix like so
// [ 00 01 02 03 04 05 06 07 08 09 10 11 12 13 14 15 ]

class Matrix4
{
public:
	float m[16];

	Matrix4();
	Matrix4(const Matrix4& rhs);

	


}; // End Matrix4(..)

Matrix4 operator*(const Matrix4& lhs, const Matrix4& rhs);
*/