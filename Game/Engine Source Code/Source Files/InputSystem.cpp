#include "InputSystem.h"

SDL_Point InputSystem::MousePosition;

bool InputSystem::KeyHeld(Uint8 SDL_SCANCODE)
{
	return SDL_GetKeyboardState(NULL)[SDL_SCANCODE];
}

bool InputSystem::Mouse(int button)
{
	return SDL_GetMouseState(NULL, NULL) & SDL_BUTTON(button);
}

void InputSystem::Update()
{
	SDL_GetMouseState(&MousePosition.x, &MousePosition.y);
}