#pragma once

#ifndef GRAPHICS_H
#define GRAPHICS_H

#include "EngineCore.h"

#include <Windows.h>
#include <d3d11_4.h>

class Graphics
{
public:

	Graphics(SDL_Window* window);
	Graphics(const Graphics&) = delete;
	Graphics& operator=(const Graphics&) = delete;
	~Graphics();

	void DrawTestTriangle();

private:

	HWND* window;

	IDXGISwapChain* pSwap = nullptr;
	ID3D11Device* pDevice = nullptr;
	ID3D11DeviceContext* pContext = nullptr;

};

#endif