#pragma once

#ifndef DEVICECONTEXT_H
#define DEVICECONTEXT_H

#include <Windows.h>
#include <d3d11_4.h>
#include <wrl.h>

class VertexBuffer;
class DeviceContext
{
public:

	~DeviceContext();

	void SetVertexBuffer(VertexBuffer* buffer);
	void SetViewportSize();

	void DrawTriangleList(UINT vertex_count, UINT start_vertex_index);
	void DrawTriangleStrip(UINT vertex_count, UINT start_vertex_index);

	void Release();

private:

	friend class ConstantBuffer;
	friend class Graphics;

};

#endif