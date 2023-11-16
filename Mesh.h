#pragma once
#include "Resource.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"


class Mesh : public Resource
{
	friend class DeviceContext;

	VertexBufferPtr mVertexBuffer;
	IndexBufferPtr mIndexBuffer;
public:
	Mesh(const wchar_t* FullPath);

	inline const VertexBufferPtr& GetVertexBuffer() { return mVertexBuffer; }
	inline const IndexBufferPtr& GetIndexBuffer() { return mIndexBuffer; }

	~Mesh();
};

