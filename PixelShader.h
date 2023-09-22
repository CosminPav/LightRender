#pragma once
#include <d3d11.h>
#include "Prerequisites.h"

class PixelShader
{
	//friends
	friend class DeviceContext;
	friend class RenderSystem;

	//Pixel shader inteface class
	ID3D11PixelShader* mPixelShader = nullptr;

	//Render System ref
	RenderSystem* mSystem = nullptr;

public:
	PixelShader(const void* ShaderByteCode, size_t ShaderCodeSize, RenderSystem* System);

	~PixelShader();
};

