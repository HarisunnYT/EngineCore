#pragma once

#ifndef CONSTANTBUFFER_H
#define CONSTANTBUFFER_H

#include <Windows.h>
#include <d3d11_4.h>
#include <wrl.h>

class DeviceContext;
class ConstantBuffer
{
public:

	ConstantBuffer();

	void Load(void* buffer, UINT size_buffer);
	void Update(void* buffer);

	void Release();

private:

	ID3D11Buffer* pBuffer = nullptr;

	friend class DeviceContext;
};

#endif