#pragma once

#ifndef MESHRENDERER_H
#define MESHRENDERER_H

#include "EngineCore.h"

class Drawable;
class Texture;
class MeshRenderer : public Component
{
public:

	MeshRenderer(Drawable* drawable);
	MeshRenderer(Drawable* drawable, Texture* texture);

	void Draw() override;

	Texture* GetTexture();

private:

	Drawable* drawable;
	Texture* texture;
};

#endif