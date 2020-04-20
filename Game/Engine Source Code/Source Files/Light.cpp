#include "..\Header Files\Light.h"

int Light::numLights;
vector<int> Light::availableLights;
vector<Light*> Light::lights;

void Light::StaticInitialise(void)
{
	glGetIntegerv(GL_MAX_LIGHTS, &numLights);

	for (int i = 0; i < numLights; i++)
	{
		availableLights.push_back(GL_LIGHT0 + i);
	}
}

Light::Light(LIGHT_TYPE type)
{
	lights.push_back(this);
	lightType = type;
}

Light::~Light()
{
	if (lightNum != 0) 
	{
		availableLights.push_back(lightNum);
	}

	for (vector<Light*>::iterator it = lights.begin(); it != lights.end(); it++)
	{
		if ((*it) == this) 
		{
			lights.erase(it);
			break;
		}
	}
}

void Light::Init()
{
	if ((int)availableLights.size() > 0)
	{
		lightNum = availableLights[0];
		availableLights.erase(availableLights.begin());

		Visible(true);

		SetLightType(lightType);
		SetAmbient(0, 0, 0, 1);
		SetDiffuse(1, 1, 1, 1);
		SetSpecular(1, 1, 1, 1);

		entity->transform->eulerRotation = Vector3(0, -1, 0);
		SetCutoff(45.0f);
		SetExponent(12.0f);
	}
	else
	{
		lightNum = 0;
		Visible(false);
	}
}

void Light::Update()
{
	position[0] = entity->transform->position.x;
	position[1] = entity->transform->position.y;
	position[2] = entity->transform->position.z;

	spotdirection[0] = entity->transform->eulerRotation.x;
	spotdirection[1] = entity->transform->eulerRotation.y;
	spotdirection[2] = entity->transform->eulerRotation.z;

	glLightfv(lightNum, GL_POSITION, position);
	glLightfv(lightNum, GL_SPOT_DIRECTION, spotdirection);
}

void Light::Visible(bool value)
{
	visible = value;

	if (visible)
	{
		glEnable(lightNum);
	}
	else
	{
		glDisable(lightNum);
	}
}

void Light::SetLightType(LIGHT_TYPE type)
{
	lightType = type;
	if (lightType == LIGHT_TYPE::LIGHT_SPOT)
	{
		position[3] = 1.0f;
	}
	else if (lightType == LIGHT_TYPE::LIGHT_POINT)
	{
		position[3] = 1.0f;
		SetCutoff(180.0f);
	}
	else if (lightType == LIGHT_TYPE::LIGHT_DIRECTIONAL)
	{
		position[3] = 0.0f;
	}
}

void Light::SetAmbient(float r, float g, float b, float a)
{
	ambient[0] = r;
	ambient[1] = g;
	ambient[2] = b;
	ambient[3] = a;

	glLightfv(lightNum, GL_AMBIENT, ambient);
}

void Light::SetDiffuse(float r, float g, float b, float a)
{
	diffuse[0] = r;
	diffuse[1] = g;
	diffuse[2] = b;
	diffuse[3] = a;

	glLightfv(lightNum, GL_DIFFUSE, diffuse);
}

void Light::SetSpecular(float r, float g, float b, float a)
{
	specularity[0] = r;
	specularity[1] = g;
	specularity[2] = b;
	specularity[3] = a;

	glLightfv(lightNum, GL_SPECULAR, specularity);
}

void Light::SetCutoff(float value)
{
	cutoff = value;
	glLightf(lightNum, GL_SPOT_CUTOFF, cutoff);
}

void Light::SetExponent(float value)
{
	exponent = value;
	glLightf(lightNum, GL_SPOT_EXPONENT, exponent);
}

void Light::SetAttenuation(float constant, float linear, float quadratic)
{
	glLightf(lightNum, GL_CONSTANT_ATTENUATION, constant);
	glLightf(lightNum, GL_LINEAR_ATTENUATION, linear);
	glLightf(lightNum, GL_QUADRATIC_ATTENUATION, quadratic);
}

int Light::GetLightNum(void)
{
	return lightNum;
}
