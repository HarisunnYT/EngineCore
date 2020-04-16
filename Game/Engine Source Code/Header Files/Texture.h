#pragma once

#ifndef TEXTURE_H
#define TEXTURE_H

#include "SDL_opengl.h"
#include "SDL_opengl_glext.h"

#include <string>
#include <vector>

struct TGAHeader
{
	GLubyte ID_Length;
	GLubyte ColorMapType;
	GLubyte ImageType;
	GLubyte ColorMapSpecification[5];
	GLshort xOrigin;
	GLshort yOrigin;
	GLshort ImageWidth;
	GLshort ImageHeight;
	GLubyte PixelDepth;
};

class Texture
{
public:

	Texture(std::string fileName, std::string name = "");
	~Texture();

	unsigned char*	imageData;
	unsigned int	bpp;
	unsigned int	width;
	unsigned int	height;
	unsigned int	texID;

	std::string		name;

	static std::vector<Texture*> textures;

private:

	bool	LoadTGA(std::string fileName);
	bool	CreateTexture(unsigned char* imageData, int width, int height, int type);

};

#endif