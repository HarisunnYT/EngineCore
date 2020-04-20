#pragma once

#ifndef MATRIX4X4_H
#define MATRIX4X4_H

#include <string>

class Vector3;
class Matrix4x4
{
public:

	Matrix4x4();
	Matrix4x4(float mat[4][4]);

	void SetIdentity();
	void SetTranslation(const Vector3& translation);
	void SetScreenSpace(float nearPlane, float farPlane);

	Matrix4x4 operator*(const Matrix4x4& mat);
	Matrix4x4& operator*=(const Matrix4x4& mat);

	Matrix4x4 operator+(const Matrix4x4& mat);
	Matrix4x4& operator+=(const Matrix4x4& mat);

	Matrix4x4 operator-(const Matrix4x4& mat);
	Matrix4x4& operator-=(const Matrix4x4& mat);

	static Matrix4x4 Rotate(float angle, Vector3 direction);
	static Matrix4x4 One();

	std::string ToString();

	float matrix[4][4];

};

#endif