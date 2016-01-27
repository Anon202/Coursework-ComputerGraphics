


#pragma once


#include "vector3.h" // Vector3 class

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
*/

// Note column-major order also means the indices of 
// consecutive memory places are mapped onto a matrix like so
// [ 00 01 02 03 04 05 06 07 08 09 10 11 12 13 14 15 ]

class Matrix4
{
public:
	float m[16];

	Matrix4();
	Matrix4(const Matrix4& rhs);

	Matrix4(float _00, float _10, float _20, float _30,
		float _01, float _11, float _21, float _31,
		float _02, float _12, float _22, float _32,
		float _03, float _13, float _23, float _33);

	float&			operator[]	(int index);
	const float&	operator[]	(int index) const;


	static Matrix4 Zero();
	static Matrix4 Identity();
	static Matrix4 Transpose(const Matrix4& mat);

	static Matrix4 SetTranslation(const Vector3& translation);
	static Vector3 GetTranslation(const Matrix4& mat);

	static Matrix4 SetScale(const Vector3& scale);

	// ref: https://en.wikipedia.org/wiki/Rotation_matrix#Rotation_matrix_from_axis_and_angle
	static Matrix4 SetRotationAxis(const Vector3& axis, float angle);

	// ref: https://en.wikibooks.org/wiki/GLSL_Programming/Applying_Matrix_Transformations
	static Vector3 TransformPoint(const Matrix4& mat, const Vector3& p);
	static Vector3 TransformDirection(const Matrix4& mat, const Vector3& n);


}; // End Matrix4(..)



Matrix4 operator*(const Matrix4& lhs, const Matrix4& rhs);








