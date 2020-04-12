#pragma once

#ifndef ECS_H
#define ECS_H

#include "Components.h"

#include <iostream>
#include <vector>
#include <memory>
#include <algorithm>
#include <bitset>
#include <array>
#include <list>

class Entity;
class Transform;
class ECS
{
public:

	ECS();

	void Update();
	void Draw();
	void Physics();
	void LateUpdate();
	void DebugDraw();

	void SubscribeEntityStateChange(Entity* entity, bool state);

	Entity& AddEntity();
	Entity& AddEntity(Entity* entity);
	Entity& AddEntity(const char* path);

	std::list<Entity*> entities;

private:

	std::list<std::pair<Entity*, bool>> changedEntities;

};

#endif