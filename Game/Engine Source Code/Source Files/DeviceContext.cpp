#include "..\Header Files\DeviceContext.h"
#include "EngineCore.h"

namespace wrl = Microsoft::WRL;

DeviceContext::~DeviceContext()
{
}

void DeviceContext::SetVertexBuffer(VertexBuffer* buffer)
{
	const UINT stride = buffer->size_vertex;
	const UINT offset = 0u;

	EngineCore::graphics->pContext->IASetVertexBuffers(0u, 1u, &buffer->pBuffer, &stride, &offset);
	EngineCore::graphics->pContext->IASetInputLayout(buffer->pInputLayout);
}

void DeviceContext::SetViewportSize()
{
	D3D11_VIEWPORT vp;
	vp.Width = EngineCore::screenSize.x;
	vp.Height = EngineCore::screenSize.y;
	vp.MinDepth = 0;
	vp.MaxDepth = 1;
	vp.TopLeftX = 0;
	vp.TopLeftY = 0;
	EngineCore::graphics->pContext->RSSetViewports(1u, &vp);
}

void DeviceContext::DrawTriangleList(UINT vertex_count, UINT start_vertex_index)
{
	EngineCore::graphics->pContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	EngineCore::graphics->pContext->Draw(vertex_count, start_vertex_index);
}

void DeviceContext::DrawTriangleStrip(UINT vertex_count, UINT start_vertex_index)
{
	EngineCore::graphics->pContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
	EngineCore::graphics->pContext->Draw(vertex_count, start_vertex_index);
}

void DeviceContext::Release()
{
	EngineCore::graphics->pContext->Release();
	delete this;
}
