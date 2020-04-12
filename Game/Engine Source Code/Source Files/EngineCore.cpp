#include "EngineCore.h"
#include "Game.h"
#include "TextureManager.h"
#include "Components.h"
#include "InputSystem.h"
#include "Camera.h"

#include <iostream>

SDL_Renderer* EngineCore::Renderer = nullptr;
SDL_Event EngineCore::Event;
ECS* EngineCore::Ecs = nullptr;
Camera* EngineCore::camera = nullptr;
AudioListener* EngineCore::audioListener = nullptr;

bool EngineCore::isRunning = false;
bool EngineCore::isDebug = false;
Vector2 EngineCore::screenSize = Vector2(800.0f, 600.0f);

float EngineCore::deltaTime = 0.0;
float EngineCore::fixedTimeStep = 0.1f;

Game* game = nullptr;

EngineCore::EngineCore()
{
}

EngineCore::~EngineCore()
{
}

void EngineCore::Init(const char* title, int xpos, int ypos, bool fullscreen)
{
	if (SDL_Init(SDL_INIT_EVERYTHING) == 0)
	{
		int flags = 0;
		if (fullscreen)
		{
			flags = SDL_WINDOW_FULLSCREEN;
		}

		window = SDL_CreateWindow(title, xpos, ypos, static_cast<int>(screenSize.x), static_cast<int>(screenSize.y), flags);
		Renderer = SDL_CreateRenderer(window, -1, 0);

		SDL_SetRenderDrawColor(Renderer, 99, 173, 255, 255);

		isRunning = true;
	}
	else
	{
		isRunning = false;
	}

	Collision::Init();

	camera = new Camera();
	Ecs = new ECS();
	game = new Game();

	Ecs->AddEntity((Entity*)camera);
}

void EngineCore::HandleEvents()
{
	SDL_PollEvent(&Event);
	switch (Event.type)
	{
	case SDL_QUIT:
		isRunning = false;
		break;
	default:
		break;
	}
}

void EngineCore::Update()
{
	InputSystem::Update();

	Ecs->Update();
	game->Update();

	game->Physics();
	Ecs->Physics();

	Ecs->LateUpdate();
	game->LateUpdate();

	if (InputSystem::KeyHeld(SDLK_ESCAPE))
	{
		EngineCore::isRunning = false;
	}
}

void EngineCore::Render()
{
	SDL_RenderClear(Renderer);

	game->Render();
	Ecs->Draw();
	Ecs->DebugDraw();
	Collision::DebugDraw();

	SDL_RenderPresent(Renderer);
}

void EngineCore::Clean()
{
	SDL_DestroyWindow(window);
	SDL_DestroyRenderer(Renderer);
	SDL_Quit();
}

bool EngineCore::Running()
{
	return isRunning;
}
