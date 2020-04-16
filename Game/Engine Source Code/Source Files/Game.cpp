#include "Game.h"
#include "TextureManager.h"
#include "TileMap.h"
#include "ECS.h"
#include "Components.h"
#include "SecondaryComponents.h"
#include "InputSystem.h"
#include "Engine Source Code/Header Files/Model.h"

#include <iostream>

Entity* e;
Model* model;

Game::Game()
{
	model = new Model("Assets/Handgun_obj");

	EngineCore::isDebug = false;
	Collision::drawGrid = false;

	//e = &EngineCore::Ecs->AddEntity();
	//e->AddComponent<SpriteRenderer>("Assets/mushroom.png", Vector2(500, 500));

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
	glDisable(GL_LIGHTING);
	glEnable(GL_BLEND);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glTranslatef(400, 300, 0);
	glColor3f(1.0f, 0.0f, 0.0f);

	glBegin(GL_TRIANGLES);

	glVertex3f(0.0f, 121.0f, 0.0f);
	glVertex3f(-121.0f, -121.0f, 0.0f);
	glVertex3f(121.0f, -121.0f, 0.0f);
	glEnd();

	//model->DrawModel();

	glEnd();

}

void Game::Physics()
{
}
