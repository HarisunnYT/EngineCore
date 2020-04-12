#pragma once

#ifndef MUSHROOM_H
#define MUSHROOM_H

#include "EngineCore.h"

class Mushroom : public Component
{
public:

	void Spawn();
	void Update() override;

	void OnCollision(Hit* hit) override;

	std::string Parse() override;
	static bool TryParse(std::string value, Entity* entity);
	static std::string componentName;

	template<class Archive>
	void Serialize(Archive& archive)
	{
		archive(componentName);
	}

private:

	bool		spawning = false;
	bool		spawned = false;

	int			bumpAmount = -32;
	float		bumpDuration = 0.5f;
	float		timer = 0.0;

	float		speed = 90.0f;
	int			direction = 1;

	Vector3		fromPosition;
	Vector3		toPosition;

	Rigidbody* rigidbody;

};

#endif