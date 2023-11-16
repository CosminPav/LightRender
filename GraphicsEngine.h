#pragma once
#include <d3d11.h>
#include "Prerequisites.h"
#include "TextureManager.h"
#include "MeshManager.h"
#include <memory>

class GraphicsEngine
{
	//std::unique_ptr<RenderSystem> mSystemPtr;
	RenderSystem* mSystem = nullptr;
	TextureManager* mTextureManager = nullptr;
	MeshManager* mMeshManager = nullptr;
	static GraphicsEngine* mEngine;

	unsigned char MeshLayoutByteCode[1024];
	size_t MeshLayoutSize{ 0 };

	//Default constructor
	GraphicsEngine();

	//Default destructor
	~GraphicsEngine() noexcept;
public:
	inline RenderSystem* GetRenderSystem() const { return mSystem; }
	inline TextureManager* GetTextureManager() const { return mTextureManager; }
	inline MeshManager* GetMeshManager() const { return mMeshManager; }

	void GetVertexMeshLayoutSharedByteCodeAndSize(void** ByteCode, size_t* size);

	static GraphicsEngine* Get();
	static void Create();
	static void Release();


};

