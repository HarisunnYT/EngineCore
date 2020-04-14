#include "..\Header Files\Graphics.h"

#include <SDL.h>
#include <SDL_syswm.h>

#include <wrl.h>
#include <d3dcompiler.h>

#pragma comment(lib, "D3DCompiler.lib")

Graphics::Graphics(SDL_Window* w)
{
	SDL_SysWMinfo wminfo;
	SDL_version v;

	SDL_VERSION(&v);

	wminfo.version = v;

	SDL_GetWindowWMInfo(w, &wminfo);
	window = &wminfo.info.win.window;

	DXGI_SWAP_CHAIN_DESC sd = {};
	ZeroMemory(&sd, sizeof(sd));
	sd.BufferCount = 1;
	sd.BufferDesc.Width = 0;
	sd.BufferDesc.Height = 0;
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

	D3D11CreateDeviceAndSwapChain(
		nullptr,
		D3D_DRIVER_TYPE_HARDWARE,
		nullptr,
		0,
		nullptr,
		0,
		D3D11_SDK_VERSION,
		&sd,
		&pSwap,
		&pDevice,
		nullptr,
		&pContext);
}

Graphics::~Graphics()
{
	if (pContext != nullptr)
		pContext->Release();
	if (pSwap != nullptr)
		pSwap->Release();
	if (pDevice != nullptr)
		pDevice->Release();
}

void Graphics::DrawTestTriangle()
{
	namespace wrl = Microsoft::WRL;

	struct Vertex
	{
		float x;
		float y;
	};

	const Vertex verticies[] =
	{
		{ 0.0f,  0.5f},
		{ 0.5f, -0.5f},
		{-0.5f, -0.5f}
	};

	D3D11_BUFFER_DESC bd = {};
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.CPUAccessFlags = 0u;
	bd.MiscFlags = 0u;
	bd.ByteWidth = sizeof(verticies);
	bd.StructureByteStride = sizeof(Vertex);

	D3D11_SUBRESOURCE_DATA sd = {};
	sd.pSysMem = verticies;

	const UINT stride = sizeof(Vertex);
	const UINT offset = 0u;

	wrl::ComPtr<ID3D11Buffer> pVertexBuffer;

	pDevice->CreateBuffer(&bd, &sd, &pVertexBuffer);
	pContext->IASetVertexBuffers(0u, 1u, &pVertexBuffer, &stride, &offset);
	pContext->Draw(3u, 0u);
}
