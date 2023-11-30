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
	PixelShaderPtr mSkyPixelShader;
	ConstantBufferPtr mConstantBuffer;
	ConstantBufferPtr mSkyConstantBuffer;

	//Texture
	TexturePtr WallTexture;
	TexturePtr BricksTexture;
	TexturePtr EarthColorTexture;	//Earth color map
	TexturePtr EarthSpecularTexture;	//Earth specular map
	TexturePtr EarthCloudsTexture;	//Earth clouds map
	TexturePtr EarthColorTexture_Night;	//Earth night color map
	TexturePtr SkyTexture;

	//Mesh
	MeshPtr mMesh;
	MeshPtr mTorusMesh;
	MeshPtr mMonkeyHeadMesh;
	MeshPtr mPlaneMesh;
	MeshPtr mSkyMesh;

	//Materials
	MaterialPtr MeshMaterial;
	MaterialPtr BrickMaterial;
	MaterialPtr EarthMaterial;
	MaterialPtr SkySphereMaterial;

	Math::Matrix4X4 mWorldCam;
	Math::Matrix4X4 mViewCam;
	Math::Matrix4X4 mProjCam;

	float Forward{ 0.0f };
	float Right{ 0.0f };

	float mOldDeltaTime{ 0.0f };
	float mNewDeltaTime{ 0.0f };
	float mDeltaTime{ 0.0f };

	float mDeltaPos{ 0.0f };
	float mDeltaScale{ 0.0f };

	float RotationX{ 0.0f };
	float RotationY{ 0.0f };

	//When true the window is in play mode and the cursor is hidden, when false it is in window mode 
	bool bPlayState{ false };
	bool bFullScreenState{ false };

	//Rotate the light on the y axis
	float RotationY_Light{ 0.0f };

	//Clouds animation offset
	float mTime{ 0.0f };

	float Scale{ 1.0f }; 

	float mLightRadius{6.0f };

	Math::Vector4D mLightPosition;
public:
	AppWindow();

	void Render();
	void Update();
	void UpdateCamera();
	void UpdateModel(Math::Vector3D Position, const MaterialPtr& Material);
	void UpdateSkyBox();
	void UpdateLight();

	void DrawMesh(const MeshPtr& Mesh, const MaterialPtr& Material);
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
	virtual void OnSize() override;
	

};

