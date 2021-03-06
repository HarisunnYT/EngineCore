#pragma once

#ifndef DRAWABLE_H
#define DRAWABLE_H

#include "EngineCore.h"

#include "MeshRenderer.h"
#include "Texture.h"

class MeshRenderer;
class Drawable
{
public:

	Drawable() = default;

	virtual void Load() {}
	virtual void Draw();

protected:

	MeshRenderer* meshRenderer;

	friend class MeshRenderer;
};

#endif