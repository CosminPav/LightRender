#include "PixelShader.h"
#include "RenderSystem.h"
#include <exception>

PixelShader::PixelShader(const void* ShaderByteCode, size_t ShaderCodeSize, RenderSystem* System) : mSystem(System)
{
	if (FAILED(mSystem->d3dDevice->CreatePixelShader(ShaderByteCode, ShaderCodeSize, nullptr, &mPixelShader))) {
		throw std::exception("Pixel Shader failed to load");
	}
}

PixelShader::~PixelShader()
{
	if (mPixelShader)
		mPixelShader->Release();
}
