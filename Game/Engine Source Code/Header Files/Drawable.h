#pragma once

#ifndef DRAWABLE_H
#define DRAWABLE_H

#include "EngineCore.h"

class MeshRenderer;
class Drawable
{
public:

	Drawable() = default;

	virtual void Load() {}
	virtual void Draw() = 0;

protected:

	MeshRenderer* meshRenderer;

	friend class MeshRenderer;
};

#endif