#pragma once

#ifndef TRANSFORM_H
#define TRANSFORM_H

#include "EngineCore.h"

class Transform : public Component
{
public:

	Transform();
	Transform(Vector3 pos);

	Vector2 scale;
	Vector3	position;

	std::string Parse() override;
	static bool TryParse(std::string value, Entity* entity);
	static std::string componentName;
	
	template<class Archive>
	void Serialize(Archive& archive)
	{
		archive(componentName, position.ToString(), scale.ToString());
	}
};

#endif