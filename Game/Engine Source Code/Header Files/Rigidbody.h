#pragma once

#ifndef RIGIDBODY_H
#define RIGIDBODY_H

#include "EngineCore.h"

class Transform;
class Collider;
class Hit;
class Rigidbody : public Component
{
public:

	Rigidbody() = default;
	Rigidbody(bool useGravity);

	void Init() override;
	void Physics() override;
	void LateUpdate() override;

	void SetVelocity(Vector2 velocity);
	Vector2 GetVelocity();

	float			mass = 1;
	bool			useGravity = true;

	std::string Parse() override;
	static bool TryParse(std::string value, Entity* entity);
	static std::string componentName;

	template<class Archive>
	void Serialize(Archive& archive)
	{
		archive(componentName, mass, useGravity);
	}

private:

	void ApplyGravity();

	Transform*		transform = nullptr;

	Vector2			velocity;
};

#endif