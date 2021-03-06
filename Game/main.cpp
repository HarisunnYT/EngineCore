#include "EngineCore.h"
#include "InputSystem.h"

#include <time.h>

#pragma comment(lib, "opengl32.lib")
#pragma comment(lib, "glu32")

#include <gl/GL.h>
#include <gl/GLU.h>

EngineCore* engineCore = nullptr;

double start = 0.0;
double last = 0.0;

const int MAX_FRAME_DELTA = 3;

int main(int agrc, char* argv[])
{
	engineCore = new EngineCore();
	engineCore->Init("EngineCore", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, false);

	while (engineCore->Running())
	{
		start = clock();

		double delta = start - last;

		EngineCore::deltaTime = (float)delta / 1000;

		delta = delta > MAX_FRAME_DELTA ? MAX_FRAME_DELTA : delta;

		engineCore->HandleEvents();

		for (int i = 0; i < delta; i++)
		{
			engineCore->Update();
		}

		engineCore->Render();

		last = start;
	}

	engineCore->Clean();

	return 0;
}