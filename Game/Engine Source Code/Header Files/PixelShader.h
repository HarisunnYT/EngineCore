#pragma once

#ifndef PIXELSHADER_H
#define PIXELSHADER_H

#include <Windows.h>
#include <d3d11_4.h>
#include <wrl.h>

class Graphics;
class PixelShader
{
public:

	PixelShader(const wchar_t* fileName);
	~PixelShader();

	void Release();

private:

	Microsoft::WRL::ComPtr<ID3D11PixelShader> pPixelShader;
};

#endif