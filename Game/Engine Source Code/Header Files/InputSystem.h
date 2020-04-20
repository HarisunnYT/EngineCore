#pragma once

#ifndef INPUTSYSTEM_H
#define INPUTSYSTEM_H

#include "EngineCore.h"
#include "ECS.h"
#include "Components.h"

class InputSystem
{
public:

	static bool KeyHeld(Uint8 SDL_SCANCODE);

	static bool MousePressed(Uint8 button);
	static bool MouseReleased(Uint8 button);
	static bool MouseHeld(Uint8 button);

	static void Update();

	static Vector2 MousePosition;

};

#endif