#pragma once
#include "Resource.h"
#include <d3d11.h>
#include "DeviceContext.h"

enum class EType
{
	Normal, RenderTarget, DepthStencil
};

class Texture : public Resource
{
	friend class DeviceContext;

	ID3D11Resource* mResource = nullptr;
	ID3D11ShaderResourceView* mShaderResourceView = nullptr;
	ID3D11SamplerState* mSamplerState = nullptr;

	EType TextureType = EType::Normal;
public:
	Texture(const wchar_t* FullPath);
	Texture(const FRect& Size, EType Type);
	~Texture();
};

