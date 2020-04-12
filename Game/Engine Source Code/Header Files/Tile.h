#pragma once

#ifndef TILE_H
#define TILE_H

#include "EngineCore.h"
#include "TextureManager.h"

#include <vector>

class Transform;
class Tile : public Component
{
public:

	Tile() = default;
	Tile(const char* path, Vector3 position, Vector2 size, Vector2 source, float scale);
	~Tile();

	void ManualConstruction(Vector3 position, float scale);

	void Init() override;
	void Update() override;
	void Draw() override;

	void SetSource(Vector2 source);

	std::string Parse() override;
	static bool TryParse(std::string value, Entity* entity);
	static std::string componentName;

	template<class Archive>
	void Serialize(Archive& archive)
	{
		archive(componentName, currentPath, size.ToString(), source.ToString());
	}

private:

	void SetRects();

	std::string		currentPath;

	SDL_Texture*	texture;
	SDL_Rect		sourceRect;
	SDL_Rect		destinationRect;

	Vector3			startingPosition;
	Vector2			startingScale;
	Vector2			source;
	Vector2			size;

	Transform*		transform;
	Collider*		collider;
};

#endif