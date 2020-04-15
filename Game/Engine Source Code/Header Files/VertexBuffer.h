#pragma once

#ifndef VERTEXBUFFER_H
#define VERTEXBUFFER_H

#include <Windows.h>
#include <d3d11_4.h>
#include <wrl.h>

class VertexBuffer
{
public:

	VertexBuffer();

	void Load(void* list_verticies, UINT size_vertex, UINT size_list, void* shader_byte_code, UINT size_byte_shader);

	void Release();

private:

	UINT size_vertex;
	UINT size_list;

	ID3D11Buffer* pBuffer = nullptr;
	ID3D11InputLayout* pInputLayout = nullptr;

	friend class DeviceContext;
};

#endif