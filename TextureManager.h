#pragma once
#include "ResourceManager.h"
#include "Prerequisites.h"
#include "Texture.h"


class TextureManager: public ResourceManager
{

public:
	TextureManager() : ResourceManager()
	{}


	TexturePtr CreatTextureFromFile(const wchar_t* FilePath);
	TexturePtr CreateTexture(const FRect& Size, Texture::TextureType Type);

	~TextureManager() = default;
protected:
	Resource* CreateResourceFromFileConcrete(const wchar_t* FilePath) override;
};

