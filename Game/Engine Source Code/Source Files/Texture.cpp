#include "..\Header Files\Texture.h"

#include <fstream>

std::vector<Texture*> Texture::textures;

Texture::Texture(std::string fn, std::string n)
{
	imageData = nullptr;

	LoadTGA(fn);
	name = n;

	textures.push_back(this);
}

Texture::~Texture()
{
	for (std::vector<Texture*>::iterator it = textures.begin(); it != textures.end(); it++)
	{
		if ((*it) == this)
		{
			textures.erase(it);
		}
	}

	if (imageData != nullptr)
		delete imageData;
}

bool Texture::LoadTGA(std::string fileName)
{
	TGAHeader tgaHeader;

	std::ifstream file(fileName.data(), std::ios_base::binary);

	if (!file.is_open())
		return false;

	if (!file.read((char*)&tgaHeader, sizeof(tgaHeader)))
		return false;

	if (tgaHeader.ImageType != 2)
		return false;

	width = tgaHeader.ImageWidth;
	height = tgaHeader.ImageHeight;
	bpp = tgaHeader.PixelDepth;

	if (width <= 0 || height <= 0 || (bpp != 24 && bpp != 32))
		return false;

	GLuint type = GL_RGBA;

	if (bpp == 24)
		type = GL_RGB;

	GLuint bytesPerPixel = bpp / 8;
	GLuint imageSize = width * height * bytesPerPixel;

	imageData = new GLubyte[imageSize];

	if (imageData == nullptr)
		return false;

	if (!file.read((char*)imageData, imageSize))
	{
		delete imageData;
		return false;
	}

	//flip BGR to RGB
	for (GLuint i = 0; i < (int)imageSize; i += bytesPerPixel)
	{
		GLuint temp = imageData[i];
		imageData[i] = imageData[i + 2];
		imageData[i + 2] = temp;
	}

	return CreateTexture(imageData, width, height, type);
}

bool Texture::CreateTexture(unsigned char* imageData, int width, int height, int type)
{
	glGenTextures(1, &texID);
	glBindTexture(GL_TEXTURE_2D, texID);

	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glTexImage2D(GL_TEXTURE_2D, 0, type, width, height, 0, type, GL_UNSIGNED_BYTE, imageData);

	return true;
}
