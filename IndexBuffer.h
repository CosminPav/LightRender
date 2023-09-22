#pragma once
#include <d3d11.h>
#include "Prerequisites.h"

class IndexBuffer
{
	//friends
	friend class DeviceContext;

	UINT mSizeList{ 0 };
	ID3D11Buffer* mBuffer = nullptr;

	//Render System ref
	RenderSystem* mSystem = nullptr;

public:
	IndexBuffer(void* ListIndecies, UINT ListSize, RenderSystem* System);

	inline UINT GetSizeIndexList()const { return mSizeList; }

	~IndexBuffer();
};

