#include "Rigidbody.h"

std::string Rigidbody::componentName = "rigidbody";

Hit hit;

Rigidbody::Rigidbody(bool gravity)
{
	useGravity = gravity;
}

void Rigidbody::Init()
{
	transform = &entity->GetComponent<Transform>();
}

void Rigidbody::Physics()
{
	if (!useGravity)
		return;

	if (entity->HasComponent<Collider>() && !entity->GetComponent<Collider>().Trigger)
	{
		if (Collision::CheckCollision(&entity->GetComponent<Collider>(), hit) && hit.collider != nullptr)
		{
			int xRounded = abs(hit.normal.x) > 0.8f ? (hit.normal.x > 0 ? 1 : -1) : 0;
			int yRounded = abs(hit.normal.y) > 0.8f ? (hit.normal.y > 0 ? 1 : -1) : 0;
			if ((yRounded == 1 && velocity.y < 0))
			{
				velocity = Vector2(velocity.x, 0);
			}
			else if ((xRounded == 1 && velocity.x > 0) || (xRounded == -1 && velocity.x < 0))
			{
				velocity = Vector2(0, velocity.y);
			}

			if (yRounded == -1 && velocity.y > 0)
			{
				velocity.y *= -0.25;
			}

			if (yRounded == 0)
			{
				ApplyGravity();
			}
		}
		else
		{
			ApplyGravity();
		}
	}
	else
	{
		ApplyGravity();
	}
}

void Rigidbody::LateUpdate()
{
	transform->SetRawPosition(transform->GetRawPosition() + (Vector3(velocity.x, -velocity.y, 0) * EngineCore::deltaTime));

	if (entity->HasComponent<Collider>())
	{
		Collision::UpdateGrid(&entity->GetComponent<Collider>());
	}
}

void Rigidbody::SetVelocity(Vector2 v)
{
	velocity = v;
}

Vector2 Rigidbody::GetVelocity()
{
	return velocity;
}

void Rigidbody::ApplyGravity()
{
	if (useGravity)
	{
		velocity.y = velocity.y + Physics::gravity.y * mass;
	}
}

std::string Rigidbody::Parse()
{
	std::stringstream ss;
	{
		cereal::JSONOutputArchive oarchive(ss);
		oarchive(Rigidbody::componentName, mass, useGravity);
	}

	return ss.str();
}

bool Rigidbody::TryParse(std::string value, Entity* entity)
{
	std::string name;
	float inMass;
	bool inUseGravity;

	std::stringstream ss(value);
	{
		cereal::JSONInputArchive oarchive(ss);
		oarchive(name, inMass, inUseGravity);
	}

	if (name == Rigidbody::componentName)
	{
		entity->AddComponent<Rigidbody>(inUseGravity);
		entity->GetComponent<Rigidbody>().mass = inMass;

		return true;
	}

	return false;
}



