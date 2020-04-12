#include "Goomba.h"
#include "Animator.h"
#include "PlayerController.h"

std::string Goomba::componentName = "goomba";

void Goomba::Init()
{
	rigidbody = &entity->GetComponent<Rigidbody>();
}

void Goomba::Update()
{
	if (!dead)
	{
		rigidbody->SetVelocity(Vector2(direction * speed, rigidbody->GetVelocity().y));
	}
	else
	{
		timer = timer + (EngineCore::fixedTimeStep / 2.0f);
		float normTime = timer / deathDuration;

		if (normTime >= 1.0f)
		{
			entity->SetActive(false);
		}
	}
}

void Goomba::OnEnable()
{
	entity->GetComponent<Animator>().PlayAnimation(0);
}

void Goomba::OnCollision(Hit* hit)
{
	if (hit->collider->Tag == "player")
	{
		if (hit->normal.y < -0.8f)
		{
			dead = true;

			rigidbody->SetVelocity(Vector2(0, 0));
			rigidbody->useGravity = false;

			entity->GetComponent<Collider>().Trigger = true;
			entity->GetComponent<Animator>().PlayAnimation(1);

			Vector3 pos = entity->transform->GetPosition();
			entity->transform->SetPosition(Vector3(pos.x, pos.y + 10, pos.z));

			Vector2 playerVel = hit->collider->entity->GetComponent<Rigidbody>().GetVelocity();
			hit->collider->entity->GetComponent<Rigidbody>().SetVelocity(Vector2(playerVel.x, 100));

			entity->GetComponent<AudioSource>().Play(L"Assets/Audio/Stomp.wav");
		}
		else
		{
			hit->collider->entity->GetComponent<PlayerController>().TakeDamage();
		}
	}
	else if ((hit->normal.x > 0.8f && direction == 1) || (hit->normal.x < -0.8f && direction == -1))
	{
		direction *= -1;
	}
}

bool Goomba::IsDead()
{
	return dead;
}

std::string Goomba::Parse()
{
	std::stringstream ss;
	{
		cereal::JSONOutputArchive oarchive(ss);
		oarchive(Goomba::componentName);
	}

	return ss.str();
}

bool Goomba::TryParse(std::string value, Entity* entity)
{
	std::string name;

	std::stringstream ss(value);
	{
		cereal::JSONInputArchive oarchive(ss);
		oarchive(name);
	}

	if (name == Goomba::componentName)
	{
		entity->AddComponent<Goomba>();

		return true;
	}

	return false;
}