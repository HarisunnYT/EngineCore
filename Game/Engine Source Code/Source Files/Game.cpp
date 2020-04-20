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
#include "Model.h"

#include "3rdPartyLibs/Includes/GLM/gtx/transform.hpp"

#include <iostream>

Entity* light;
Entity* cube;

Game::Game()
{
	EngineCore::isDebug = false;
	Collision::drawGrid = false;

	glEnable(GL_LIGHTING);

	cube = &EngineCore::Ecs->AddEntity();
	cube->AddComponent<MeshRenderer>(new Cube());
	cube->transform->position.z = 25;

	light = &EngineCore::Ecs->AddEntity();
	light->AddComponent<Light>(LIGHT_TYPE::LIGHT_SPOT);
	light->GetComponent<Light>().SetDiffuse(2.0f, 2.0f, 2.0f, 1.0f);
	light->transform->position = Vector3(0.0f, 5.0f, 0.0f);

	EngineCore::camera->transform->eulerRotation = Vector3(0, 0, 1);

	float m1[4][4] =
	{
		{ 10.0f, 3.0f, 8.0f, 3.0f },
		{ 5.0f, 7.0f, 3.0f, 7.0f },
		{ 4.0f, 7.0f, 3.0f, 1.0f },
		{ 0.0f, 4.0f, 9.0f, 7.0f }
	};

	float m2[4][4] =
	{
		{ 4.0f, 6.0f, 3.0f, 4.0f },
		{ 2.0f, 4.0f, 7.0f, 6.0f },
		{ 5.0f, 7.0f, 3.0f, 4.0f },
		{ 3.0f, 4.0f, 6.0f, 7.0f }
	};

	Matrix4x4 mat1 = Matrix4x4(m1);
	Matrix4x4 mat2 = Matrix4x4(m2);

	Matrix4x4 result = mat1 * mat2;

	std::cout << result.ToString() << std::endl;
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
