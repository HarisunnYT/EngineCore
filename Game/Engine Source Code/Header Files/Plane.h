#pragma once

#ifndef PLANE_H
#define PLANE_H

#include "Drawable.h"

class Plane : public Drawable
{
public:

	Plane() = default;

	void Draw() override;

};

#endif