#include "DeviceContext.h"
#include "SwapChain.h"
#include "VertexBuffer.h" 
#include "ConstantBuffer.h"
#include "IndexBuffer.h"
#include "VertexShader.h"
#include "PixelShader.h"
#include "Texture.h"


DeviceContext::DeviceContext(ID3D11DeviceContext* DeviceContext, RenderSystem* System) : mDeviceContext(DeviceContext),  mSystem(System)
{
}

void DeviceContext::ClearRenderTargetColor(const SwapChainPtr& SwapChain, float Red, float Green, float Blue, float Alpha)
{
	FLOAT ClearColor[] = { Red, Green, Blue, Alpha };
		
	mDeviceContext->ClearRenderTargetView(SwapChain->mRenderTargetView, ClearColor);
	mDeviceContext->ClearDepthStencilView(SwapChain->mDepthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1, 0);
	mDeviceContext->OMSetRenderTargets(1, &SwapChain->mRenderTargetView, SwapChain->mDepthStencilView);

}

//VERTEX BUFFER

void DeviceContext::SetVertexBuffer(const VertexBufferPtr& Buffer)
{
	UINT Stride = Buffer->mSizeVetex;
	UINT Offset = 0;

	mDeviceContext->IASetVertexBuffers(0, 1, &Buffer->mBuffer, &Stride, &Offset);
	mDeviceContext->IASetInputLayout(Buffer->mInLayout);
}

void DeviceContext::SetIndexBuffer(const IndexBufferPtr &Buffer)
{
	mDeviceContext->IASetIndexBuffer(Buffer->mBuffer, DXGI_FORMAT_R32_UINT, 0);
}

//	CONSTANT BUFFER

void DeviceContext::SetConstantBuffer(const VertexShaderPtr &vertexShader, const ConstantBufferPtr &constantBuffer)
{
	mDeviceContext->VSSetConstantBuffers(0, 1, &constantBuffer->mBuffer);
}

void DeviceContext::SetConstantBuffer(const PixelShaderPtr &pixelShader, const ConstantBufferPtr &constantBuffer)
{
	mDeviceContext->PSSetConstantBuffers(0, 1, &constantBuffer->mBuffer);
}

void DeviceContext::SetTexture(const VertexShaderPtr &vertexShader, const TexturePtr* Texture, UINT NumTextures)
{
	ID3D11ShaderResourceView* ResoureceList[32];
	ID3D11SamplerState* ResoureceSamplerList[32];

	for (size_t i = 0; i < NumTextures; ++i) {
		ResoureceList[i] = Texture[i]->mShaderResourceView;
		ResoureceSamplerList[i] = Texture[i]->mSamplerState;
	}
	mDeviceContext->VSSetShaderResources(0, NumTextures, ResoureceList);
	mDeviceContext->VSSetSamplers(0, NumTextures, ResoureceSamplerList);
}

void DeviceContext::SetTexture(const PixelShaderPtr &pixelShader, const TexturePtr* Texture, UINT NumTextures)
{
	ID3D11ShaderResourceView* ResoureceList[32];
	ID3D11SamplerState* ResoureceSamplerList[32];

	for (size_t i = 0; i < NumTextures; ++i) {
		ResoureceList[i] = Texture[i]->mShaderResourceView;
		ResoureceSamplerList[i] = Texture[i]->mSamplerState;
	}
	mDeviceContext->PSSetShaderResources(0, NumTextures, ResoureceList);
	mDeviceContext->PSSetSamplers(0, NumTextures, ResoureceSamplerList);
}

// VERTEX SHADER

void DeviceContext::SetVertexShader(const VertexShaderPtr &vertexShader)
{
	mDeviceContext->VSSetShader(vertexShader->mvertexShader, nullptr, 0);
}

//PIXEL SHADER

void DeviceContext::SetPixelShader(const PixelShaderPtr &pixelShader)
{
	mDeviceContext->PSSetShader(pixelShader->mPixelShader, nullptr, 0);
}

void DeviceContext::DrawTriangleList(UINT VertexCount, UINT VertexStartIndex)
{
	if (!mDeviceContext) return;
	mDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	mDeviceContext->Draw(VertexCount, VertexStartIndex);
}

void DeviceContext::DrawIndexTriangleList(UINT IndexCount, UINT StartVertrexIndex, UINT StartIndexLocation)
{
	mDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	mDeviceContext->DrawIndexed(IndexCount, StartIndexLocation, StartVertrexIndex);
}


void DeviceContext::DrawTriangleStrip(UINT VertexCount, UINT VertexStartIndex)
{
	if (!mDeviceContext) return;
	mDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
	mDeviceContext->Draw(VertexCount, VertexStartIndex);
}

void DeviceContext::SetViewportSize(UINT Width, UINT Height)
{
	D3D11_VIEWPORT Viewport = {};
	Viewport.Width = static_cast<FLOAT>(Width);
	Viewport.Height = static_cast<FLOAT>(Height);
	Viewport.MinDepth = 0.0f;
	Viewport.MaxDepth = 1.0f;

	if (mDeviceContext) {
		mDeviceContext->RSSetViewports(1, &Viewport);
	}
}

DeviceContext::~DeviceContext()
{
	mDeviceContext->Release();
}

