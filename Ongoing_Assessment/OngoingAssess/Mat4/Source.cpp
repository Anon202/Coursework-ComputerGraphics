#include "matrix4.h"
#include <iostream>

int main()
{
	int enter = 0;

	Matrix4 a();
	Matrix4 b(0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15);
	Matrix4 c(b);

	b[0] = 10;
	
	/*
	std::cout << "Matrix A: \n"
		<< a[0] << " " << b[1] << " " << b[2] << " " << b[3] << "\n"
		<< a[4] << " " << b[5] << " " << b[6] << " " << b[7] << "\n"
		<< a[8] << " " << b[9] << " " << b[10] << " " << b[11] << "\n"
		<< a[12] << " " << b[13] << " " << b[14] << " " << b[15] << "\n"
		<< std::endl; */


	std::cout << "Matrix B: \n"
		<< b[0] << " " << b[1] << " " << b[2] << " " << b[3] << "\n"
		<< b[4] << " " << b[5] << " " << b[6] << " " << b[7] << "\n"
		<< b[8] << " " << b[9] << " " << b[10] << " " << b[11] << "\n"
		<< b[12] << " " << b[13] << " " << b[14] << " " << b[15] << "\n"
		<< std::endl;

	std::cout << "Matrix C: \n"
		<< c[0] << " " << c[1] << " " << c[2] << " " << c[3] << "\n"
		<< c[4] << " " << c[5] << " " << c[6] << " " << c[7] << "\n"
		<< c[8] << " " << c[9] << " " << c[10] << " " << c[11] << "\n"
		<< c[12] << " " <<c[13] << " " << c[14] << " " << c[15] << "\n"
		<< std::endl;



	Vector3 v(10, 5, 9);
	Matrix4 d = Matrix4::SetTranslation(v);


	std::cout << "Matrix D: Translation Matrix by x = 10, y = 5, z = 9\n"
		<<d[0] << " " << d[1] << " " << d[2] << " " << d[3] << "\n"
		<<d[4] << " " << d[5] << " " << d[6] << " " << d[7] << "\n"
		<<d[8] << " " << d[9] << " " << d[10] << " " <<d[11] << "\n"
		<<d[12] << " " <<d[13] << " " <<d[14] << " " <<d[15] << "\n"
		<< std::endl;

	Vector3 tran = Matrix4::GetTranslation(d);
	std::cout << "Get translation from Translation matrix T: \n"
		<< tran.x << " " << tran.y << " " << tran.z << std::endl;

	Vector3 scale(2, 2, 2);
	Matrix4 e = Matrix4::SetScale(scale);


	std::cout << "Matrix C: \n"
		<< e[0] << " " << e[1] << " " << e[2] << " " << e[3] << "\n"
		<< e[4] << " " << e[5] << " " << e[6] << " " << e[7] << "\n"
		<< e[8] << " " << e[9] << " " << e[10] << " " <<e[11] << "\n"
		<< e[12] << " " << e[13] << " " << e[14] << " " << e[15] << "\n"
		<< std::endl;

	Vector3 transformed_point = Matrix4::TransformPoint(d, v);

	std::cout << "Transform point v by Translation matrix.\nBefore: "
		<< v.x << " " << v.y << " " << v.z << "\n"
		<< "After: " << transformed_point.x << " " << transformed_point.y << " " << transformed_point.z << std::endl;

	Vector3 transformed_pt = Matrix4::TransformDirection(d, v);

	std::cout << "Transform point v by Translation matrix.\nBefore: "
		<< v.x << " " << v.y << " " << v.z << "\n"
		<< "After: " << transformed_pt.x << " " << transformed_pt.y << " " << transformed_pt.z << std::endl;



	std::cin >> enter;

	return 0;
}