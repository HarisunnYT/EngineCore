#include "TileMap.h"
#include "EngineCore.h"

#include <fstream>

TileMap::TileMap()
{	
}

TileMap::~TileMap()
{
}

void TileMap::LoadMap(std::string mapPath, const char* spriteSheetPath, Vector2 tileSize, Vector2 mpSize, float scale)
{
	char c;
	std::fstream mapFile;
	mapFile.open(mapPath);

	int sourceX, sourceY;

	for (int y = 0; y < mpSize.y; y++)
	{
		for (int x = 0; x < mpSize.x; x++)
		{
			std::string id;
			mapFile.get(c);
			sourceY = static_cast<int>(atoi(&c) * tileSize.y);

			id.push_back(c);

			mapFile.get(c);

			if (c == ',')
			{
				sourceX = sourceY;
				sourceY = 0;
				id.insert(id.begin(), '0');
			}
			else
			{
				sourceX = static_cast<int>(atoi(&c) * tileSize.x);
				mapFile.ignore();
				id.push_back(c);
			}

			AddTile(spriteSheetPath, id, Vector3(x * tileSize.x * scale, y * tileSize.y * scale, 0), tileSize, Vector2(static_cast<float>(sourceX), static_cast<float>(sourceY)), scale);

			if (x == static_cast<int>(mpSize.x - 1))
			{
				mapFile.ignore();
			}
		}
	}

	mapFile.close();
	mapSize = mpSize * scale;
}

void TileMap::AddTile(const char* spriteSheetPath, std::string id, Vector3 position, Vector2 size, Vector2 source, float scale)
{
	std::string p = "Assets/Prefabs/Tiles/" + id;
	std::ifstream inFile(p);

	Entity* tile;
	if (inFile.good())
	{
		tile = &(EngineCore::Ecs->AddEntity(p.c_str()));
		Vector3 pos = Vector3(position.x, position.y, tile->transform->GetPosition().z);
		tile->GetComponent<Tile>().ManualConstruction(pos, scale);
		tile->GetComponent<Tile>().Init();
	}
	else
	{
		tile = &(EngineCore::Ecs->AddEntity());
		tile->AddComponent<Tile>(spriteSheetPath, position, size, source, scale);
	}

	tiles.push_back(tile);
}

void TileMap::SetOffset(Vector3 pos)
{
	for (auto& t : tiles)
	{
		if (t != nullptr)
		{
			Vector3 p = t->transform->GetPosition();
			t->transform->SetPosition(Vector3(p.x + pos.x, p.y + pos.y, pos.z));
		}
	}
}
