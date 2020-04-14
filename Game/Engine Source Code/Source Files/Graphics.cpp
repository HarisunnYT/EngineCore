#include "..\Header Files\Graphics.h"

#include <SDL.h>
#include <SDL_syswm.h>

#include <d3dcompiler.h>

#pragma comment(lib, "D3DCompiler.lib")

namespace wrl = Microsoft::WRL;

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
	sd.BufferDesc.Width = EngineCore::screenSize.x;
	sd.BufferDesc.Height = EngineCore::screenSize.y;
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

	wrl::ComPtr<ID3D11Resource> pBackBuffer;
	pSwap->GetBuffer(0, __uuidof(ID3D11Resource), &pBackBuffer);
	pDevice->CreateRenderTargetView(pBackBuffer.Get(), nullptr, &pTarget);

	pBackBuffer->Release();
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

void Graphics::EndFrame()
{
	pSwap->Present(1u, 0u);
}

void Graphics::DrawTestTriangle()
{
	struct Vertex
	{
		float x;
		float y;
	};

	const Vertex verticies[] =
	{
		{ 0.0f,  0.5f},
		{ 0.5f, -0.5f},
		{-0.5f, -0.5f},
		{ 0.0f,  0.5f}
	};

	wrl::ComPtr<ID3D11Buffer> pVertexBuffer;
	D3D11_BUFFER_DESC bd = {};
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.CPUAccessFlags = 0u;
	bd.MiscFlags = 0u;
	bd.ByteWidth = sizeof(verticies);
	bd.StructureByteStride = sizeof(Vertex);

	D3D11_SUBRESOURCE_DATA sd = {};
	sd.pSysMem = verticies;

	pDevice->CreateBuffer(&bd, &sd, &pVertexBuffer);

	const UINT stride = sizeof(Vertex);
	const UINT offset = 0u;

	pContext->IASetVertexBuffers(0u, 1u, pVertexBuffer.GetAddressOf(), &stride, &offset);

	wrl::ComPtr<ID3D11PixelShader> pPixelShader;
	wrl::ComPtr<ID3DBlob> pBlob;
	D3DReadFileToBlob(L"PixelShader.cso", &pBlob);
	pDevice->CreatePixelShader(pBlob->GetBufferPointer(), pBlob->GetBufferSize(), nullptr, &pPixelShader);
	pContext->PSSetShader(pPixelShader.Get(), nullptr, 0u);

	wrl::ComPtr<ID3D11VertexShader> pVertexShader;
	D3DReadFileToBlob(L"VertexShader.cso", &pBlob);
	pDevice->CreateVertexShader(pBlob->GetBufferPointer(), pBlob->GetBufferSize(), nullptr, &pVertexShader);
	pContext->VSSetShader(pVertexShader.Get(), nullptr, 0u);

	wrl::ComPtr<ID3D11InputLayout> pInputLayout;
	const D3D11_INPUT_ELEMENT_DESC ied[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 }
	};

	pDevice->CreateInputLayout(ied, (UINT)std::size(ied), pBlob->GetBufferPointer(), pBlob->GetBufferSize(), &pInputLayout);

	pContext->IASetInputLayout(pInputLayout.Get());

	pContext->OMSetRenderTargets(1u, pTarget.GetAddressOf(), nullptr);

	pContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINESTRIP);

	D3D11_VIEWPORT vp;
	vp.Width = 800;
	vp.Height = 600;
	vp.MinDepth = 0;
	vp.MaxDepth = 1;
	vp.TopLeftX = 0;
	vp.TopLeftY = 0;
	pContext->RSSetViewports(1u, &vp);

	pContext->Draw((UINT)std::size(verticies), 0u);
}
