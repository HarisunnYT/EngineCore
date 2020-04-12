#include "Particle.h"

std::string Particle::componentName = "particle";

Particle::Particle(const char* path, Vector2 sheetSize, Vector2 f, int s, int eType) : SpriteRenderer(path, sheetSize)
{
	frames = f;
	speed = s;

	endType = eType;
}

void Particle::Init()
{
	SpriteRenderer::Init();

	ticksOffset = SDL_GetTicks();
}

void Particle::Update()
{
	SpriteRenderer::Update();

	int totalOffset = static_cast<int>((SDL_GetTicks() - ticksOffset) / speed) % static_cast<int>(frames.x * frames.y);
	int xOffset = static_cast<int>((SDL_GetTicks() - ticksOffset) / speed) % static_cast<int>(frames.x);
	int yOffset = static_cast<int>(floor(totalOffset / frames.x));

	if (!initialPlay && endType != PARTICLE_LOOP && ((frames.y > 0 && yOffset == 0) || (frames.y == 0 && xOffset == 0)))
	{
		switch (endType)
		{
		case PARTICLE_DESTROY:
			entity->Destroy();
			break;
		case PARTICLE_HIDE:
			entity->SetActive(false);
			break;
		}

		return;
	}
	else if ((frames.y > 0 && yOffset > 0) || (frames.y == 0 && xOffset > 0))
	{
		initialPlay = false;
	}

	sourceRect.x = sourceRect.w * xOffset;
	sourceRect.y = yOffset * static_cast<int>(spriteSize.y);
}

void Particle::OnEnable()
{
	ticksOffset = SDL_GetTicks();
	initialPlay = true;
}

void Particle::SetSpeed(int s)
{
	speed = s;
}

std::string Particle::Parse()
{
	std::stringstream ss;
	{
		cereal::JSONOutputArchive oarchive(ss);
		oarchive(Particle::componentName, currentPath, frames.ToString(), spriteSize.ToString(), speed, endType);
	}

	return ss.str();
}

bool Particle::TryParse(std::string value, Entity* entity)
{
	std::string name;
	std::string inPath;
	std::string inSize;
	std::string inFrames;
	int inSpeed;
	int inEndType;

	std::stringstream ss(value);
	{
		cereal::JSONInputArchive oarchive(ss);
		oarchive(name, inPath, inFrames, inSize, inSpeed, inEndType);
	}

	if (name == Particle::componentName)
	{
		entity->AddComponent<Particle>(inPath.c_str(), Vector2::FromString(inSize), Vector2::FromString(inFrames), inSpeed, inEndType);

		return true;
	}

	return false;
}
