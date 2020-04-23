#include "EngineCore.h"
#include "Game.h"
#include "TextureManager.h"
#include "Components.h"
#include "InputSystem.h"
#include "Camera.h"
#include "Light.h"

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

	EstablishProjectionMatrix();

	glEnable(GL_DEPTH_TEST);
	glShadeModel(GL_SMOOTH);

	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
	glEnable(GL_PERSPECTIVE_CORRECTION_HINT);

	glEnable(GL_LIGHTING);
	glEnable(GL_TEXTURE_2D);

	isRunning = true;

	Collision::Init();
	Light::StaticInitialise();

	Ecs = new ECS();

	camera = &Ecs->AddEntity().AddComponent<Camera>();

	game = new Game();
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

	EstablishProjectionMatrix();

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	camera->ViewUpdate();

	glEnable(GL_LIGHTING);
	glDisable(GL_BLEND);

	game->Draw();
	Ecs->Draw();
	Ecs->DebugDraw();
	Collision::DebugDraw();

	SDL_GL_SwapWindow(window);
}

void EngineCore::Clean()
{
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

void EngineCore::EstablishProjectionMatrix()
{
	glViewport(0, 0, (GLsizei)screenSize.x, (GLsizei)screenSize.y);

	glClearColor(0.5f, 0.5f, 0.5f, 1);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	//gluPerspective(45.0f, (GLdouble)screenSize.x / (GLdouble)screenSize.y, nearClippingPlane, farClippingPlane);
}

void EngineCore::SetOrthographic()
{
	glViewport(0, 0, (GLsizei)screenSize.x, (GLsizei)screenSize.y);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	glOrtho(0, (GLfloat)screenSize.x, (GLfloat)screenSize.y, 0, nearClippingPlane, farClippingPlane);
}
