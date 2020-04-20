#include "InputSystem.h"

Vector2 InputSystem::MousePosition;

bool InputSystem::KeyHeld(Uint8 SDL_SCANCODE)
{
	return SDL_GetKeyboardState(NULL)[SDL_SCANCODE];
}

bool InputSystem::MousePressed(Uint8 button)
{
	return EngineCore::Event.type == SDL_MOUSEBUTTONDOWN && EngineCore::Event.button.button == button;
}

bool InputSystem::MouseReleased(Uint8 button)
{
	return EngineCore::Event.type == SDL_MOUSEBUTTONUP && EngineCore::Event.button.button == button;
}

bool InputSystem::MouseHeld(Uint8 button)
{
	return SDL_GetMouseState(NULL, NULL) & SDL_BUTTON(button);
}

void InputSystem::Update()
{
	SDL_Point p;
	SDL_GetMouseState(&p.x, &p.y);

	MousePosition = Vector2(p.x, p.y);
}