#pragma once
#include "InputInterface.h"
#include "Window.h"
#include "Matrix4X4.h"
#include "Prerequisites.h"
#include <vector>

class DWindow;

class SpaceShooter : public DWindow, public InputListener
{
	SwapChainPtr mSwapChain = nullptr;


	//Texture
	TexturePtr SkyTexture;
	TexturePtr ShipTexture;
	TexturePtr AsteroidTexture;

	//Mesh
	MeshPtr mMesh;
	MeshPtr mSkyMesh;
	MeshPtr mShipMesh;
	MeshPtr mAsterioidMesh;

	//Materials
	MaterialPtr MeshMaterial;
	MaterialPtr SkySphereMaterial;
	MaterialPtr ShipMaterial;
	MaterialPtr AsteroidMaterial;


	Math::Matrix4X4 mWorldCam;
	Math::Matrix4X4 mViewCam;
	Math::Matrix4X4 mProjCam;

	float Forward{ 0.0f };
	float Right{ 0.0f };

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
	
	Math::Vector3D mShipPosition;
	Math::Vector3D mCurrentShipPos;

	Math::Vector3D mCurrentShipRotation;
	Math::Vector3D mShipRotation;

	float ShipSpeed{ 125.0f };

	//When true the window is in play mode and the cursor is hidden, when false it is in window mode 
	bool bPlayState{ false };
	bool bFullScreenState{ false };
	bool bTurboMode{ false };

	//Rotate the light on the y axis
	float RotationY_Light{ 0.0f };

	Math::Matrix4X4 LightRotMatrix;

	//Clouds animation offset
	float mTime{ 0.0f };

	Math::Vector3D mAsteroidsPositions[500];
	Math::Vector3D mAsteroidsRotations[500];
	Math::Vector3D mAteroidsScales[500];

	Math::Vector4D mLightPosition;

	std::vector<MaterialPtr> mMaterials;
public:
	SpaceShooter();

	void Render();
	void Update();
	void UpdateCamera();
	void UpdateThirdPersonCamera();
	void UpdateModel(Math::Vector3D Position, Math::Vector3D Rotaion, Math::Vector3D Scale, const std::vector<MaterialPtr>& Materials);
	void UpdateSkyBox();
	void UpdateLight();
	void UpdateShip();

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

	~SpaceShooter() {}

	virtual void OnCreate() override;
	virtual void OnUpdate() override;
	virtual void OnDestroy() override;
	virtual void OnFocus() override;
	virtual void StopFocus() override;
	virtual void OnSize() override;
};

