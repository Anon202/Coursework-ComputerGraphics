

// Minimal Vector3 class

#pragma once


class Vector3
{
public:
	float x, y, z;

	Vector3();
	Vector3(float x, float y, float z);
	Vector3(const Vector3& rhs);

	void	operator+= (const Vector3 &v);
	void	operator-= (const Vector3 &v);
	void	operator*= (const float    s);
	Vector3 operator/  (const float    s) const;
	Vector3 operator-  (const Vector3 &v) const;
	Vector3 operator+  (const Vector3 &v) const;
	Vector3 operator*  (const float    s) const;
	Vector3 operator-  ()				  const; // Unary minus

	static Vector3  Cross(const Vector3 &vA, const Vector3& vB);
	static float	Dot(const Vector3 &vA, const Vector3& vB);
	static Vector3  Normalize(const Vector3& v);
	static float	Length(const Vector3& v);
	static float	LengthSq(const Vector3& v);

}; // End Vector3(..)






