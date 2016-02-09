// source to test quaternion

#include "Quaternion.h"
#include <iostream>

using namespace std;

int main()
{

//	const float pi = atan(1) * 4;
	int enter;

	// test default constructor
	Quaternion a;

	Quaternion b(1, 2, 3);

	Quaternion c(4, 5, 6, 2);

	Quaternion i = Quaternion::Identity();

	Quaternion conj_b = Quaternion::Conjugate(b);

	float len_b = Quaternion::Length(b);
	float lenSq_b = Quaternion::LengthSq(b);

	Quaternion norm_b = Quaternion::Normalize(b);

	float dot_bc = Quaternion::Dot(b, c);

	// from axis pi round x
	Vector3 zyAxis(0, 1, 1);
	float theta = 3.14159267f/2;
	Quaternion fromAxis = Quaternion::FromAxisAngle(zyAxis, theta);

	Vector3 vec;
	float angle = Quaternion::ToAxisAngle(fromAxis, vec);

	cout << "Default constructor (empty): "
		<< a.x << " " << a.y << " " << a.z << " " << a.w << endl;

	cout << "Default change values: "
		<< b.x << " " << b.y << " " << b.z << " " << b.w << endl;

	cout << "Identity quat: "
		<< i.x << " " << i.y << " " << i.z << " " << i.w << endl;

	cout << "conjugate of b quat: "
		<< conj_b.x << " " << conj_b.y << " " << conj_b.z << " " << conj_b.w << endl;

	cout << "Length of quat b: "
		<< len_b << endl;

	cout << "Length squared of quat b: "
		<< lenSq_b << endl;

	cout << "Normalised b: "
		<< norm_b.x << " " << norm_b.y << " " << norm_b.z << " " << norm_b.w << endl;

	cout << "Dot b, c: "
		<< dot_bc << endl;

	cout << "From axis angle pi round zy-axis: "
		<< fromAxis.x << " " << fromAxis.y << " " << fromAxis.z << " " << fromAxis.w << endl;

	cout << "To axis angle from quat: "
		<< vec.x << " " << vec.y << " " << vec.z << "\nAngle: " << angle << endl;

	Vector3 norm = Vector3::Normalize(zyAxis);

	cout << "Answer should be: "
		<< norm.x << " " << norm.y << " " << norm.z << "\nAngle: " << theta << "\n\n" << endl;

	
	Matrix4 mat = Quaternion::ToRotationMatrix(Quaternion(1, 0, 0, 0));

	cout << "To matrix, x by pi: \n";
	for (int i = 0; i < 4; ++i)
	{
		std::cout << mat[i] << " " << mat[i + 4] << " " << mat[i + 8] << " " << mat[i + 12] << "\n";
	}
	std::cout << std::endl;

	Quaternion fromMat = Quaternion::FromRotationMatrix(mat);
	cout << "From Matrix: "
		<< fromMat.x << " " << fromMat.y << " " << fromMat.z << endl;



	Quaternion v;
	Quaternion f(1, 2, 3);

	Quaternion g = v + f;
	cout << "Additon :"
		<< g.x << " " << g.y << " " << g.z << " " << g.w << endl;


	Quaternion m = c * f;

	cout << "Multiplication :"
		<< m.x << " " << m.y << " " << m.z << " " << m.w << endl;

	


	Quaternion slerpA(0.0f, 0.0f, 0.7071f, 0.7071f);
	Quaternion slerpB;

	Quaternion slerpC = Quaternion::Slerp(0.33f, slerpA, slerpB);

	cout << "Slerp:"
		<< slerpC.x << " " << slerpC.y << " " << slerpC.z << " " << slerpC.w << endl;
	cin >> enter;

	return 0;
}