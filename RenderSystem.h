#pragma once
#include <d3d11.h>
#include "Prerequisites.h"


class RenderSystem
{
	//friend classes
	friend class SwapChain;
	friend class VertexBuffer;
	friend class IndexBuffer;
	friend class ConstantBuffer;
	friend class VertexShader;
	friend class PixelShader;
	friend class Texture;

	//diect x inteface
	ID3D11Device* d3dDevice = nullptr;
	D3D_FEATURE_LEVEL mFeatureLevel = {};


	IDXGIDevice* dxgiDevice = nullptr;
	IDXGIAdapter* dxgiAdapter = nullptr;
	IDXGIFactory* dxgiFactory = nullptr;
	ID3D11DeviceContext* mImmContext = nullptr;
	/*
	Device context
	*/
	DeviceContextPtr mDeviceContext = nullptr;

	/*
	* VERTEX BUFFER
	*/


	/*
	* PIXEL/VERTEX SHADERS
	*/
	ID3DBlob* mBlob = nullptr;

	ID3DBlob* mVsBlob = nullptr;
	ID3DBlob* mPsBlob = nullptr;
	ID3D11VertexShader* mVertexShader = nullptr;
	ID3D11PixelShader* mPixelShader = nullptr;

	ID3D11RasterizerState* mCullFrontState = nullptr;
	ID3D11RasterizerState* mCullBackState = nullptr;


	void InitRasterizerState();
public:

	SwapChainPtr MakeSwapChain(HWND hwnd, UINT Width, UINT Height);
	inline DeviceContextPtr GetImmDeviceContext() { return mDeviceContext; }
	VertexBufferPtr CreateVertexBuffer(void* ListVertices, UINT SizeVertex, UINT ListSize, void* ShaderByteCode, UINT SizeShderByte);
	ConstantBufferPtr MakeConstantBuffer(void* Buffer, UINT BufferSize);
	IndexBufferPtr MakeIndexBuffer(void* ListIndecies, UINT ListSize);
	VertexShaderPtr MakeVertexShader(const void* SharedByteCode, size_t ByteCodeLength);
	PixelShaderPtr MakePixelShader(const void* SharedByteCode, size_t ByteCodeLength);
public:
	//Default constructor
	RenderSystem();


	//Custom vertex shader
	bool CompileVertexShader(const wchar_t* FileName, const char* EntryPointName, void** ShaderByteCode, size_t* ByteCodeSize);
	void ReleaseCompiledShaders();

	//Custom pixel shader
	bool CompilePixelShader(const wchar_t* FileName, const char* EntryPointName, void** SharedByteCode, size_t* ByteCodeSize);

	//@bCullFront = true->front face culling otherwise back face culling 
	void SetRazterizerState(bool bCullFront);
	//Default destructor
	~RenderSystem();
};

