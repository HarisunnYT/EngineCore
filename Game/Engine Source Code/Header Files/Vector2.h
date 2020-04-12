#ifndef VECTOR2_H
#define VECTOR2_H

#include "Point.h"

#include <iostream>
#include <string>

class Vector2
{
public:

	float x;
	float y;

	Vector2(float X = 0, float Y = 0);

	Vector2& Equals(const Vector2& p);
	Vector2& Add(const Vector2& p);
	Vector2& Subtract(const Vector2& p);
	Vector2& Multiply(float value);
	Vector2& Divide(float value);

	Vector2& operator =(const Vector2& p);

	Vector2 operator +(const Vector2& p);
	Vector2& operator +=(const Vector2& p);

	Vector2 operator -(const Vector2& p);
	Vector2& operator -=(const Vector2& p);

	Vector2& operator *(float multiplier);
	Vector2& operator *=(float multiplier);

	Vector2& operator /(float multiplier);
	Vector2& operator /=(float multiplier);

	static Vector2 Lerp(const Vector2& vecA, const Vector2& vecB, float time);
	static Vector2 Zero();

	Vector2 Normalised();

	float Magnitude();

	std::string ToString();
	static Vector2 FromString(std::string str);
};

ostream& operator <<(ostream& stream, Vector2& p);

#endif