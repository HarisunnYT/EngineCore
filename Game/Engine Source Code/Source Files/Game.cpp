#include "Game.h"
#include "TextureManager.h"
#include "TileMap.h"
#include "ECS.h"
#include "Components.h"
#include "SecondaryComponents.h"
#include "InputSystem.h"

#include <iostream>

Entity* e;

Game::Game()
{
	EngineCore::renderMode = RENDERMODE_3D;

	EngineCore::isDebug = false;
	Collision::drawGrid = false;

	e = &EngineCore::Ecs->AddEntity();
	e->AddComponent<SpriteRenderer>("Assets/mushroom.png", Vector2(500, 500));
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

void Game::Render()
{
}

void Game::Physics()
{
}
