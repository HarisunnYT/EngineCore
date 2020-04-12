#include "Brick.h"
#include "Coin.h"
#include "Mushroom.h"

#include <time.h>

std::string Brick::componentName = "brick";

Brick::Brick(int type)
{
	brickType = type;
}

void Brick::Init()
{
	if (brickType == ITEMSPAWNER)
	{
		item = &EngineCore::Ecs->AddEntity("Assets/Prefabs/mushroom");
		item->SetActive(false);
	}
	else if (brickType == COINSPAWNER)
	{
		item = &EngineCore::Ecs->AddEntity("Assets/Prefabs/coin");
		item->SetActive(false);
	}
}

void Brick::Update()
{
	if (bumping)
	{
		timer = timer + EngineCore::deltaTime;;
		float normTime = timer / bumpDuration;

		if (normTime < 1.0f)
		{
			Vector3 l = Vector3::Lerp(fromPosition, toPosition, normTime);
			entity->transform->SetRawPosition(l);
		}
		else
		{
			normTime = normTime - 1.0f;

			if (normTime >= 1.0f)
			{
				bumping = false;
				return;
			}

			Vector3 l = Vector3::Lerp(toPosition, fromPosition, normTime);
			entity->transform->SetRawPosition(l);
		}
	}
}

void Brick::Bump()
{
	if (!bumping && canBump)
	{
		fromPosition = entity->transform->GetRawPosition();
		toPosition = entity->transform->GetRawPosition() + Vector3(0.0f, (float)bumpAmount, 0.0f);

		timer = 0.0f;
		bumping = true;

		if (brickType == ITEMSPAWNER)
		{
			item->transform->SetPosition(entity->transform->GetPosition() + Vector3(1, -15, -5));
			item->SetActive(true);
			item->GetComponent<Mushroom>().Spawn();

			entity->GetComponent<Tile>().SetSource(Vector2(96.0f, 32.0f));
			canBump = false;
		}
		else if (brickType == COINSPAWNER)
		{
			item->transform->SetPosition(entity->transform->GetPosition() + Vector3(-5, -60, 0));
			item->GetComponent<Coin>().Bump();
			item->SetActive(true);

			entity->GetComponent<Tile>().SetSource(Vector2(96.0f, 32.0f));
			canBump = false;
		}
		else
		{
			entity->GetComponent<AudioSource>().Play(L"Assets/Audio/Bump.wav");
		}
	}
	else if (!canBump)
	{
		entity->GetComponent<AudioSource>().Play(L"Assets/Audio/Bump.wav");
	}
}

std::string Brick::Parse()
{
	std::stringstream ss;
	{
		cereal::JSONOutputArchive oarchive(ss);
		oarchive(Brick::componentName);
	}

	return ss.str();
}

bool Brick::TryParse(std::string value, Entity* entity)
{
	int brickType;
	std::string name;

	std::stringstream ss(value);
	{
		cereal::JSONInputArchive oarchive(ss);
		oarchive(name, brickType);
	}

	if (name == Brick::componentName)
	{
		entity->AddComponent<Brick>(brickType);

		return true;
	}

	return false;
}
