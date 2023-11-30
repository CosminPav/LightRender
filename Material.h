#pragma once
#include "VertexShader.h"
#include "PixelShader.h"
#include "ConstantBuffer.h"
#include <vector>

enum class ECullMode : int
{
	Front, 
	Back
};

class GraphicsEngine;

class Material
{
	friend class GraphicsEngine;

	VertexShaderPtr mVertexShader;
	PixelShaderPtr mPixelShader;
	ConstantBufferPtr mConstantBuffer;
	
	//Texture list for the material 
	std::vector<TexturePtr> Textures;

	//Cull mode
	ECullMode mCullMode = ECullMode::Back;

public:
	Material(const wchar_t* VertexShaderPath, const wchar_t* PixelShaderPath);
	Material(const MaterialPtr& source);
	~Material() noexcept;

	void AddTexture(const TexturePtr& Texture);
	void RemoveTexture(unsigned int Index);

	void SetData(void* Data, unsigned int Size);

	//Set and get the cull mode
	inline void SetCullMode(ECullMode CullModeToSet) { mCullMode = CullModeToSet; }
	inline ECullMode GetCullMode() const { return mCullMode; }
};

