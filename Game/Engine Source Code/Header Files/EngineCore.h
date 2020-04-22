#ifndef ENGINECORE_H
#define ENGINECORE_H

//#include "3rdPartyLibs/Includes/GL/glew.h"
#include "SDL.h"
#include "SDL_opengl.h"
#include "SDL_image.h"

#include "Vector3.h"
#include "Vector2.h"
#include "Collision.h"
#include "Physics.h"
#include "Components.h"
#include "Hit.h"

#include <stdio.h>

class Camera;
class ECS;
class AudioListener;
class EngineCore
{
public:

	EngineCore();
	~EngineCore();

	void Init(const char* title, int xpos, int ypos, bool fullscreen);

	void HandleEvents();
	void Update();
	void Render();
	void Clean();

	bool Running();

	static SDL_Renderer*	Renderer;
	static SDL_Event		Event;
	static SDL_GLContext	mainContext;
	static ECS*				Ecs;
	static Camera*			camera;
	static AudioListener*	audioListener;

	static bool				isRunning;
	static bool				isDebug;

	static Vector2			screenSize;

	static float			deltaTime;
	static float			fixedTimeStep;

private:

	void EstablishProjectionMatrix();
	void SetOrthographic();

	float nearClippingPlane = 0.1f;
	float farClippingPlane = 500.0f;

	SDL_Window*				window;
};

#endif
