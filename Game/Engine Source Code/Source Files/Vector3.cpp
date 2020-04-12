#include "Vector3.h"
#include <vector>

Vector3::Vector3(float X, float Y, float Z) : Point(X, Y, Z)
{
	
}

Vector3& Vector3::Equals(const Vector3& p)
{
	x = p.x;
	y = p.y;
	z = p.z;

	return *this;
}

Vector3& Vector3::Add(const Vector3& p)
{
	x += p.x;
	y += p.y;
	z += p.z;

	return *this;
}

Vector3& Vector3::Subtract(const Vector3& p)
{
	x -= p.x;
	y -= p.y;
	z -= p.z;

	return *this;
}

Vector3& Vector3::Multiply(float value)
{
	x *= value;
	y *= value;
	z *= value;

	return *this;
}

Vector3& Vector3::Divide(float value)
{
	x /= value;
	y /= value;
	z /= value;

	return *this;
}

Vector3& Vector3::operator=(const Vector3& p)
{
	return this->Equals(p);
}

Vector3& Vector3::operator+(const Vector3 &p)
{
	return this->Add(p);
}

Vector3& Vector3::operator+=(const Vector3& p)
{
	return this->Add(p);
}

Vector3& Vector3::operator-(const Vector3& p)
{
	return this->Subtract(p);
}

Vector3& Vector3::operator-=(const Vector3& p)
{
	return this->Subtract(p);
}

Vector3& Vector3::operator*(float multiplier)
{
	return this->Multiply(multiplier);
}

Vector3& Vector3::operator*=(float multiplier)
{
	return this->Multiply(multiplier);
}

Vector3& Vector3::operator/(float multiplier)
{
	return this->Divide(multiplier);
}

Vector3& Vector3::operator/=(float multiplier)
{
	return this->Divide(multiplier);
}

bool Vector3::operator==(const Vector3& p)
{
	float dist = (x + y + z) - (p.x + p.y + p.z);
	return dist < 0.05f;
}

bool Vector3::operator!=(const Vector3& p)
{
	return !(*this == p);
}

Vector3 Vector3::Zero()
{
	return Vector3(0, 0, 0);
}

Vector3 Vector3::Lerp(const Vector3& vecA, const Vector3& vecB, float time)
{
	float x = (vecA.x * (1.0f - time)) + (vecB.x * time);
	float y = (vecA.y * (1.0f - time)) + (vecB.y * time);
	float z = (vecA.z * (1.0f - time)) + (vecB.z * time);

	return Vector3(x, y, z);
}

Vector3 Vector3::Normalised()
{
	Vector3 vec;
	vec.x = x / Magnitude();
	vec.y = y / Magnitude();
	vec.z = z / Magnitude();

	return vec;
}

float Vector3::Magnitude()
{
	return sqrt((x * x) + (y * y) + (z * z));
}

std::string Vector3::ToString()
{
	std::string strX = std::to_string(x);
	std::string strY = std::to_string(y);
	std::string strZ = std::to_string(z);

	std::string str = '(' + strX + ',' + strY + ',' + strZ + ')';
	return str;
}

Vector3 Vector3::FromString(std::string str)
{
	Vector3 vec;

	vector<std::string> values;
	std::string currentVal;

	for (char& c : str)
	{
		if (c != '(')
		{
			if (c == ',' || c == ')')
			{
				values.push_back(currentVal);
				currentVal.clear();
			}
			else
			{
				currentVal.push_back(c);
			}
		}
	}

	vec.x = std::stof(values[0]);
	vec.y = std::stof(values[1]);
	vec.z = std::stof(values[2]);

	return vec;
}
