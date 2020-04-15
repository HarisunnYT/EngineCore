#include "..\Header Files\Graphics.h"

#include <SDL.h>
#include <SDL_syswm.h>

namespace wrl = Microsoft::WRL;

#include <d3dcompiler.h>

#pragma comment(lib, "D3DCompiler.lib")

Graphics::Graphics()
{
}

Graphics::~Graphics()
{
	if (pContext != nullptr)
		pContext->Release();
	if (pDevice != nullptr)
		pDevice->Release();

	swapChain->Release();
	deviceContext->Release();
}

void Graphics::Init(SDL_Window* w)
{
	SDL_SysWMinfo wminfo;
	SDL_version v;

	SDL_VERSION(&v);

	wminfo.version = v;

	SDL_GetWindowWMInfo(w, &wminfo);
	HWND* window = &wminfo.info.win.window;

	DXGI_SWAP_CHAIN_DESC sd = {};
	ZeroMemory(&sd, sizeof(sd));
	sd.BufferCount = 1;
	sd.BufferDesc.Width = static_cast<UINT>(EngineCore::screenSize.x);
	sd.BufferDesc.Height = static_cast<UINT>(EngineCore::screenSize.y);
	sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	sd.BufferDesc.RefreshRate.Numerator = 0;
	sd.BufferDesc.RefreshRate.Denominator = 0;
	sd.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
	sd.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	sd.OutputWindow = *window;
	sd.SampleDesc.Count = 1;
	sd.SampleDesc.Quality = 0;
	sd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	sd.Flags = 0;
	sd.Windowed = true;

	D3D11CreateDevice(nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr, 0, nullptr, 0, D3D11_SDK_VERSION, &pDevice, nullptr, &pContext);
	
	pDevice->QueryInterface(__uuidof(IDXGIDevice), (void**)&pDxgiDevice);
	pDxgiDevice->GetParent(__uuidof(IDXGIAdapter), (void**)&pAdaptor);
	pAdaptor->GetParent(__uuidof(IDXGIFactory), (void**)&pFactory);

	deviceContext = new DeviceContext();
	swapChain = new SwapChain(sd, pTarget);
	vertexBuffer = new VertexBuffer();
}

void Graphics::EndFrame()
{
	swapChain->Present();
}

void Graphics::DrawTestTriangle()
{
	struct Vertex
	{
		struct
		{
			float x, y;
		} pos;
		struct
		{
			unsigned char r, g, b, a;
		} color;
	};

	Vertex verticies[] =
	{
		{ -0.5f,  -0.5f, 255, 0, 0, 0},
		{ -0.5f,   0.5f, 0, 255, 0, 0},
		{  0.5f,  -0.5f, 0, 0, 255, 0},
		{  0.5f,   0.5f, 255, 0, 0, 0}
	};
	
	deviceContext->SetViewportSize();

	deviceContext->SetPixelShader();
	deviceContext->SetVertexShader();

	vertexBuffer->Load(verticies, sizeof(Vertex), ARRAYSIZE(verticies), deviceContext->vsBlob->GetBufferPointer(), deviceContext->vsBlob->GetBufferSize());

	deviceContext->SetVertexBuffer(vertexBuffer);

	pContext->OMSetRenderTargets(1u, pTarget.GetAddressOf(), nullptr);

	deviceContext->DrawTriangleList((UINT)std::size(verticies), 0u);
}
