#pragma once

#ifndef VERTEXSHADER_H
#define VERTEXSHADER_H

#include <Windows.h>
#include <d3d11_4.h>
#include <wrl.h>

class Graphics;
class VertexShader
{
public:

	VertexShader(const wchar_t* fileName);
	~VertexShader();

	void Release();

private:

	Microsoft::WRL::ComPtr<ID3D11VertexShader> pVertexShader;
};

#endif