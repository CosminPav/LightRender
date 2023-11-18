#pragma once
#include "Resource.h"
#include <d3d11.h>
#include "DeviceContext.h"

class Texture : public Resource
{
	friend class DeviceContext;

	ID3D11Resource* mResource = nullptr;
	ID3D11ShaderResourceView* mShaderResourceView = nullptr;
	ID3D11SamplerState* mSamplerState = nullptr;
public:
	Texture(const wchar_t* FullPath);
	~Texture();
};

