#include "matrix4.h"
#include <iostream>


int main()
{
	int enter = 0;

	Matrix4 a;
	Matrix4 b(0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15);
	Matrix4 c(b);
	
	std::cout << "Matrix A (empty constructor): \n";
	for (int i = 0; i < 4; ++i)
	{
		std::cout << a[i] << " " << a[i+4] << " " << a[i+8] << " " << a[i+12] << "\n";
	}
	std::cout << std::endl;

	std::cout << "Matrix B (float constructor - column major): \n";
	for (int i = 0; i < 4; ++i)
	{
		std::cout << b[i] << " " << b[i + 4] << " " << b[i + 8] << " " << b[i + 12] << "\n";
	}
	std::cout << std::endl;

	std::cout << "Matrix C (rhs constructor (passed in b): \n";
	for (int i = 0; i < 4; ++i)
	{
		std::cout << c[i] << " " << c[i + 4] << " " << c[i + 8] << " " << c[i + 12] << "\n";
	}
	std::cout << std::endl;

	Vector3 v(10, 5, 9);
	Matrix4 d = Matrix4::SetTranslation(v);

	std::cout << "Matrix Translation D: \n";
	for (int i = 0; i < 4; ++i)
	{
		std::cout << d[i] << " " << d[i + 4] << " " << d[i + 8] << " " << d[i + 12] << "\n";
	}
	std::cout << std::endl;


	Vector3 tran = Matrix4::GetTranslation(d);
	std::cout << "Get translation from Translation matrix D: \n"
		<< tran.x << " " << tran.y << " " << tran.z << "\n" << std::endl;

	Vector3 scale(2, 2, 2);
	Matrix4 e = Matrix4::SetScale(scale);

	std::cout << "Matrix E Scale Matrix (2, 2, 2): \n";
	for (int i = 0; i < 4; ++i)
	{
		std::cout << e[i] << " " << e[i + 4] << " " << e[i + 8] << " " << e[i + 12] << "\n";
	}
	std::cout << std::endl;

	Matrix4 id = Matrix4::Identity();
	std::cout << "Identity Matrix: \n";
	for (int i = 0; i < 4; ++i)
	{
		std::cout << id[i] << " " << id[i + 4] << " " << id[i + 8] << " " << id[i + 12] << "\n";
	}
	std::cout << std::endl;

	Matrix4 zero = Matrix4::Zero();
	std::cout << "Zero Matrix: \n";
	for (int i = 0; i < 4; ++i)
	{
		std::cout << zero[i] << " " << zero[i + 4] << " " << zero[i + 8] << " " << zero[i + 12] << "\n";
	}
	std::cout << std::endl;



	Matrix4 transpose = Matrix4::Transpose(b);
	std::cout << "Transpose of matrix b: \n";
	for (int i = 0; i < 4; ++i)
	{
		std::cout << transpose[i] << " " << transpose[i + 4] << " " << transpose[i + 8] << " " << transpose[i + 12] << "\n";
	}
	std::cout << std::endl;

	Vector3 axis(0.0f, 0.0f, 1.0f);
	Matrix4 rotation = Matrix4::SetRotationAxis(axis, 90);
	std::cout << "Rotation of 90 by z \n";
	for (int i = 0; i < 4; ++i)
	{
		std::cout << rotation[i] << " " << rotation[i + 4] << " " << rotation[i + 8] << " " << rotation[i + 12] << "\n";
	}
	std::cout << std::endl;

	Matrix4 direc = rotation * d;
	std::cout << "Matrix multiplication rotation * transformation: \n";
	for (int i = 0; i < 4; ++i)
	{
		std::cout << direc[i] << " " << direc[i + 4] << " " << direc[i + 8] << " " << direc[i + 12] << "\n";
	}
	std::cout << std::endl;


	Vector3 v1(5.0f, 18.0f, 3.0f);
	Vector3 transformed_point = Matrix4::TransformPoint(direc, v1);
	std::cout << "Transform point v by Translation matrix.\nBefore: "
		<< v1.x << " " << v1.y << " " << v1.z << "\n"
		<< "After: " << transformed_point.x << " " << transformed_point.y << " " << transformed_point.z << std::endl;


	Vector3 v2(0.0, 1.0, 3.0);

	
	Vector3 transformed_pt = Matrix4::TransformDirection(direc, v1);

	std::cout << "Transform v2 by Translation matrix.\nBefore: "
		<< v2.x << " " << v2.y << " " << v2.z << "\n"
		<< "After: " << transformed_pt.x << " " << transformed_pt.y << " " << transformed_pt.z << std::endl;

	Matrix4 matMul = b * c;
	std::cout << "Matrix multiplication B*C: \n";
	for (int i = 0; i < 4; ++i)
	{
		std::cout << matMul[i] << " " << matMul[i + 4] << " " << matMul[i + 8] << " " << matMul[i + 12] << "\n";
	}
	std::cout << std::endl;
	
	std::cin >> enter;

	return 0;
}