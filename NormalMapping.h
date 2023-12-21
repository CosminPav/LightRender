#pragma once
#include "InputInterface.h"
#include "Window.h"
#include "Matrix4X4.h"
#include "Prerequisites.h"
#include <vector>

class DWindow;

class NormalMapping : public DWindow, public InputListener
{
	SwapChainPtr mSwapChain = nullptr;


	//Texture
	TexturePtr SkyTexture;
	TexturePtr BrickTexture;
	TexturePtr BrickNormalMap;

	//Mesh
	MeshPtr mMesh;
	MeshPtr mSkyMesh;


	//Materials
	MaterialPtr MeshMaterial;
	MaterialPtr BrickMaterial;
	MaterialPtr SkySphereMaterial;



	Math::Matrix4X4 mWorldCam;
	Math::Matrix4X4 mViewCam;
	Math::Matrix4X4 mProjCam;


	float mOldDeltaTime{ 0.0f };
	float mNewDeltaTime{ 0.0f };
	float mDeltaTime{ 0.0f };

	Math::Vector3D mCurrentCameraPostion;
	Math::Vector3D mCameraPosition;

	Math::Vector3D mCurrentCameraRotation;
	Math::Vector3D mCameraRotation;
	float mDeltaMouseX{ 0.0f };
	float mDeltaMouseY{ 0.0f };

	float mCameraDistance{ 24.0f };
	float mCurrentCameraDistance{ 0.0f };

	//When true the window is in play mode and the cursor is hidden, when false it is in window mode 
	bool bPlayState{ false };
	bool bFullScreenState{ false };

	//Rotate the light on the y axis
	float RotationY_Light{ 0.0f };

	Math::Matrix4X4 LightRotMatrix;

	//Clouds animation offset
	float mTime{ 0.0f };


	Math::Vector4D mLightPosition;

	std::vector<MaterialPtr> mMaterials;
public:
	NormalMapping();

	void Render();
	void Update();
	void UpdateCamera();
	void UpdateViewportProj();
	void UpdateThirdPersonCamera();
	void UpdateModel(Math::Vector3D Position, Math::Vector3D Rotaion, Math::Vector3D Scale, const std::vector<MaterialPtr>& Materials);
	void UpdateSkyBox();
	void UpdateLight();


	void DrawMesh(const MeshPtr& Mesh, const std::vector<MaterialPtr>& Materials);
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

	~NormalMapping() {}

	virtual void OnCreate() override;
	virtual void OnUpdate() override;
	virtual void OnDestroy() override;
	virtual void OnFocus() override;
	virtual void StopFocus() override;
	virtual void OnSize() override;
};


