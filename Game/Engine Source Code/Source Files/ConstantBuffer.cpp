#include "..\Header Files\ConstantBuffer.h"
#include "EngineCore.h"

ConstantBuffer::ConstantBuffer()
{
}

void ConstantBuffer::Load(void* buffer, UINT size_buffer)
{
	if (pBuffer) pBuffer->Release();

	D3D11_BUFFER_DESC bd = {};
	bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.CPUAccessFlags = 0u;
	bd.MiscFlags = 0u;
	bd.ByteWidth = size_buffer;

	D3D11_SUBRESOURCE_DATA sd = {};
	sd.pSysMem = buffer;

	EngineCore::graphics->pDevice->CreateBuffer(&bd, &sd, &pBuffer);
}

void ConstantBuffer::Update(void* buffer)
{
	EngineCore::graphics->pContext->UpdateSubresource(pBuffer, NULL, NULL, buffer, NULL, NULL);
}

void ConstantBuffer::Release()
{
	if (pBuffer) pBuffer->Release();
	delete this;
}
