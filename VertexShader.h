#pragma once
#include <d3d11.h>
#include "Prerequisites.h"

class VertexShader
{
	//friends 
	friend class DeviceContext;
	friend class RenderSystem;

	//Vertex shader inteface
	ID3D11VertexShader* mvertexShader = nullptr;

	//Render system ref
	RenderSystem* mSystem = nullptr;

public:
	VertexShader(const void* ShaderByteCode, size_t ShaderByteCodeLength, RenderSystem* System);

	~VertexShader();
};

