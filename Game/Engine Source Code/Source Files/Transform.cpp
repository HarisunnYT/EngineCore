#include "Transform.h"

std::string Transform::componentName = "transform";

Transform::Transform()
{
	scale = Vector3(1, 1, 1);
}

Transform::Transform(Vector3 pos) : Transform()
{
	position = pos;
}

std::string Transform::Parse()
{
	std::stringstream ss;
	{
		cereal::JSONOutputArchive oarchive(ss);
		oarchive(Transform::componentName, position.ToString(), scale.ToString());
	}

	return ss.str();
}

bool Transform::TryParse(std::string value, Entity* entity)
{
	std::string name;
	std::string inPos;
	std::string inScale;

	std::stringstream ss(value);
	{
		cereal::JSONInputArchive oarchive(ss);
		oarchive(name, inPos, inScale);
	}

	if (name == Transform::componentName)
	{
		Vector3 vec = Vector3::FromString(inPos);
		entity->GetComponent<Transform>().position = vec;
		entity->GetComponent<Transform>().scale = Vector3::FromString(inScale);

		return true;
	}

	return false;
}
