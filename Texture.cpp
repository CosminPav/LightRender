#include "Texture.h"
#include <DirectXTex.h>
#include <exception>
#include "GraphicsEngine.h"
#include "RenderSystem.h"

Texture::Texture(const wchar_t* FullPath) : Resource(FullPath)
{
	DirectX::ScratchImage ImageData;
	HRESULT HRes = DirectX::LoadFromWICFile(FullPath, DirectX::WIC_FLAGS_NONE, nullptr, ImageData);
	if (SUCCEEDED(HRes)) {
		HRes = DirectX::CreateTexture(GraphicsEngine::Get()->GetRenderSystem()->d3dDevice, ImageData.GetImages(), ImageData.GetImageCount(),
			ImageData.GetMetadata(), &mResource);

		D3D11_SHADER_RESOURCE_VIEW_DESC  Desc = {};
		Desc.Format = ImageData.GetMetadata().format;
		Desc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
		Desc.Texture2D.MipLevels = ImageData.GetMetadata().mipLevels;
		Desc.Texture2D.MostDetailedMip = 0;

		GraphicsEngine::Get()->GetRenderSystem()->d3dDevice->CreateShaderResourceView(mResource, &Desc, &mShaderResourceView);
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
