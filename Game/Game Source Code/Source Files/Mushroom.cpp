#include "Mushroom.h"
#include "PlayerController.h"

std::string Mushroom::componentName = "mushroom";

void Mushroom::Spawn()
{
	fromPosition = entity->transform->GetRawPosition();
	toPosition = entity->transform->GetRawPosition() + Vector3(0.0f, (float)bumpAmount, 0.0f);

	rigidbody = &entity->GetComponent<Rigidbody>();

	timer = 0;
	spawning = true;
	spawned = false;

	entity->GetComponent<AudioSource>().Play(L"Assets/Audio/PowerUpAppears.wav");
}

void Mushroom::Update()
{
	if (spawning)
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
			spawning = false;
		}
	}
	else if (!spawned)
	{
		entity->GetComponent<Collider>().Trigger = false;
		rigidbody->useGravity = true;

		spawned = true;
	}
	else
	{
		rigidbody->SetVelocity(Vector2(direction * speed, rigidbody->GetVelocity().y));
	}
}

void Mushroom::OnCollision(Hit* hit)
{
	if ((hit->normal.x > 0.8f && direction == 1) || (hit->normal.x < -0.8f && direction == -1))
	{
		direction *= -1;
	}

	if (hit->collider->Tag == "player")
	{
		hit->collider->entity->GetComponent<PlayerController>().SetBig();
		entity->SetActive(false);

		entity->GetComponent<AudioSource>().Play(L"Assets/Audio/Upgrade.wav");
	}
}

std::string Mushroom::Parse()
{
	std::stringstream ss;
	{
		cereal::JSONOutputArchive oarchive(ss);
		oarchive(Mushroom::componentName);
	}

	return ss.str();
}

bool Mushroom::TryParse(std::string value, Entity* entity)
{
	std::string name;

	std::stringstream ss(value);
	{
		cereal::JSONInputArchive oarchive(ss);
		oarchive(name);
	}

	if (name == Mushroom::componentName)
	{
		entity->AddComponent<Mushroom>();

		return true;
	}

	return false;
}