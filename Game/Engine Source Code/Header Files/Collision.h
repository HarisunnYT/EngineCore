#pragma once

#ifndef COLLISION_H
#define COLLISION_H

#include "EngineCore.h"

#include <vector>
#include <list>

class Hit;
class Collider;
class Collision
{
public:

	static void Init();
	static void DebugDraw();

	static void UpdateGrid(Collider* collider);
	static bool CheckCollision(Collider* collider, Hit& hit);

	static bool AABB(const SDL_Rect& rectA, const SDL_Rect& rectB);
	static bool AABB(Collider& colA, Collider& colB);

	static vector<pair<SDL_Rect, vector<Collider*>>> grid;

	static bool drawGrid;
};

#endif