#pragma once

#ifndef SPRITERENDERER_H
#define SPRITERENDERER_H

#include "EngineCore.h"

class Transform;
class SpriteRenderer : public Component
{
public:

	SpriteRenderer(const char* path, Vector2 size);
	~SpriteRenderer();

	void Init() override;
	void Draw() override;
	void SetTexture(const char* path);

	Vector2			spriteSize;

	std::string Parse() override;
	static bool TryParse(std::string value, Entity* entity);
	static std::string componentName;

	template<class Archive>
	void Serialize(Archive& archive)
	{
		archive(componentName, currentPath, size.ToString());
	}

	bool enabled = true;

protected:

	Transform*		transform;

	SDL_Texture*	texture;
	
	std::string		currentPath;

	SDL_Rect		sourceRect;
	SDL_Rect		destinationRect;

};

#endif