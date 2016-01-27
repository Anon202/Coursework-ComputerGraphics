#include "matrix4.h"
#include <iostream>

int main()
{
	int enter = 0;

	Matrix4 a();
	Matrix4 b(0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15);
	Matrix4 c(b);

	std::cout << "Matrix A: \n"
		<< b[0] << " " << b[1] << " " << b[2] << " " << b[3] << "\n"
		<< b[4] << " " << b[5] << " " << b[6] << " " << b[7] << "\n"
		<< b[8] << " " << b[9] << " " << b[10] << " " << b[11] << "\n"
		<< b[12] << " " << b[13] << " " << b[14] << " " << b[15] << "\n"
				
		<< std::endl;


	std::cin >> enter;

	return 0;
}