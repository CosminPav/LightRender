#include "GraphicsEngine.h"
#include "RenderSystem.h"
#include <exception>

GraphicsEngine* GraphicsEngine::mEngine = nullptr;

GraphicsEngine::GraphicsEngine()
{
    //create the render system
    try {
        mSystem = new RenderSystem();
    }
    catch (...) {
        throw std::exception("Render system failed to create");
    }
   
    //create the texture manager
    try {
        mTextureManager = new TextureManager();
    }
    catch (...) {
        throw std::exception("Texture Manager failed to create");
    }

    //Create the mesh manager
    try {
        mMeshManager = new MeshManager();
    }
    catch (...) {
        throw std::exception("Mesh Manager failed to create");
    }

    void* ShaderByteCode = nullptr;
    //the zize if mem buffer in bytes
    size_t SizeOfShader = 0;

    //VERTEX MESH SHADER

    //compile the vertex shader from file
    mSystem->CompileVertexShader(L"VertexMeshLayout.hlsl", "vsmain", &ShaderByteCode, &SizeOfShader);
    memcpy(MeshLayoutByteCode, ShaderByteCode, SizeOfShader);
    MeshLayoutSize = SizeOfShader;
    mSystem->ReleaseCompiledShaders();
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

void GraphicsEngine::GetVertexMeshLayoutSharedByteCodeAndSize(void** ByteCode, size_t* size)
{
    *ByteCode = MeshLayoutByteCode;
    *size = MeshLayoutSize;
}

GraphicsEngine::~GraphicsEngine() noexcept
{
    GraphicsEngine::mEngine = nullptr;
    delete mMeshManager;
    delete mTextureManager;
    delete mSystem;
}


