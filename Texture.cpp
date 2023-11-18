#include "Texture.h"
#include <DirectXTex.h>
#include <exception>
#include "GraphicsEngine.h"
#include "RenderSystem.h"

Texture::Texture(const wchar_t* FullPath) : Resource(FullPath)
{
	DirectX::ScratchImage ImageData;
	HRESULT HRes = DirectX::LoadFromWICFile(FullPath, DirectX::WIC_FLAGS_IGNORE_SRGB, nullptr, ImageData);
	if (SUCCEEDED(HRes)) {
		HRes = DirectX::CreateTexture(GraphicsEngine::Get()->GetRenderSystem()->d3dDevice, ImageData.GetImages(), ImageData.GetImageCount(),
			ImageData.GetMetadata(), &mResource);
		if(FAILED(HRes)) throw std::exception("Texture Image has not been created");
		D3D11_SHADER_RESOURCE_VIEW_DESC  Desc = {};
		Desc.Format = ImageData.GetMetadata().format;
		Desc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
		Desc.Texture2D.MipLevels = static_cast<UINT>(ImageData.GetMetadata().mipLevels);
		Desc.Texture2D.MostDetailedMip = 0;

		D3D11_SAMPLER_DESC samplerDesc = {};
		samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
		samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
		samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
		samplerDesc.Filter = D3D11_FILTER_ANISOTROPIC;
		samplerDesc.MinLOD = 0;
		samplerDesc.MaxLOD = static_cast<UINT>(ImageData.GetMetadata().mipLevels);

		HRes = GraphicsEngine::Get()->GetRenderSystem()->d3dDevice->CreateSamplerState(&samplerDesc, &mSamplerState);		
		if (FAILED(HRes)) throw std::exception("Texture Image has not been created");

		HRes = GraphicsEngine::Get()->GetRenderSystem()->d3dDevice->CreateShaderResourceView(mResource, &Desc, &mShaderResourceView);
		if (FAILED(HRes)) throw std::exception("Texture Image has not been created");
	}
	else {
		throw std::exception("Texture Image has not been created");
	}
}

Texture::~Texture()
{
	mShaderResourceView->Release();
	mResource->Release();
}
