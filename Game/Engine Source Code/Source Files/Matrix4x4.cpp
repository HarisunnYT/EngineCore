#include "..\Header Files\Matrix4x4.h"
#include "Vector3.h"
#include "EngineCore.h"

#include <memory>

void Matrix4x4::SetIdentity()
{
	memset(matrix, 0, sizeof(float) * 16);

	matrix[0][0] = 1;
	matrix[1][1] = 1;
	matrix[2][2] = 1;
	matrix[3][3] = 1;
}

void Matrix4x4::SetTranslation(const Vector3& translation)
{
	memset(matrix, 0, sizeof(float) * 16);

	matrix[3][0] = translation.x;
	matrix[3][1] = translation.y;
	matrix[3][2] = translation.z;
}

void Matrix4x4::SetScreenSpace(float nearPlane, float farPlane)
{
	SetIdentity();

	matrix[0][0] = 2.0f / EngineCore::screenSize.x;
	matrix[1][1] = 2.0f / EngineCore::screenSize.y;
	matrix[2][2] = 1.0f / (farPlane - nearPlane);
	matrix[3][3] = -(nearPlane / (farPlane - nearPlane));
}
