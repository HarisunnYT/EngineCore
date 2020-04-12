#pragma once

#ifndef TEXTUREMANAGER_H
#define TEXTUREMANAGER_H

#include "EngineCore.h"

#include <map>

class TextureManager
{
public:

	~TextureManager();

	static SDL_Texture* LoadTexture(const char* fileName);
	static void Draw(SDL_Texture* tex, SDL_Rect src, SDL_Rect dest);

private:
	
	static map<const char*, SDL_Texture*> loadedTextures;

};

#endif