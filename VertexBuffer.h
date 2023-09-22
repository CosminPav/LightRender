#pragma once
#include <d3d11.h>
#include "Prerequisites.h"

class VertexBuffer
{
	//FRIENDS
	friend class DeviceContext;


	UINT mSizeVetex{ 0 };
	UINT mSizeList{ 0 };

	//Buffer
	ID3D11Buffer* mBuffer = nullptr;
	ID3D11InputLayout* mInLayout = nullptr;

	//Render System ref
	RenderSystem* mSystem = nullptr;
public:
	VertexBuffer(void* ListVertices, UINT SizeVertex, UINT ListSize, void* ShaderByteCode, UINT SizeShderByte, RenderSystem* System);

	inline UINT GetVertexSize() const { return mSizeList; }

	~VertexBuffer();
};

