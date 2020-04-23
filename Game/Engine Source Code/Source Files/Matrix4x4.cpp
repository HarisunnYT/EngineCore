#include "..\Header Files\Matrix4x4.h"
#include "Vector3.h"
#include "EngineCore.h"

#include <memory>

Matrix4x4::Matrix4x4()
{
	for (int x = 0; x <= 3; x++)
		for (int y = 0; y <= 3; y++)
			matrix[y][x] = 0;
}

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
	Matrix4x4 newMat = Matrix4x4();

	int aX = 0, bX = 0, aY = 0, bY = 0;
	int newX = 0, newY = 0;

	while (true)
	{
		newMat.matrix[newY][newX] += matrix[aY][aX] * mat.matrix[bY][bX];

		if (aX == 3)
		{
			newX++;

			aX = 0;
			bY = 0;

			if (bX == 3)
			{
				if (bY == 3)
				{
					newX = 0;
					newY++;
				}

				bX = 0;
				bY = 0;

				aY++;
			}
			else
			{
				bX++;
			}

			if (aY == 4)
			{
				break;
			}
		}
		else
		{
			aX++;
			bY++;
		}
	}

	return newMat;
}

Matrix4x4& Matrix4x4::operator*=(const Matrix4x4& mat)
{
	*this = *this * mat;
	return *this;
}

Matrix4x4 Matrix4x4::operator+(const Matrix4x4& mat)
{
	Matrix4x4 newMat = Matrix4x4();
	for (int x = 0; x <= 3; x++)
	{
		for (int y = 0; y <= 3; y++)
		{
			newMat.matrix[y][x] = matrix[y][x] + mat.matrix[y][x];
		}
	}

	return newMat;
}

Matrix4x4& Matrix4x4::operator+=(const Matrix4x4& mat)
{
	*this = *this + mat;
	return *this;
}

Matrix4x4 Matrix4x4::operator-(const Matrix4x4& mat)
{
	Matrix4x4 newMat = Matrix4x4();
	for (int x = 0; x <= 3; x++)
	{
		for (int y = 0; y <= 3; y++)
		{
			newMat.matrix[y][x] = matrix[y][x] - mat.matrix[y][x];
		}
	}

	return newMat;
}

Matrix4x4& Matrix4x4::operator-=(const Matrix4x4& mat)
{
	*this = *this - mat;
	return *this;
}

Matrix4x4 Matrix4x4::Rotate(float angle, Vector3 direction)
{
	const float a = angle;
	const float c = cos(a);
	const float s = sin(a);

	direction.Normalise();

	Vector3 temp = Vector3((1 - c) * direction.x, (1 - c) * direction.y, (1 - c) * direction.z);
	Matrix4x4 rot = Matrix4x4();

	rot.matrix[0][0] = c + temp.x * direction.x;
	rot.matrix[0][1] = 0 + temp.x * direction.y + s * direction.z;
	rot.matrix[0][2] = 0 + temp.x * direction.z - s * direction.y;
	   										 
	rot.matrix[1][0] = 0 + temp.y * direction.x - s * direction.z;
	rot.matrix[1][1] = c + temp.y * direction.y;
	rot.matrix[1][2] = 0 + temp.y * direction.z + s * direction.x;
	   										 
	rot.matrix[2][0] = 0 + temp.z * direction.x + s * direction.y;
	rot.matrix[2][1] = 0 + temp.z * direction.y - s * direction.x;
	rot.matrix[2][2] = c + temp.z * direction.z;

	return rot;
}

Matrix4x4 Matrix4x4::LookAt(const Vector3& eye, const Vector3& center, const Vector3& up)
{
	const Vector3 f = ((Vector3)center - eye).Normalised();
	const Vector3 s = (Vector3::Cross(f, up)).Normalised();
	const Vector3 u = Vector3::Cross(s, f);

	Matrix4x4 result = Matrix4x4::One();
	result.matrix[0][0] = s.x;
	result.matrix[1][0] = s.y;
	result.matrix[2][0] = s.z;
	result.matrix[0][1] = u.x;
	result.matrix[1][1] = u.y;
	result.matrix[2][1] = u.z;
	result.matrix[0][2] = -f.x;
	result.matrix[1][2] = -f.y;
	result.matrix[2][2] = -f.z;
	result.matrix[3][0] = -Vector3::Dot(s, eye);
	result.matrix[3][1] = -Vector3::Dot(u, eye);
	result.matrix[3][2] = Vector3::Dot(f, eye);

	return result;
}

Matrix4x4 Matrix4x4::One()
{
	Matrix4x4 mat = Matrix4x4();
	for (int x = 0; x <= 3; x++)
		for (int y = 0; y <= 3; y++)
			mat.matrix[y][x] = 1;

	return mat;
}

std::string Matrix4x4::ToString()
{
	std::string row0 = std::to_string(matrix[0][0]) + " " + std::to_string(matrix[0][1]) + " " + std::to_string(matrix[0][2]) + " " + std::to_string(matrix[0][3]) + "\n";
	std::string row1 = std::to_string(matrix[1][0]) + " " + std::to_string(matrix[1][1]) + " " + std::to_string(matrix[1][2]) + " " + std::to_string(matrix[1][3]) + "\n";
	std::string row2 = std::to_string(matrix[2][0]) + " " + std::to_string(matrix[2][1]) + " " + std::to_string(matrix[2][2]) + " " + std::to_string(matrix[2][3]) + "\n";
	std::string row3 = std::to_string(matrix[3][0]) + " " + std::to_string(matrix[3][1]) + " " + std::to_string(matrix[3][2]) + " " + std::to_string(matrix[3][3]) + "\n";

	return row0 + row1 + row2 + row3;
}
