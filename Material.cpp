 #include "Material.h"
#include "RenderSystem.h"
#include "GraphicsEngine.h"
#include  <stdexcept>

Material::Material(const wchar_t* VertexShaderPath, const wchar_t* PixelShaderPath)
{
	//ptr to mem buffer
	void* ShaderByteCode = nullptr;
	//the zize if mem buffer in bytes
	size_t SizeOfShader = 0;

	//VERTEX SHADER

	//compile the vertex shader from file
	GraphicsEngine::Get()->GetRenderSystem()->CompileVertexShader(VertexShaderPath, "vsmain", &ShaderByteCode, &SizeOfShader);

	//Create the verrtex shader
	mVertexShader = GraphicsEngine::Get()->GetRenderSystem()->MakeVertexShader(ShaderByteCode, SizeOfShader);

	GraphicsEngine::Get()->GetRenderSystem()->ReleaseCompiledShaders();

	if (!mVertexShader)
		throw std::runtime_error("Material failed to create properly->VertexShader");

	//PIXEL SHADER

	//Compile the pixel shader file
	GraphicsEngine::Get()->GetRenderSystem()->CompilePixelShader(PixelShaderPath, "psmain", &ShaderByteCode, &SizeOfShader);
	//Make the pixel shader
	mPixelShader = GraphicsEngine::Get()->GetRenderSystem()->MakePixelShader(ShaderByteCode, SizeOfShader);
	//release the pixel shader
	GraphicsEngine::Get()->GetRenderSystem()->ReleaseCompiledShaders();

	if (!mPixelShader)
		throw std::runtime_error("Material failed to create properly->PixelShader");
}

Material::Material(const MaterialPtr& source)
{
	mVertexShader = source->mVertexShader;
	mPixelShader = source->mPixelShader;
}


Material::~Material() noexcept
{

}

void Material::AddTexture(const TexturePtr& Texture)
{
	Textures.push_back(Texture);
}

void Material::RemoveTexture(unsigned int Index)
{
	if (Index >= Textures.size())
		return;
	Textures.erase(Textures.begin() + Index);
}

void Material::SetData(void* Data, unsigned int Size)
{
	if (!mConstantBuffer)
		mConstantBuffer = GraphicsEngine::Get()->GetRenderSystem()->MakeConstantBuffer(Data, Size);
	else
		mConstantBuffer->Update(GraphicsEngine::Get()->GetRenderSystem()->GetImmDeviceContext(), Data);
}



