#pragma once

#ifndef CAMERA_H
#define CAMERA_H

#include "Entity.h"
#include "EngineCore.h"

class Entity;
class Transform;
class Camera : public Entity
{
public:

	Camera();

	bool InsideCameraView(SDL_Rect* rect);

	SDL_Rect offset;

private:

	Transform*	transform;

	Vector2			screenTopLeft = Vector2(0, 0);
	Vector2			screenBottomLeft = Vector2(0, EngineCore::screenSize.y);
	Vector2			screenTopRight = Vector2(EngineCore::screenSize.x, 0);
	Vector2			screenBottomRight = Vector2(EngineCore::screenSize.x, EngineCore::screenSize.y);

	Vector2			inputTopLeft;
	Vector2			inputBottomLeft;
	Vector2			inputTopRight;
	Vector2			inputBottomRight;

};

#endif