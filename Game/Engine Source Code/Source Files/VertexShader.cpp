#include "..\Header Files\VertexShader.h"
#include "EngineCore.h"

#include <d3dcompiler.h>

#pragma comment(lib, "D3DCompiler.lib")

namespace wrl = Microsoft::WRL;

VertexShader::VertexShader(const wchar_t* fileName)
{
	D3DReadFileToBlob(fileName, &EngineCore::graphics->vsBlob);
	EngineCore::graphics->pDevice->CreateVertexShader(EngineCore::graphics->vsBlob->GetBufferPointer(), EngineCore::graphics->vsBlob->GetBufferSize(), nullptr, &pVertexShader);
	EngineCore::graphics->pContext->VSSetShader(pVertexShader.Get(), nullptr, 0u);
}

VertexShader::~VertexShader()
{
}

void VertexShader::Release()
{
	pVertexShader->Release();
	delete this;
}
