#include "VertexBuffer.h"
#include  "RenderSystem.h"
#include <exception>

#include <iostream>

VertexBuffer::VertexBuffer(void* ListVertices, UINT SizeVertex, UINT ListSize, void* ShaderByteCode, UINT SizeShderByte, RenderSystem* System) :mBuffer(0), mInLayout(0), mSystem(System)
{
    D3D11_BUFFER_DESC BDesc = {};
    BDesc.Usage = D3D11_USAGE_DEFAULT;
    BDesc.ByteWidth = SizeVertex * ListSize;
    BDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    BDesc.CPUAccessFlags = 0;
    BDesc.MiscFlags = 0;

    D3D11_SUBRESOURCE_DATA SubData = {};
    SubData.pSysMem = ListVertices;

    mSizeVetex = SizeVertex;
    mSizeList = ListSize;

    //Create the vertex buffer
     if (FAILED(mSystem->d3dDevice->CreateBuffer(&BDesc, &SubData, &mBuffer))) {
         throw std::exception("Vertex Buffer failed to load");
     }
    
 
       
   
    D3D11_INPUT_ELEMENT_DESC IDesc[] =
    {
        //Semantic name         SemanticIndex           Format                      InputSlot    AlignedByteOffset      InputSlotClass          InstanceDataStepRate
        {"POSITION",                0,          DXGI_FORMAT_R32G32B32_FLOAT,            0,              0,          D3D11_INPUT_PER_VERTEX_DATA,          0},
        {"TEXCOORD",                0,          DXGI_FORMAT_R32G32_FLOAT,               0,              12,         D3D11_INPUT_PER_VERTEX_DATA,          0},
        {"NORMAL",                  0,          DXGI_FORMAT_R32G32B32_FLOAT,            0,              20,         D3D11_INPUT_PER_VERTEX_DATA,          0}
    };
    
    //layout size
    UINT LayoutSize = ARRAYSIZE(IDesc);

    //Create the input layout
    if (FAILED(mSystem->d3dDevice->CreateInputLayout(IDesc, LayoutSize, ShaderByteCode, SizeShderByte, &mInLayout))) {
        throw std::exception("Input Layout failed to load");
    }
}


VertexBuffer::~VertexBuffer()
{ 
    if(mInLayout)
        mInLayout->Release();
    mBuffer->Release();
}
