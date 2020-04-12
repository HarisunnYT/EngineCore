#pragma once

#ifndef HIT_H
#define HIT_H

#include "EngineCore.h"

class Collider;
class Hit
{
public:

	Hit() = default;
	Hit(Vector2 n, Collider* c);

	Vector2		normal;
	Collider*	collider;
};

#endif