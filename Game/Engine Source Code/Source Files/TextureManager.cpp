#include "TextureManager.h"

map<const char*, SDL_Texture*> TextureManager::loadedTextures;

TextureManager::~TextureManager()
{
	for (auto& t : loadedTextures)
	{
		SDL_DestroyTexture(t.second);
	}
}

SDL_Texture* TextureManager::LoadTexture(const char* fileName)
{
	for (auto& t : loadedTextures)
	{
		std::string str1 = t.first;
		std::string str2 = fileName;
		if (str1 == str2)
		{
			return t.second;
		}
	}

	SDL_Surface* tempSurface = IMG_Load(fileName);
	SDL_Texture* tex = SDL_CreateTextureFromSurface(EngineCore::Renderer, tempSurface);

	SDL_FreeSurface(tempSurface);

	loadedTextures.emplace(fileName, tex);

	return tex;
}

void TextureManager::Draw(SDL_Texture* tex, SDL_Rect src, SDL_Rect dest)
{
	SDL_RenderCopy(EngineCore::Renderer, tex, &src, &dest);
}
