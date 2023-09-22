#pragma once
#include <d3d11.h>
#include <Windows.h>
#include "Prerequisites.h"


class DeviceContext 
{
	//ComPtr<ID3D11DeviceContext> mDeviceContext;
	//Friends
	friend class ConstantBuffer;

	ID3D11DeviceContext* mDeviceContext = nullptr;

	//Render System ref
	RenderSystem* mSystem = nullptr;

public:
	DeviceContext(ID3D11DeviceContext* DeviceContext, RenderSystem* System); 

	void ClearRenderTargetColor(SwapChainPtr SwapChain, float Red, float Green, float Blue, float Alpha);

	//Vertex buffer
	void SetVertexBuffer(VertexBufferPtr Buffer);

	//Index buffer
	void SetIndexBuffer(IndexBufferPtr Buffer);

	//Constant buffer
	void SetConstantBuffer(VertexShaderPtr vertexShader, ConstantBufferPtr constantBuffer);
	void SetConstantBuffer(PixelShaderPtr pixelShader, ConstantBufferPtr constantBuffer);


	//VertexShdaer
	void SetVertexShader(VertexShaderPtr vertexShader);

	//Pixel Shader
	void SetPixelShader(PixelShaderPtr pixelShader);

	//
	void DrawTriangleList(UINT VertexCount, UINT VertexStartIndex);
	void DrawIndexTriangleList(UINT IndexCount, UINT StartVertrexIndex, UINT StartIndexLocation);

	void DrawTriangleStrip(UINT VertexCount, UINT VertexStartIndex);
	void SetViewportSize(UINT Width, UINT Height);

	~DeviceContext();
};

