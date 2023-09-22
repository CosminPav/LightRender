#pragma once
#include <d3d11.h>
#include "Prerequisites.h"

class ConstantBuffer
{
	//friends
	friend class DeviceContext;

	ID3D11Buffer* mBuffer = nullptr;

	//Render System ref
	RenderSystem* mSystem = nullptr;
public:
	ConstantBuffer(void* Buffer, UINT BufferSize, RenderSystem* System);

	void Update(DeviceContextPtr deviceContext, void* Buffer);

	~ConstantBuffer();
};

