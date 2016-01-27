/* 
 *	Created by Zoe Wall
 *	Last modified 27/01/2016
 *	Matrix4.cpp - Implementation of the matrix4 header file */

#include "matrix4.h"

Matrix4::Matrix4() // empty Constructor. set to zero?
{ 
	this->m[0] = { 0 };
	//this->m[1] 
	/*
	for (int i = 0; i <= 16; ++i)
	{
		this->m[i] = 0;
	} */
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
/*
Matrix4 Matrix4::operator* (const Matrix4& lhs, const Matrix4& rhs)
{
	return Matrix4(); 
} */

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
	return Matrix4(1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1); // Matrix4(z[0], z[1], z[2], z[3], z[4], z[5], z[6], z[7], z[8], z[9], z[10]);
}

Matrix4 Matrix4::Transpose(const Matrix4& mat)
{
	Matrix4 T = mat; // diagonal is the same so set equal first.

	T[4] = mat[1];
	T[8] = mat[2];
	T[12] = mat[3];
	T[9] = mat[6];
	T[13] = mat[7];
	T[14] = mat[11];

	T[1] = mat[4];
	T[2] = mat[8];
	T[3] = mat[12];
	T[6] = mat[9];
	T[7] = mat[13];
	T[11] =  mat[14];
	  
	return T;
}

Matrix4 Matrix4::SetTranslation(const Vector3& translation)
{
	Matrix4 T = Matrix4::Identity();

	T[3] = translation.x;
	T[7] = translation.y;
	T[11] = translation.z;

	return Matrix4(T);
}


Vector3 Matrix4::GetTranslation(const Matrix4& mat)
{
	return Vector3(mat[3], mat[7], mat[11]);
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
static Matrix4 SetRotationAxis(const Vector3& axis, float angle)
{

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