#include "..\Header Files\VertexBuffer.h"
#include "EngineCore.h"

namespace wrl = Microsoft::WRL;

VertexBuffer::VertexBuffer()
{
	size_vertex = 0;
	size_list = 0;
}

void VertexBuffer::Load(void* list_verticies, UINT size_vert, UINT size_lst, void* shader_byte_code, UINT size_byte_shader)
{
	if (pBuffer) pBuffer->Release();
	if (pInputLayout) pInputLayout->Release();

	size_vertex = size_vert;
	size_list = size_lst;

	D3D11_BUFFER_DESC bd = {};
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.CPUAccessFlags = 0u;
	bd.MiscFlags = 0u;
	bd.ByteWidth = size_vertex * size_list;

	D3D11_SUBRESOURCE_DATA sd = {};
	sd.pSysMem = list_verticies;

	EngineCore::graphics->pDevice->CreateBuffer(&bd, &sd, &pBuffer);

	const D3D11_INPUT_ELEMENT_DESC ied[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "COLOR", 0, DXGI_FORMAT_R8G8B8A8_UNORM, 0, 8u, D3D11_INPUT_PER_VERTEX_DATA, 0 }
	};

	EngineCore::graphics->pDevice->CreateInputLayout(ied, (UINT)std::size(ied), shader_byte_code, size_byte_shader, &pInputLayout);
}

void VertexBuffer::Release()
{
	pInputLayout->Release();
	pBuffer->Release();

	delete this;
}
