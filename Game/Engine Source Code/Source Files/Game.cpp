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

#include "Plane.h"

#include <iostream>

Entity* light;
Entity* plane;

Game::Game()
{
	EngineCore::isDebug = false;
	Collision::drawGrid = false;

	glEnable(GL_LIGHTING);

	plane = &EngineCore::Ecs->AddEntity();
	plane->AddComponent<MeshRenderer>(new Plane());
	plane->transform->position.y = -200;
	plane->transform->scale = Vector3::One() * 10;

	light = &EngineCore::Ecs->AddEntity();
	light->AddComponent<Light>(LIGHT_TYPE::LIGHT_SPOT);
	light->GetComponent<Light>().SetDiffuse(2.0f, 2.0f, 2.0f, 1.0f);
	light->transform->position = Vector3(0.0f, 5.0f, 0.0f);

	EngineCore::camera->transform->eulerRotation = Vector3(0, 0, 1);
}

Game::~Game()
{
}

void Game::Update()
{
}

void Game::LateUpdate()
{
}

void Game::Draw()
{
}

void Game::Physics()
{
}
