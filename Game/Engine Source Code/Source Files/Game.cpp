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
#include "FBXMesh.h"
#include "FBXLoader.h"

#include "3rdPartyLibs/Includes/GLM/gtx/transform.hpp"

#include <iostream>

Entity* light;
//Entity* cube;

FBXLoader* loader = nullptr;

Game::Game()
{
	EngineCore::isDebug = false;
	Collision::drawGrid = false;

	//loader = new FBXLoader("Assets/Crow_Run.fbx");

	Entity* model = &EngineCore::Ecs->AddEntity();
	model->AddComponent<MeshRenderer>(new Model("Assets/Handgun_obj.obj"));

	for (int i = 0; i < 10; i++)
	{
		Entity* cube = &EngineCore::Ecs->AddEntity();
		cube->AddComponent<MeshRenderer>(new Cube());
		cube->transform->position.z = -25;
		cube->transform->position.x = i * 3;
	}

	light = &EngineCore::Ecs->AddEntity();
	light->AddComponent<Light>(LIGHT_TYPE::LIGHT_DIRECTIONAL);
	light->GetComponent<Light>().SetDiffuse(2.0f, 2.0f, 2.0f, 1.0f);
	light->transform->position = Vector3(0.0f, 30.0f, 0.0f);

	EngineCore::camera->entity->transform->eulerRotation = Vector3(0, 0, 1);
}

Game::~Game()
{
}

void Game::Update()
{
	//cube->transform->position.x += 0.01f;
	//EngineCore::camera->entity->transform->position.x += 0.01f;
}

void Game::LateUpdate()
{
}

void Game::Draw()
{
	//loader->DrawModel(0);
}

void Game::Physics()
{
}
