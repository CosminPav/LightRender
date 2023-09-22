#include "GraphicsEngine.h"
#include "RenderSystem.h"
#include <exception>

GraphicsEngine* GraphicsEngine::mEngine = nullptr;

GraphicsEngine::GraphicsEngine()
{
    try {
        mSystem = new RenderSystem();
    }
    catch (...) {
        throw std::exception("Render system failed to create");
    }
   
}

GraphicsEngine* GraphicsEngine::Get()
{
    return mEngine;
}

void GraphicsEngine::Create()
{
    if (GraphicsEngine::mEngine)
        throw std::exception("Graphics Engine already exists");
    GraphicsEngine::mEngine = new GraphicsEngine();
}

void GraphicsEngine::Release()
{
    if (!GraphicsEngine::mEngine)
        return;
    delete GraphicsEngine::mEngine;
}

GraphicsEngine::~GraphicsEngine() noexcept
{
   delete mSystem;
}


