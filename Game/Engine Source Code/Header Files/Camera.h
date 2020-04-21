#pragma once

#ifndef CAMERA_H
#define CAMERA_H

#include "EngineCore.h"

#include "Matrix4x4.h"

class Component;
class Transform;
class Camera : public Component
{
public:

	Camera();

	void ViewUpdate();

	void Update() override;
	void Draw() override;

	bool InsideCameraView(SDL_Rect* rect);

	SDL_Rect offset;

private:

	Vector2			screenTopLeft = Vector2(0, 0);
	Vector2			screenBottomLeft = Vector2(0, EngineCore::screenSize.y);
	Vector2			screenTopRight = Vector2(EngineCore::screenSize.x, 0);
	Vector2			screenBottomRight = Vector2(EngineCore::screenSize.x, EngineCore::screenSize.y);

	Vector2			inputTopLeft;
	Vector2			inputBottomLeft;
	Vector2			inputTopRight;
	Vector2			inputBottomRight;

	Vector2 previousMouseHeldPosition;
	float movementSpeed = 0.1f;
	float rotationDamper = 500.0f;

};

#endif