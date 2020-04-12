#include "Game.h"
#include "TextureManager.h"
#include "TileMap.h"
#include "ECS.h"
#include "Components.h"
#include "SecondaryComponents.h"
#include "InputSystem.h"

#include <iostream>

TileMap* tileMap = nullptr;

Entity* player = nullptr;
Entity* bgMusic = nullptr;

vector<Entity*> goombas;

Vector3 previousPlayerPosition;

Game::Game()
{
	EngineCore::isDebug = false;
	Collision::drawGrid = false;

	player = &EngineCore::Ecs->AddEntity("Assets/Prefabs/player");
	//player->GetComponent<AudioListener>().audioEnabled = false;

	tileMap = new TileMap();
	tileMap->LoadMap("Assets/Map/map.map", "Assets/Map/tileset.png", Vector2(16, 16), Vector2(102, 11), 3.44f);

	bgMusic = &EngineCore::Ecs->AddEntity();
	bgMusic->AddComponent<AudioSource>();
	bgMusic->GetComponent<AudioSource>().Play(L"Assets/Audio/bg.wav");

	SpawnGoombas();

	EngineCore::camera->offset.x = static_cast<int>(player->transform->GetPosition().x);
}

Game::~Game()
{
}

void Game::Update()
{
	if (player->transform->GetPosition().x > EngineCore::screenSize.x / 2 && player->transform->GetRawPosition().x > previousPlayerPosition.x)
	{
		EngineCore::camera->offset.x = static_cast<int>(player->transform->GetRawPosition().x - (EngineCore::screenSize.x / 2));
	}
	previousPlayerPosition = player->transform->GetRawPosition();
}

void Game::LateUpdate()
{
	for (auto& g : goombas)
	{
		if (EngineCore::camera->InsideCameraView(&g->GetComponent<Collider>().RawCollider()) && !g->IsActive() && !g->GetComponent<Goomba>().IsDead())
		{
			g->SetActive(true);
		}
	}
}

void Game::Render()
{
}

void Game::Physics()
{
}

void Game::SpawnGoombas()
{
	Entity* goomba = &EngineCore::Ecs->AddEntity("Assets/Prefabs/goomba");
	goomba->GetComponent<Animator>().PlayAnimation(0);
	goombas.push_back(goomba);

	Vector3 pos = goomba->transform->GetPosition();

	goomba = &EngineCore::Ecs->AddEntity("Assets/Prefabs/goomba");
	goomba->transform->SetPosition(Vector3(pos.x + 1000, pos.y, pos.z));
	goombas.push_back(goomba);

	goomba = &EngineCore::Ecs->AddEntity("Assets/Prefabs/goomba");
	goomba->transform->SetPosition(Vector3(pos.x + 1400, pos.y, pos.z));
	goombas.push_back(goomba);

	goomba = &EngineCore::Ecs->AddEntity("Assets/Prefabs/goomba");
	goomba->transform->SetPosition(Vector3(pos.x + 3100, pos.y - 300, pos.z));
	goombas.push_back(goomba);

	goomba = &EngineCore::Ecs->AddEntity("Assets/Prefabs/goomba");
	goomba->transform->SetPosition(Vector3(pos.x + 3150, pos.y - 300, pos.z));
	goombas.push_back(goomba);

	goomba = &EngineCore::Ecs->AddEntity("Assets/Prefabs/goomba");
	goomba->transform->SetPosition(Vector3(pos.x + 4400, pos.y, pos.z));
	goombas.push_back(goomba);

	goomba = &EngineCore::Ecs->AddEntity("Assets/Prefabs/goomba");
	goomba->transform->SetPosition(Vector3(pos.x + 4450, pos.y, pos.z));
	goombas.push_back(goomba);

	for (auto& g : goombas)
	{
		g->SetActive(false);
	}
}
