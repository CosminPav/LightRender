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

	void ClearRenderTargetColor(const SwapChainPtr& SwapChain, float Red, float Green, float Blue, float Alpha);

	//Vertex buffer
	void SetVertexBuffer(const VertexBufferPtr& Buffer);

	//Index buffer
	void SetIndexBuffer(const IndexBufferPtr& Buffer);

	//Constant buffer
	void SetConstantBuffer(const VertexShaderPtr& vertexShader,const ConstantBufferPtr& constantBuffer);
	void SetConstantBuffer(const PixelShaderPtr& pixelShader, const ConstantBufferPtr& constantBuffer);

	//Texture coord
	void SetTexture(const VertexShaderPtr& vertexShader, const TexturePtr& Texture);
	void SetTexture(const PixelShaderPtr& pixelShader, const TexturePtr& Texture);


	//VertexShdaer
	void SetVertexShader(const VertexShaderPtr& vertexShader);

	//Pixel Shader
	void SetPixelShader(const PixelShaderPtr& pixelShader);

	//
	void DrawTriangleList(UINT VertexCount, UINT VertexStartIndex);
	void DrawIndexTriangleList(UINT IndexCount, UINT StartVertrexIndex, UINT StartIndexLocation);

	void DrawTriangleStrip(UINT VertexCount, UINT VertexStartIndex);
	void SetViewportSize(UINT Width, UINT Height);

	~DeviceContext();
};

