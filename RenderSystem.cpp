#include "RenderSystem.h"
#include "SwapChain.h"
#include "DeviceContext.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "ConstantBuffer.h"
#include "VertexShader.h"
#include "PixelShader.h"
#include <d3dcompiler.h>

#include <iostream>

RenderSystem::RenderSystem()
{
    //Driver types
    D3D_DRIVER_TYPE DriverTypes[] =
    {
        D3D_DRIVER_TYPE_HARDWARE,
        D3D_DRIVER_TYPE_WARP,
        D3D_DRIVER_TYPE_REFERENCE
    };

    UINT DTNum = ARRAYSIZE(DriverTypes);

    //Feature level
    D3D_FEATURE_LEVEL FeatureLevels[] =
    {
        D3D_FEATURE_LEVEL_11_0
    };

    UINT FLNum = ARRAYSIZE(FeatureLevels);

    HRESULT Res = 0;

    //DTIndex = DriverTypeIndex
    for (UINT DTIndex = 0; DTIndex < DTNum;) {
        Res = D3D11CreateDevice(NULL, DriverTypes[DTIndex], NULL, NULL, FeatureLevels, FLNum, D3D11_SDK_VERSION, &d3dDevice, &mFeatureLevel,
            &mImmContext);
        if (SUCCEEDED(Res)) {
            break;
            ++DTIndex;
        }
    }
    //exepction
    if (FAILED(Res))
        throw std::exception("Render System failed to load");
        

    mDeviceContext = std::make_shared<DeviceContext>(mImmContext, this);
    //DeviceContext_UPtr = std::make_unique<DeviceContext>(mImmDeviceContext);

    d3dDevice->QueryInterface(__uuidof(IDXGIDevice), (void**)&dxgiDevice);
    //d3dDevice.As(&dxgiDevice);
    dxgiDevice->GetParent(__uuidof(IDXGIAdapter), (void**)&dxgiAdapter);
    //dxgiDevice.As(&dxgiAdapter);
    dxgiAdapter->GetParent(__uuidof(IDXGIFactory), (void**)&dxgiFactory);
    //dxgiAdapter.As(&dxgiFactory);

    InitRasterizerState();
}

//custom vertex shader

bool RenderSystem::CompileVertexShader(const wchar_t* FileName, const char* EntryPointName, void** ShaderByteCode, size_t* ByteCodeSize)
{
    ID3DBlob* ErrorBlob = nullptr;

    if (FAILED(D3DCompileFromFile(FileName, nullptr, nullptr, EntryPointName, "vs_5_0", 0, 0, &mBlob, &ErrorBlob))) {
        if (ErrorBlob)
            ErrorBlob->Release();
        return false;
    }
    *ShaderByteCode = mBlob->GetBufferPointer();
    *ByteCodeSize = mBlob->GetBufferSize();

    return true;
}

void RenderSystem::ReleaseCompiledShaders()
{
    if (mBlob)
        mBlob->Release();
}

//Custom pixel shader
bool RenderSystem::CompilePixelShader(const wchar_t* FileName, const char* EntryPointName, void** SharedByteCode, size_t* ByteCodeSize)
{
    ID3DBlob* ErrorBlob = nullptr;

    if (FAILED(D3DCompileFromFile(FileName, nullptr, nullptr, EntryPointName, "ps_5_0", 0, 0, &mBlob, &ErrorBlob))) {
        if (ErrorBlob)
            ErrorBlob->Release();
        return false;
    }
    *SharedByteCode = mBlob->GetBufferPointer();
    *ByteCodeSize = mBlob->GetBufferSize();

    return true;
}
//swap chain

void RenderSystem::InitRasterizerState()
{
    //Front face culling
    D3D11_RASTERIZER_DESC desc = {};
    desc.CullMode = D3D11_CULL_FRONT;
    desc.DepthClipEnable = true;
    desc.FillMode = D3D11_FILL_SOLID;

    d3dDevice->CreateRasterizerState(&desc, &mCullFrontState);

    //Back face culling
    desc.CullMode = D3D11_CULL_BACK;
    d3dDevice->CreateRasterizerState(&desc, &mCullBackState);
}

SwapChainPtr RenderSystem::MakeSwapChain(HWND hwnd, UINT Width, UINT Height)
{
    SwapChainPtr Sc = nullptr;
    try {
        Sc = std::make_shared<SwapChain>(hwnd, Width, Height, this);
    }
    catch (...) {

    }
    return Sc;
}

//vertex buffer
VertexBufferPtr RenderSystem::CreateVertexBuffer(void* ListVertices, UINT SizeVertex, UINT ListSize, void* ShaderByteCode, UINT SizeShderByte)
{
    VertexBufferPtr Vb = nullptr;
    try {
        Vb = std::make_shared <VertexBuffer>(ListVertices, SizeVertex, ListSize, ShaderByteCode, SizeShderByte, this);
    }
    catch (...) {
    }

    return Vb;
}

ConstantBufferPtr RenderSystem::MakeConstantBuffer(void* Buffer, UINT BufferSize)
{
    ConstantBufferPtr Cb = nullptr;
    try {
        Cb = std::make_shared< ConstantBuffer>(Buffer, BufferSize, this);
    }
    catch (...) {

    }
    return Cb;
}

IndexBufferPtr RenderSystem::MakeIndexBuffer(void* ListIndecies, UINT ListSize)
{
    IndexBufferPtr Ib = nullptr;
    try {
        Ib = std::make_shared< IndexBuffer>(ListIndecies, ListSize, this);
    }
    catch (...) {

    }
    return Ib;
}

// vertex shader
VertexShaderPtr RenderSystem::MakeVertexShader(const void* SharedByteCode, size_t ByteCodeLength)
{
    VertexShaderPtr vertexShader = nullptr;
    try {
        vertexShader = std::make_shared< VertexShader>(SharedByteCode, ByteCodeLength, this);
    }
    catch (...) {

    }
  
    return vertexShader;
}

//pixel shader
PixelShaderPtr RenderSystem::MakePixelShader(const void* SharedByteCode, size_t ByteCodeLength)
{
    PixelShaderPtr pixelShader = nullptr;
    try {
        pixelShader = std::make_shared< PixelShader>(SharedByteCode, ByteCodeLength, this);
    }
    catch (...){

    }
    return pixelShader;
}

void RenderSystem::SetRazterizerState(bool bCullFront)
{
    if (bCullFront) 
        mImmContext->RSSetState(mCullFrontState);
    else 
        mImmContext->RSSetState(mCullBackState); 
    
}

RenderSystem::~RenderSystem()
{
    if (mVsBlob)
        mVsBlob->Release();
    if (mPsBlob)
        mPsBlob->Release();

    dxgiDevice->Release();
    dxgiAdapter->Release();
    dxgiFactory->Release();
    d3dDevice->Release();
}
