#pragma once

#ifndef GRAPHICS_H
#define GRAPHICS_H

#include "EngineCore.h"

#include "SwapChain.h"
#include "DeviceContext.h"
#include "VertexBuffer.h"
#include "VertexShader.h"
#include "PixelShader.h"

#include <Windows.h>
#include <d3d11_4.h>
#include <wrl.h>

class SwapChain;
class DeviceContext;
class VertexBuffer;
class VertexShader;
class PixelShader;
class Graphics
{
public:

	Graphics();
	Graphics(const Graphics&) = delete;
	Graphics& operator=(const Graphics&) = delete;
	~Graphics();

	void Init(SDL_Window* window);

	void EndFrame();
	void DrawTestTriangle();

	ID3D11Device* pDevice = nullptr;
	ID3D11DeviceContext* pContext = nullptr;

private:

	HWND* window;

	Microsoft::WRL::ComPtr<IDXGIDevice> pDxgiDevice = nullptr;
	Microsoft::WRL::ComPtr<IDXGIFactory> pFactory = nullptr;
	Microsoft::WRL::ComPtr<IDXGIAdapter> pAdaptor = nullptr;

	SwapChain* swapChain = nullptr;
	DeviceContext* deviceContext = nullptr;
	VertexBuffer* vertexBuffer = nullptr;

	VertexShader* vertexShader = nullptr;
	PixelShader* pixelShader = nullptr;

	Microsoft::WRL::ComPtr<ID3DBlob> psBlob;
	Microsoft::WRL::ComPtr<ID3DBlob> vsBlob;

	Microsoft::WRL::ComPtr<ID3D11RenderTargetView> pTarget;

	friend class VertexShader;
	friend class PixelShader;
};

#endif