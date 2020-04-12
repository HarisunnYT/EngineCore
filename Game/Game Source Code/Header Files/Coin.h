#pragma once

#ifndef COIN_H
#define COIN_H

#include "EngineCore.h"

class Coin : public Component
{

public:

	void Update() override;
	void Bump();

	std::string Parse() override;
	static bool TryParse(std::string value, Entity* entity);
	static std::string componentName;

	template<class Archive>
	void Serialize(Archive& archive)
	{
		archive(componentName);
	}

private:

	bool		bumping = false;
	int			bumpAmount = -130;
	float		bumpDuration = 0.15f;
	float		timer = 0.0;

	Vector3		fromPosition;
	Vector3		toPosition;
};

#endif