#include "IndexBuffer.h"
#include "RenderSystem.h"
#include <exception>

IndexBuffer::IndexBuffer(void* ListIndecies, UINT ListSize, RenderSystem* System) : mSystem(System)
{
    if (mBuffer)
        mBuffer->Release();

    D3D11_BUFFER_DESC BDesc = {};
    BDesc.Usage = D3D11_USAGE_DEFAULT;
    BDesc.ByteWidth = 4 * ListSize;
    BDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;// D3D11_BIND_INDEX_BUFFER;
    BDesc.CPUAccessFlags = 0;
    BDesc.MiscFlags = 0;

    D3D11_SUBRESOURCE_DATA SubData = {};
    SubData.pSysMem = ListIndecies;

    mSizeList = ListSize;

    //Create the vertex buffer
    if (FAILED(mSystem->d3dDevice->CreateBuffer(&BDesc, &SubData, &mBuffer))) {
        throw std::exception("Index Buffer failed to load");
    }
}

IndexBuffer::~IndexBuffer()
{  
    if (mBuffer)
        mBuffer->Release(); 
}
