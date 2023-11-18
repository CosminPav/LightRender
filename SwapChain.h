#pragma once
#include <iostream>
#include <d3d11.h>
#include <memory.h>
#include "Prerequisites.h"

class SwapChain
{
	friend class DeviceContext;

	IDXGISwapChain* dxgiSwapChain = nullptr;
	//std::unique_ptr<IDXGISwapChain> dxgiSwapChain_Smart = new IDXGISwapChain();
	ID3D11RenderTargetView* mRenderTargetView = nullptr;
	ID3D11DepthStencilView* mDepthStencilView = nullptr;

	//Render System ref
	RenderSystem* mSystem = nullptr;
public:
	//Default constructor
	SwapChain(HWND hwnd, UINT Width, UINT Height, RenderSystem* System);
	bool Preseant(bool vSync);

	//Resize the buffers in the swap chain
	void Resize(UINT width, UINT height);
	void ReloadBuffers(UINT width, UINT height);

	//@FullScreen to false = window mode ->WIndowed, FUll screen true = window mode ->Full Screen
	void SetFullScreen(bool FullScreen, UINT width, UINT height);
	//Default destructor
	~SwapChain();
};

