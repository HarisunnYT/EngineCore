#include "Game.h"
#include "TextureManager.h"
#include "TileMap.h"
#include "ECS.h"
#include "Components.h"
#include "SecondaryComponents.h"
#include "InputSystem.h"

#include <iostream>

Game::Game()
{
	EngineCore::isDebug = false;
	Collision::drawGrid = false;
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
