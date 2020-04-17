#pragma once

#ifndef CUBE_H
#define CUBE_H

#include "Drawable.h"

class Cube : public Drawable
{
public:

	Cube() = default;

	void Draw() override;

};

#endif