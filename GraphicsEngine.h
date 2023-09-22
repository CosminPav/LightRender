#pragma once
#include <d3d11.h>
#include "Prerequisites.h"
#include <memory>

class GraphicsEngine
{
	//std::unique_ptr<RenderSystem> mSystemPtr;
	RenderSystem* mSystem = nullptr;
	static GraphicsEngine* mEngine;

	//Default constructor
	GraphicsEngine();

	//Default destructor
	~GraphicsEngine() noexcept;
public:
	inline RenderSystem* GetRenderSystem() const { return mSystem; }

	static GraphicsEngine* Get();
	static void Create();
	static void Release();


};

