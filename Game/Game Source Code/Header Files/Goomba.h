#pragma once

#ifndef GOOMBA_H
#define GOOMBA_H

#include "EngineCore.h"

class Rigidbody;
class Goomba : public Component
{
public:

	void Init() override;
	void Update() override;
	void OnEnable() override;

	void OnCollision(Hit* hit) override;

	bool IsDead();

	std::string Parse() override;
	static bool TryParse(std::string value, Entity* entity);
	static std::string componentName;

	template<class Archive>
	void Serialize(Archive& archive)
	{
		archive(componentName);
	}

private:

	float		speed = 60.0f;
	int			direction = -1;

	float		deathDuration = 0.25f;
	float		timer = 0.0;

	bool		dead = false;

	Rigidbody* rigidbody;

};

#endif