#include "SwapChain.h"
#include <iostream>
#include "RenderSystem.h"
#include <exception>



SwapChain::SwapChain(HWND hwnd, UINT Width, UINT Height, RenderSystem* System) : mSystem(System)
{
	//Ref to d3d device from the graphics engine
	ID3D11Device* Sd3dDevice = mSystem->d3dDevice;

	//Swap chain descriptor
	DXGI_SWAP_CHAIN_DESC SDesc;
	ZeroMemory(&SDesc, sizeof(SDesc));
	SDesc.BufferCount = 1;
	SDesc.BufferDesc.Width = Width;
	SDesc.BufferDesc.Height = Height;
	SDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	SDesc.BufferDesc.RefreshRate.Numerator = 60;
	SDesc.BufferDesc.RefreshRate.Denominator = 1;
	SDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	SDesc.OutputWindow = hwnd;
	SDesc.SampleDesc.Count = 1;
	SDesc.SampleDesc.Quality = 0;
	SDesc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
	SDesc.Windowed = TRUE;

	//Create the swap chain for the window indicated by the HWND param
	HRESULT hRes = mSystem->dxgiFactory->CreateSwapChain(Sd3dDevice, &SDesc, &dxgiSwapChain);

	//If the swap chain fails to create, abort
	if (FAILED(hRes)) {
		throw std::exception("Swap Chain failed to load");
	}
	ReloadBuffers(Width, Height);
}


bool SwapChain::Preseant(bool vSync)
{
	dxgiSwapChain->Present(vSync, NULL);
	return true;
}

SwapChain::~SwapChain()
{	
	dxgiSwapChain->Release();
	mRenderTargetView->Release();
}

void SwapChain::Resize(UINT width, UINT height)
{
	if (mRenderTargetView)
		mRenderTargetView->Release();
	if (mDepthStencilView)
		mDepthStencilView->Release();

	dxgiSwapChain->ResizeBuffers(1, width, height, DXGI_FORMAT_R8G8B8A8_UNORM, 0);
	ReloadBuffers(width, height);
}

void SwapChain::ReloadBuffers(UINT width, UINT height)
{
	ID3D11Device* Sd3dDevice = mSystem->d3dDevice;

	//Get the back buffer color and create its render target view
	ID3D11Texture2D* Buffer = nullptr;

	HRESULT hRes = dxgiSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&Buffer);

	if (FAILED(hRes)) {
		throw std::exception("Swap Chain failed to load");
	}

	hRes = Sd3dDevice->CreateRenderTargetView(Buffer, NULL, &mRenderTargetView);
	Buffer->Release();

	if (FAILED(hRes))
		throw std::exception("Swap Chain failed to load");

	D3D11_TEXTURE2D_DESC TexDesc = {};
	TexDesc.Width = width;
	TexDesc.Height = height;
	TexDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	TexDesc.Usage = D3D11_USAGE_DEFAULT;
	TexDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	TexDesc.MipLevels = 1;
	TexDesc.SampleDesc.Count = 1;
	TexDesc.SampleDesc.Quality = 0;
	TexDesc.MiscFlags = 0;
	TexDesc.ArraySize = 1;
	TexDesc.CPUAccessFlags = 0;

	hRes = Sd3dDevice->CreateTexture2D(&TexDesc, nullptr, &Buffer);
	if (FAILED(hRes)) {
		throw std::exception("Depth buffer failed to load");
	}

	hRes = Sd3dDevice->CreateDepthStencilView(Buffer, NULL, &mDepthStencilView);
	Buffer->Release();
	if (FAILED(hRes)) {
		throw std::exception("Depth buffer failed to load");
	}
}

void SwapChain::SetFullScreen(bool FullScreen, UINT width, UINT height)
{
	//Set full screen mode with the buffers updated with the current size
	Resize(width, height);

	//Set full screen state from the swap chain
	if (dxgiSwapChain)
		dxgiSwapChain->SetFullscreenState(FullScreen, nullptr);
}
