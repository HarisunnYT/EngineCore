#include "..\Header Files\SwapChain.h"
#include "Graphics.h"

namespace wrl = Microsoft::WRL;

SwapChain::SwapChain(const DXGI_SWAP_CHAIN_DESC& sd, wrl::ComPtr<ID3D11RenderTargetView>& pTarget)
{
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
		&EngineCore::graphics->pDevice,
		nullptr,
		&EngineCore::graphics->pContext);

	wrl::ComPtr<ID3D11Resource> pBackBuffer;
	pSwap->GetBuffer(0, __uuidof(ID3D11Resource), &pBackBuffer);
	EngineCore::graphics->pDevice->CreateRenderTargetView(pBackBuffer.Get(), nullptr, &pTarget);

	pBackBuffer->Release();
}

void SwapChain::Release()
{
	pSwap->Release();
	delete this;
}

void SwapChain::Present()
{
	pSwap->Present(1u, 0u);
}
