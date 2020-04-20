#include "..\Header Files\Matrix4x4.h"
#include "Vector3.h"
#include "EngineCore.h"

#include <memory>

Matrix4x4::Matrix4x4(float mat[4][4])
{
	matrix[0][0] = mat[0][0];
	matrix[1][0] = mat[1][0];
	matrix[2][0] = mat[2][0];
	matrix[3][0] = mat[3][0];

	matrix[0][1] = mat[0][1];
	matrix[1][1] = mat[1][1];
	matrix[2][1] = mat[2][1];
	matrix[3][1] = mat[3][1];

	matrix[0][2] = mat[0][2];
	matrix[1][2] = mat[1][2];
	matrix[2][2] = mat[2][2];
	matrix[3][2] = mat[3][2];

	matrix[0][3] = mat[0][3];
	matrix[1][3] = mat[1][3];
	matrix[2][3] = mat[2][3];
	matrix[3][3] = mat[3][3];
}

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

Matrix4x4 Matrix4x4::operator*(const Matrix4x4& mat)
{
	Matrix4x4 newMat;
	newMat.matrix[0][0] = matrix[0][0] * mat.matrix[0][0] + matrix[0][1] * mat.matrix[1][0] + matrix[0][2] * mat.matrix[2][0] + matrix[0][3] * mat.matrix[3][0];
	newMat.matrix[0][1] = matrix[0][0] * mat.matrix[0][1] + matrix[0][1] * mat.matrix[1][1] + matrix[0][2] * mat.matrix[2][1] + matrix[0][3] * mat.matrix[3][1];
	newMat.matrix[0][2] = matrix[0][0] * mat.matrix[0][2] + matrix[0][1] * mat.matrix[1][2] + matrix[0][2] * mat.matrix[2][2] + matrix[0][3] * mat.matrix[3][2];
	newMat.matrix[0][3] = matrix[0][0] * mat.matrix[0][3] + matrix[0][1] * mat.matrix[1][3] + matrix[0][2] * mat.matrix[2][3] + matrix[0][3] * mat.matrix[3][3];

	newMat.matrix[1][0] = matrix[1][0] * mat.matrix[0][0] + matrix[1][1] * mat.matrix[1][0] + matrix[1][2] * mat.matrix[2][0] + matrix[1][3] * mat.matrix[3][0];
	newMat.matrix[1][1] = matrix[1][0] * mat.matrix[0][1] + matrix[1][1] * mat.matrix[1][1] + matrix[1][2] * mat.matrix[2][1] + matrix[1][3] * mat.matrix[3][1];
	newMat.matrix[1][2] = matrix[1][0] * mat.matrix[0][2] + matrix[1][1] * mat.matrix[1][2] + matrix[1][2] * mat.matrix[2][2] + matrix[1][3] * mat.matrix[3][2];
	newMat.matrix[1][3] = matrix[1][0] * mat.matrix[0][3] + matrix[1][1] * mat.matrix[1][3] + matrix[1][2] * mat.matrix[2][3] + matrix[1][3] * mat.matrix[3][3];

	newMat.matrix[2][0] = matrix[2][0] * mat.matrix[0][0] + matrix[2][1] * mat.matrix[1][0] + matrix[2][2] * mat.matrix[2][0] + matrix[2][3] * mat.matrix[3][0];
	newMat.matrix[2][1] = matrix[2][0] * mat.matrix[0][1] + matrix[2][1] * mat.matrix[1][1] + matrix[2][2] * mat.matrix[2][1] + matrix[2][3] * mat.matrix[3][1];
	newMat.matrix[2][2] = matrix[2][0] * mat.matrix[0][2] + matrix[2][1] * mat.matrix[1][2] + matrix[2][2] * mat.matrix[2][2] + matrix[2][3] * mat.matrix[3][2];
	newMat.matrix[2][3] = matrix[2][0] * mat.matrix[0][3] + matrix[2][1] * mat.matrix[1][3] + matrix[2][2] * mat.matrix[2][3] + matrix[2][3] * mat.matrix[3][3];

	newMat.matrix[3][0] = matrix[3][0] * mat.matrix[0][0] + matrix[3][1] * mat.matrix[1][0] + matrix[3][2] * mat.matrix[2][0] + matrix[3][3] * mat.matrix[3][0];
	newMat.matrix[3][1] = matrix[3][0] * mat.matrix[0][1] + matrix[3][1] * mat.matrix[1][1] + matrix[3][2] * mat.matrix[2][1] + matrix[3][3] * mat.matrix[3][1];
	newMat.matrix[3][2] = matrix[3][0] * mat.matrix[0][2] + matrix[3][1] * mat.matrix[1][2] + matrix[3][2] * mat.matrix[2][2] + matrix[3][3] * mat.matrix[3][2];
	newMat.matrix[3][3] = matrix[3][0] * mat.matrix[0][3] + matrix[3][1] * mat.matrix[1][3] + matrix[3][2] * mat.matrix[2][3] + matrix[3][3] * mat.matrix[3][3];

	return newMat;
}

std::string Matrix4x4::ToString()
{
	std::string row0 = std::to_string(matrix[0][0]) + " " + std::to_string(matrix[0][1]) + " " + std::to_string(matrix[0][2]) + " " + std::to_string(matrix[0][3]) + "\n";
	std::string row1 = std::to_string(matrix[1][0]) + " " + std::to_string(matrix[1][1]) + " " + std::to_string(matrix[1][2]) + " " + std::to_string(matrix[1][3]) + "\n";
	std::string row2 = std::to_string(matrix[2][0]) + " " + std::to_string(matrix[2][1]) + " " + std::to_string(matrix[2][2]) + " " + std::to_string(matrix[2][3]) + "\n";
	std::string row3 = std::to_string(matrix[3][0]) + " " + std::to_string(matrix[3][1]) + " " + std::to_string(matrix[3][2]) + " " + std::to_string(matrix[3][3]) + "\n";

	return row0 + row1 + row2 + row3;
}
