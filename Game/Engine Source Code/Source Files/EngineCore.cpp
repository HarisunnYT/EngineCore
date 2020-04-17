#include "EngineCore.h"
#include "Game.h"
#include "TextureManager.h"
#include "Components.h"
#include "InputSystem.h"
#include "Camera.h"

#include <iostream>

SDL_GLContext EngineCore::mainContext;
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

SDL_Texture* texture;
const void* pixels;

Game* game = nullptr;

EngineCore::EngineCore()
{
}

EngineCore::~EngineCore()
{
}

void EngineCore::Init(const char* title, int xpos, int ypos, bool fullscreen)
{
	SDL_Init(SDL_INIT_EVERYTHING);

	window = SDL_CreateWindow(title, xpos, ypos, static_cast<int>(screenSize.x), static_cast<int>(screenSize.y), SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN);
	mainContext = SDL_GL_CreateContext(window);
	Renderer = SDL_CreateRenderer(window, -1, SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45.0f, (GLfloat)screenSize.x / (GLfloat)screenSize.y, 0.1f, 500.0f);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glEnable(GL_TEXTURE_2D);
	glEnable(GL_DEPTH_TEST);

	isRunning = true;

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
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	//SDL_RenderClear(Renderer);

	glViewport(0, 0, (GLint)EngineCore::screenSize.x, (GLint)EngineCore::screenSize.y);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	gluPerspective(45.0f, (GLfloat)EngineCore::screenSize.x / (GLfloat)EngineCore::screenSize.y, 0.1f, 200.0f);

	glEnable(GL_BLEND);

	game->Render();
	Ecs->Draw();
	Ecs->DebugDraw();
	Collision::DebugDraw();

	//SDL_RenderPresent(Renderer);
	SDL_GL_SwapWindow(window);
}

void EngineCore::Clean()
{
	delete camera;
	delete Ecs;
	delete game;

	SDL_DestroyWindow(window);
	SDL_DestroyRenderer(Renderer);
	SDL_Quit();
}

bool EngineCore::Running()
{
	return isRunning;
}
