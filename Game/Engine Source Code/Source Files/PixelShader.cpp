#include "..\Header Files\PixelShader.h"
#include "EngineCore.h"

#include <d3dcompiler.h>

#pragma comment(lib, "D3DCompiler.lib")

namespace wrl = Microsoft::WRL;

PixelShader::PixelShader(const wchar_t* fileName)
{
	D3DReadFileToBlob(fileName, &EngineCore::graphics->psBlob);
	EngineCore::graphics->pDevice->CreatePixelShader(EngineCore::graphics->psBlob->GetBufferPointer(), EngineCore::graphics->psBlob->GetBufferSize(), nullptr, &pPixelShader);
	EngineCore::graphics->pContext->PSSetShader(pPixelShader.Get(), nullptr, 0u);
}

PixelShader::~PixelShader()
{
}

void PixelShader::Release()
{
	pPixelShader->Release();
	delete this;
}
