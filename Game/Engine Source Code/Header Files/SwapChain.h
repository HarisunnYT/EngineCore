#pragma once

#ifndef SWAPCHAIN_H
#define SWAPCHAIN_H

#include <Windows.h>
#include <d3d11_4.h>
#include <wrl.h>

class SwapChain
{
public:

	SwapChain(const DXGI_SWAP_CHAIN_DESC& sd, Microsoft::WRL::ComPtr<ID3D11RenderTargetView>& pTarget);

	void Release();
	void Present();

private:

	IDXGISwapChain* pSwap = nullptr;

};

#endif