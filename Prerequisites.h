#pragma once
#include <memory>

class SwapChain;
class DeviceContext;
class VertexBuffer;
class ConstantBuffer;
class VertexShader;
class PixelShader;
class IndexBuffer;
class RenderSystem;
class GraphicsEngine;

class Resource;
class ResourceManager;
class Texture;
class TextureManager;

class Mesh;
class MeshManager;



class Rect;
class Game_Master;
class Material;

//using SwapChainPtr = std::unique_ptr<SwapChain>;

typedef std::shared_ptr<SwapChain> SwapChainPtr;
typedef std::shared_ptr<DeviceContext> DeviceContextPtr;
typedef std::shared_ptr<VertexBuffer> VertexBufferPtr;
typedef std::shared_ptr<ConstantBuffer> ConstantBufferPtr;
typedef std::shared_ptr<VertexShader> VertexShaderPtr;
typedef std::shared_ptr<PixelShader> PixelShaderPtr;
typedef std::shared_ptr<IndexBuffer> IndexBufferPtr;
typedef std::shared_ptr<RenderSystem> RenderSystemPtr;
typedef std::shared_ptr<GraphicsEngine> GraphicsEnginePtr;

typedef std::shared_ptr<Resource> ResourcePtr;
typedef std::shared_ptr<Texture>TexturePtr;

typedef std::shared_ptr<Mesh> MeshPtr;

typedef std::shared_ptr<Material> MaterialPtr;

typedef std::shared_ptr<Rect> RectPtr;


#define CROC_ERROR(message)
{
\
std::wcout<<throw std::runtime_error("");
/
}
