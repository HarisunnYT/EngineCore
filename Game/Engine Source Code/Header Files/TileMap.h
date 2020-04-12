#pragma once

#ifndef MAP_H
#define MAP_H

#include "EngineCore.h"

class Entity;
class TileMap
{
public:

	TileMap();
	~TileMap();

	void LoadMap(std::string mapPath, const char* spriteSheetPath, Vector2 tileSize, Vector2 mapSize, float scale);
	void AddTile(const char* spriteSheetPath, std::string id, Vector3 position, Vector2 size, Vector2 source, float scale);

	void SetOffset(Vector3 vec);

	Vector2 mapSize;

private:

	std::vector<Entity*> tiles;

};

#endif