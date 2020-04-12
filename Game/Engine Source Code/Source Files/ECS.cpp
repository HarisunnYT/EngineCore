#include "ECS.h"
#include "Entity.h"
#include "Animator.h"
#include "Particle.h"

#include "PlayerController.h"
#include "Brick.h"
#include "Coin.h"
#include "Mushroom.h"
#include "Goomba.h"

#include <fstream>
#include <iostream>

ECS::ECS()
{
	entities = std::list<Entity*>();
}

void ECS::Update()
{
	for (auto& e : entities)
	{
		if (e != nullptr && e->IsActive())
		{
			e->Update();
		}
	}
}

struct compare_z_depth
{
	inline bool operator() (Entity *a, Entity *b)
	{
		return a->transform->GetPosition().z < b->transform->GetPosition().z;
	}
};


void ECS::Draw()
{
	entities.sort(compare_z_depth());
	for (auto& entity : entities)
	{
		if (entity != nullptr)
		{
			entity->Draw();
		}
	}
}

void ECS::Physics()
{
	for (auto& e : entities)
	{
		if (e != nullptr)
		{
			e->Physics();
		}
	}
}

void ECS::LateUpdate()
{
	for (auto& e : entities)
	{
		if (e != nullptr)
		{
			e->LateUpdate();
		}
	}

	for (auto& e : changedEntities)
	{
		if (e.second)
		{
			EngineCore::Ecs->entities.push_back(e.first);
			e.first->OnEnable();
		}
		else
		{
			e.first->OnDisable();
			EngineCore::Ecs->entities.remove(e.first);
		}
	}

	changedEntities.clear();
}

void ECS::DebugDraw()
{
	if (EngineCore::isDebug)
	{
		for (auto& entity : entities)
		{
			if (entity != nullptr)
			{
				entity->DebugDraw();
			}
		}
	}
}

void ECS::SubscribeEntityStateChange(Entity* entity, bool state)
{
	changedEntities.push_back(pair<Entity*, bool>(entity, state));
}

Entity& ECS::AddEntity()
{
	Entity* e = new Entity();
	return AddEntity(e);
}

Entity& ECS::AddEntity(Entity* entity)
{
	Entity* uPtr{ entity };
	entities.emplace_back(std::move(uPtr));

	return *entity;
}

Entity& ECS::AddEntity(const char* path)
{
	std::ifstream file(path);
	std::string content((std::istreambuf_iterator<char>(file)), (std::istreambuf_iterator<char>()));

	vector<std::string> values;

	std::string currentVal;
	char previousCharacter = ' ';

	for (auto& c : content)
	{
		if (c == '\n' && previousCharacter == '}')
		{
			values.push_back(currentVal);
			currentVal.clear();
		}
		else
		{
			currentVal.push_back(c);
		}

		previousCharacter = c;
	}

	if (values.size() > 0)
	{
		Entity* entity = &EngineCore::Ecs->AddEntity();
		for (auto& str : values)
		{
			if (str.find(Transform::componentName) != string::npos && Transform::TryParse(str, entity)) {}
			else if (str.find(Collider::componentName) != string::npos && Collider::TryParse(str, entity)) {}
			else if (str.find(SpriteRenderer::componentName) != string::npos && SpriteRenderer::TryParse(str, entity)) {}
			else if (str.find(Rigidbody::componentName) != string::npos && Rigidbody::TryParse(str, entity)) {}
			else if (str.find(Animator::componentName) != string::npos && Animator::TryParse(str, entity)) {}
			else if (str.find(Tile::componentName) != string::npos && Tile::TryParse(str, entity)) {}
			else if (str.find(Particle::componentName) != string::npos && Particle::TryParse(str, entity)) {}
			else if (str.find(AudioListener::componentName) != string::npos && AudioListener::TryParse(str, entity)) {}
			else if (str.find(AudioSource::componentName) != string::npos && AudioSource::TryParse(str, entity)) {}
			else if (str.find(PlayerController::componentName) != string::npos && PlayerController::TryParse(str, entity)) {}
			else if (str.find(Brick::componentName) != string::npos && Brick::TryParse(str, entity)) {}
			else if (str.find(Coin::componentName) != string::npos && Coin::TryParse(str, entity)) {}
			else if (str.find(Mushroom::componentName) != string::npos && Mushroom::TryParse(str, entity)) {}
			else if (str.find(Goomba::componentName) != string::npos && Goomba::TryParse(str, entity)) {}
		}

		entities.emplace_back(std::move(entity));

		return *entity;
	}

	throw std::invalid_argument("prefab does not exist");
}