#pragma once

#ifndef MATRIX4X4_H
#define MATRIX4X4_H

class Vector3;
class Matrix4x4
{
public:

	void SetIdentity();
	void SetTranslation(const Vector3& translation);
	void SetScreenSpace(float nearPlane, float farPlane);

	float matrix[4][4];

};

#endif