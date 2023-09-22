#include "ConstantBuffer.h"
#include "DeviceContext.h"
#include "RenderSystem.h"
#include <exception>

ConstantBuffer::ConstantBuffer(void* Buffer, UINT BufferSize, RenderSystem* System) : mSystem(System)
{

    D3D11_BUFFER_DESC BDesc = {};
    BDesc.Usage = D3D11_USAGE_DEFAULT;
    BDesc.ByteWidth = BufferSize;
    BDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
    BDesc.CPUAccessFlags = 0;
    BDesc.MiscFlags = 0;

    D3D11_SUBRESOURCE_DATA SubData = {};
    SubData.pSysMem = Buffer;

    if (FAILED(mSystem->d3dDevice->CreateBuffer(&BDesc, &SubData, &mBuffer))) {
        throw std::exception ("Constant buffer failed to load");
    }
}

void ConstantBuffer::Update(DeviceContextPtr deviceContext, void* Buffer)
{
    if (deviceContext.get())
    {
        deviceContext->mDeviceContext->UpdateSubresource(mBuffer, NULL, NULL, Buffer, NULL, NULL);
    }
}

ConstantBuffer::~ConstantBuffer()
{ 
    if (mBuffer)
        mBuffer->Release();
}
