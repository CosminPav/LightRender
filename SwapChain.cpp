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
	SDesc.Windowed = TRUE;

	//Create the swap chain for the window indicated by the HWND param
	HRESULT hRes = mSystem->dxgiFactory->CreateSwapChain(Sd3dDevice, &SDesc, &dxgiSwapChain);

	//If the swap chain fails to create, abort
	if (FAILED(hRes)) {
		throw std::exception("Swap Chain failed to load");
	}
		

	//Get the back buffer color and create its render target view
	ID3D11Texture2D* Buffer = nullptr;

	hRes = dxgiSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&Buffer);

	if (FAILED(hRes)) {
		throw std::exception("Swap Chain failed to load");
	}
		
	hRes = Sd3dDevice->CreateRenderTargetView(Buffer, NULL, &mRenderTargetView);
	Buffer->Release();

	if (FAILED(hRes))
		throw std::exception("Swap Chain failed to load");
}


bool SwapChain::Preseant(bool vSync)
{
	dxgiSwapChain->Present(vSync, NULL);
	return true;
}

SwapChain::~SwapChain()
{	
	dxgiSwapChain->Release();
}
