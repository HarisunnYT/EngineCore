#pragma once

#ifndef LIGHT_H
#define LIGHT_H

#include "EngineCore.h"

#include <vector>

enum class LIGHT_TYPE
{
	LIGHT_SPOT,
	LIGHT_POINT,
	LIGHT_DIRECTIONAL
};

class Light : public Component
{
public:

	static void StaticInitialise(void);

	Light(LIGHT_TYPE lightType);
	~Light();

	void Init() override;
	void Update() override;

	void Visible(bool value = true);

	void SetLightType(LIGHT_TYPE lightType);
	void SetAmbient(float r, float g, float b, float a);
	void SetDiffuse(float r, float g, float b, float a);
	void SetSpecular(float r, float g, float b, float a);

	void SetCutoff(float value);
	void SetExponent(float value);

	void SetAttenuation(float constant, float linear, float quadratic);

	int GetLightNum(void);

	static int numLights;
	static vector<int> availableLights;
	static vector<Light*> lights;

private:

	GLfloat		position[4];
	GLfloat		diffuse[4];
	GLfloat		ambient[4];
	GLfloat		specularity[4];
	GLfloat		spotdirection[4];

	float		cutoff;
	float		exponent;

	bool		visible;
	int			lightNum;
	LIGHT_TYPE	lightType;
};

#endif