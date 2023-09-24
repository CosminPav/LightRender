#pragma once
#include "ResourceManager.h"
#include "Prerequisites.h"

class TextureManager: public ResourceManager
{

public:
	TextureManager() : ResourceManager()
	{}


	TexturePtr CreatTextureFromFile(const wchar_t* FilePath);

	~TextureManager() = default;
protected:
	Resource* CreateResourceFromFileConcrete(const wchar_t* FilePath) override;
};

