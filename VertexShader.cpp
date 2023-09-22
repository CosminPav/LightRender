#include "VertexShader.h"
#include  "RenderSystem.h"
#include <exception>

VertexShader::VertexShader(const void * ShaderByteCode, size_t ShaderByteCodeLength, RenderSystem* System): mSystem(System)
{
	if (FAILED(mSystem->d3dDevice->CreateVertexShader(ShaderByteCode, ShaderByteCodeLength, nullptr, &mvertexShader))) {
		throw std::exception("Vertex Shader failed to load");
	}
}


VertexShader::~VertexShader()
{
	if(mvertexShader)
		mvertexShader->Release();
}
