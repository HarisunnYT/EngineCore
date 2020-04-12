#include "..\Header Files\Coin.h"

std::string Coin::componentName = "coin";

void Coin::Update()
{
	if (bumping)
	{
		timer = timer + EngineCore::deltaTime;
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
				entity->SetActive(false);

				return;
			}

			Vector3 l = Vector3::Lerp(toPosition, fromPosition, normTime);
			entity->transform->SetRawPosition(l);
		}
	}
}

void Coin::Bump()
{
	if (!bumping)
	{
		fromPosition = entity->transform->GetRawPosition();
		toPosition = entity->transform->GetRawPosition() + Vector3(0.0f, (float)bumpAmount, 0.0f);

		timer = 0.0f;
		bumping = true;

		entity->GetComponent<AudioSource>().Play(L"Assets/Audio/Coin.wav");
	}
}

std::string Coin::Parse()
{
	std::stringstream ss;
	{
		cereal::JSONOutputArchive oarchive(ss);
		oarchive(Coin::componentName);
	}

	return ss.str();
}

bool Coin::TryParse(std::string value, Entity* entity)
{
	int brickType = 0;
	std::string name;

	std::stringstream ss(value);
	{
		cereal::JSONInputArchive oarchive(ss);
		oarchive(name);
	}

	if (name == Coin::componentName)
	{
		entity->AddComponent<Coin>();

		return true;
	}

	return false;
}
