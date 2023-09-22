#pragma once
#include "InputInterface.h"
#include "Window.h"
#include "Matrix4X4.h"
#include "Prerequisites.h"

class DWindow;


//class Point;


class AppWindow : public DWindow, public InputListener
{
	SwapChainPtr mSwapChain = nullptr;
	VertexBufferPtr mVertexBuffer;
	IndexBufferPtr mIndexBuffer;
	VertexShaderPtr mVertexShader;
	PixelShaderPtr mPixelShader;
	ConstantBufferPtr mConstantBuffer;

	Math::Matrix4X4 mWorldCam;

	float Forward{ 0.0f };
	float Right{ 0.0f };

	float mOldDeltaTime{ 0.0f };
	float mNewDeltaTime{ 0.0f };
	float mDeltaTime{ 0.0f };

	float mDeltaPos{ 0.0f };
	float mDeltaScale{ 0.0f };

	float RotationX{ 0.0f };
	float RotationY{ 0.0f };

	float Scale{ 1.0f }; 
public:
	AppWindow();

	void Update();

	/*
	INPUT INTERFACE
	*/
	void OnKeyDown(int key) override;
	void OnKeyUp(int key) override;


	void OnMouseMove(const Point& MousePos) override;
	void OnLeftMouseButtonDown(const Point& MousePos) override;
	void OnLeftMouseButtonUp(const Point& MousePos) override;
	void OnRightMouseButtonDown(const Point& MousePos) override;
	void OnRightMouseButtonUp(const Point& MousePos) override;
	/*
	END OF INPUT INTERFACE
	*/

	~AppWindow() {}

	virtual void OnCreate() override;
	virtual void OnUpdate() override;
	virtual void OnDestroy() override;
	virtual void OnFocus() override;
	virtual void StopFocus() override;

	

};

