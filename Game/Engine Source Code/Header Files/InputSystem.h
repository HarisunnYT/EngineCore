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

	static bool Mouse(int button);

	static void Update();

	static SDL_Point MousePosition;

};

#endif