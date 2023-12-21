#pragma once
#include "Resource.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include <vector>

#include "Vector2D.h"
#include "Vector3D.h"


struct MaterialSlot
{
	size_t StartIndex{ 0 };
	size_t NumIndices{ 0 };
	size_t MaterialId{ 0 };
};

class Mesh : public Resource
{
	friend class DeviceContext;

	VertexBufferPtr mVertexBuffer;
	IndexBufferPtr mIndexBuffer;

	std::vector<MaterialSlot> MaterialSlots;
public:
	Mesh(const wchar_t* FullPath);

	inline const VertexBufferPtr& GetVertexBuffer() { return mVertexBuffer; }
	inline const IndexBufferPtr& GetIndexBuffer() { return mIndexBuffer; }

	inline const MaterialSlot& GetMaterialSlot(unsigned int Slot) const { return (Slot >= MaterialSlots.size()) ?  MaterialSlot() :  MaterialSlots[Slot]; }
	inline size_t GetNumMaterials() const { return MaterialSlots.size(); }
	~Mesh();

private:
	void ComputeTangents(
		const Math::Vector3D& V0, const Math::Vector3D& V1, const Math::Vector3D& V2,
		const Math::Vector2D& T0, const Math::Vector2D& T1, const Math::Vector2D& T2,
		Math::Vector3D& Tangent, Math::Vector3D& Bitangent);
};

