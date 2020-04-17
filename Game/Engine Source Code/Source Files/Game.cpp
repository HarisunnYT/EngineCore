#include "Game.h"
#include "TextureManager.h"
#include "TileMap.h"
#include "ECS.h"
#include "Components.h"
#include "SecondaryComponents.h"
#include "InputSystem.h"
#include "Engine Source Code/Header Files/Model.h"

#include "Drawable.h"
#include "Texture.h"
#include "Light.h"

#include "Cube.h"

#include <iostream>

Entity* light;
Entity* cube;

Game::Game()
{
	EngineCore::isDebug = false;
	Collision::drawGrid = false;

	glEnable(GL_LIGHTING);

	Light::StaticInitialise();

	cube = &EngineCore::Ecs->AddEntity();
	cube->AddComponent<MeshRenderer>(new Cube());

	light = &EngineCore::Ecs->AddEntity();
	light->AddComponent<Light>(LIGHT_TYPE::LIGHT_SPOT);
	light->GetComponent<Light>().SetDiffuse(2.0f, 2.0f, 2.0f, 1.0f);
	light->transform->position = Vector3(0.0f, 5.0f, 0.0f);
}

Game::~Game()
{
}

void Game::Update()
{
	cube->transform->position.x -= 0.01f;
}

void Game::LateUpdate()
{
}

void Game::Render()
{
	glTranslatef(0, 0, -25);
}

void Game::Physics()
{
}
