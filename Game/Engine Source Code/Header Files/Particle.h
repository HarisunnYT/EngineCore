#pragma once

#ifndef PARTICLE_H
#define PARTICLE_H

#include "EngineCore.h"

enum 
{
	PARTICLE_LOOP,
	PARTICLE_HIDE,
	PARTICLE_DESTROY
};

class Particle : public SpriteRenderer
{
public:

	Particle(const char* path, Vector2 sheetSize, Vector2 frames, int speed, int endType);

	void Init() override;
	void Update() override;

	void OnEnable() override;

	void SetSpeed(int speed);

	std::string Parse() override;
	static bool TryParse(std::string value, Entity* entity);
	static std::string componentName;

	template<class Archive>
	void Serialize(Archive& archive)
	{
		archive(componentName, currentPath, frames.ToString(), spriteSize.ToString(), speed, endType);
	}

private:

	Vector2			frames;
	int				speed = 100;

	int				ticksOffset;
	int				endType;

	bool			initialPlay = true;
};

#endif