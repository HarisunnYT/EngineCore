#include "Entity.h"
#include "Component.h"

#include <iostream>
#include <fstream>

Entity::Entity()
{
	transform = &AddComponent<Transform>();

	OnEnable();
}

Entity::~Entity()
{
	//for (auto& c : components)
	//{
	//	delete *c;
	//}
}

void Entity::Update()
{
	for (auto& c : components)
	{
		c->Update();
	}
}

void Entity::Draw()
{
	for (auto& c : components)
	{
		c->Draw();
	}
}

void Entity::Destroy()
{
	delete this;
}

void Entity::Physics()
{
	for (auto& c : components)
	{
		c->Physics();
	}
}

void Entity::LateUpdate()
{
	for (auto& c : components)
	{
		c->LateUpdate();
	}
}

void Entity::DebugDraw()
{
	for (auto& c : components)
	{
		c->DebugDraw();
	}
}

void Entity::OnEnable()
{
	for (auto& c : components)
	{
		c->OnEnable();
	}
}

void Entity::OnDisable()
{
	for (auto& c : components)
	{
		c->OnDisable();
	}
}

void Entity::OnCollision(Hit* hit)
{
	for (auto& c : components)
	{
		c->OnCollision(hit);
	}
}

void Entity::OnTrigger(Hit* hit)
{
	for (auto& c : components)
	{
		c->OnTrigger(hit);
	}
}

bool Entity::IsActive() const
{
	return active;
}

void Entity::SetActive(bool a)
{
	active = a;
	EngineCore::Ecs->SubscribeEntityStateChange(this, active);
}

void Entity::SaveToDisk(const char* path)
{
	std::string data;
	for (auto& c : components)
	{
		std::string result = c->Parse();
		if (result != "")
		{
			data.append(result);
			data.append("\n");
		}
	}

	std::ofstream file(path);
	file << data;
}
