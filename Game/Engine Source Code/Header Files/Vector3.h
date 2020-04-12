#ifndef VECTOR3_H
#define VECTOR3_H

#include "Point.h"

#include <iostream>
#include <string>

class Vector3 : public Point
{
public:

	Vector3(float X = 0, float Y = 0, float Z = 0);

	Vector3& Equals(const Vector3& p);
	Vector3& Add(const Vector3& p);
	Vector3& Subtract(const Vector3& p);
	Vector3& Multiply(float value);
	Vector3& Divide(float value);

	Vector3& operator =(const Vector3& p);

	Vector3& operator +(const Vector3 &p);
	Vector3& operator +=(const Vector3& p);

	Vector3& operator -(const Vector3& p);
	Vector3& operator -=(const Vector3& p);

	Vector3& operator *(float multiplier);
	Vector3& operator *=(float multiplier);

	Vector3& operator /(float multiplier);
	Vector3& operator /=(float multiplier);

	bool operator ==(const Vector3& p);
	bool operator !=(const Vector3& p);

	static Vector3 Zero();
	static Vector3 Lerp(const Vector3& vecA, const Vector3& vecB, float time);

	Vector3 Normalised();

	float Magnitude();

	std::string ToString();
	static Vector3 FromString(std::string str);
};

#endif