#include "SpaceShooter.h"

#include "Window.h"
#include <Windows.h>
#include "GraphicsEngine.h"
#include "SwapChain.h"
#include "DeviceContext.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "ConstantBuffer.h"
#include "VertexShader.h"
#include "PixelShader.h"
#include "RenderSystem.h"
#include <Windows.h>
#include <iostream>

#include "InputSystem.h"
#include "Vector3D.h"
#include "Vector2D.h"
#include "Matrix4X4.h"
#include "Point.h"
#include "MathUtilis.h"

#include "Mesh.h"
#include "Material.h"
#include <time.h>

struct Vertex
{
	Math::Vector3D Position;
	Math::Vector2D TexCoord;
};

__declspec(align(16))
struct Constants
{
	Math::Matrix4X4 World;
	Math::Matrix4X4 View;
	Math::Matrix4X4 Projection;
	Math::Vector4D LightDirection;
	Math::Vector4D CameraPosition;
	Math::Vector4D LightPosition = Math::Vector4D(0.0f, 1.0f, 0.0f, 0.0f);
	float LightRadius{ 6.0f };

	float Time = 0.0f;
};

SpaceShooter::SpaceShooter()
{
}

void SpaceShooter::Update()
{
	UpdateShip();
	UpdateThirdPersonCamera();
	UpdateLight();
	UpdateSkyBox();
}

void SpaceShooter::OnKeyDown(int key)
{
	switch (key) {
	case 'W': case 'w':
	{
		Forward = 1.0f;
		break;
	}
	case 'S': case's':
	{
		Forward = -1.0f;
		break;
	}
	case 'A': case 'a':
	{
		Right = -1.0f;
		break;
	}case 'D': case 'd':
	{
		Right = 1.0f;
		break;
	}
	case VK_LSHIFT:
	{
		bTurboMode = true;
		break;
	}
	default:
		break;
	}

	//if (key == 'W' || key == 'w') {
	//	//RotationX += 3.19f * mDeltaTime;
	//	Forward = 1.0f;
	//}
	//else if (key == 'S' || key == 's') {
	//	//RotationX -= 3.19f * mDeltaTime;
	//	Forward = -1.0f;
	//}else if (key == 'A' || key == 'a') {
	//	//RotationY += 3.19f * mDeltaTime;
	//	Right = -1.0f;
	//}
	//else if (key == 'D' || key == 'd') {
	//	//RotationY -= 3.19f * mDeltaTime;
	//	Right = 1.0f;
	//}
	//else if (key == 'O' || key == 'o') {
	//	mLightRadius -= 1.0f * mDeltaTime;
	//}
	//else if (key == 'P' || key == 'p') {
	//	mLightRadius += 1.0f * mDeltaTime;
	//}
}

void SpaceShooter::OnKeyUp(int key)
{
	Forward = 0.0f;
	Right = 0.0f;

	if (key == VK_ESCAPE) {
		if (bPlayState) {
			bPlayState = false;
			Input::InputSystem::Get()->HideCursor(!bPlayState);
		}
	}
	else if (key == 'F' || key == 'f') {
		bFullScreenState = (bFullScreenState) ? false : true;
		if (mSwapChain) {
			RECT ScreenSize = GetScreenSize();
			mSwapChain->SetFullScreen(bFullScreenState, ScreenSize.right, ScreenSize.bottom);
		}
	}
	else if (key == VK_LSHIFT) {
		bTurboMode = false;
	}
}

void SpaceShooter::OnMouseMove(const Point& MousePos)
{
	if (!bPlayState) return;

	RECT WindowSize = GetClientWindowRect();

	int Width = WindowSize.right - WindowSize.left;
	int Height = WindowSize.bottom - WindowSize.top;

	mDeltaMouseX = (int)(MousePos.X - (int)(WindowSize.left + (static_cast<float>(Width) / 2.0f)));
	mDeltaMouseY = (int)(MousePos.Y - (int)(WindowSize.top + (static_cast<float>(Height) / 2.0f)));

	Input::InputSystem::Get()->SetCursorPosition(Point(WindowSize.left + (int)Width / 2, WindowSize.top+ (int)Height / 2));
}

void SpaceShooter::DrawMesh(const MeshPtr& Mesh, const std::vector<MaterialPtr>& Materials)
{
	//Set the vertex buffer
	GraphicsEngine::Get()->GetRenderSystem()->GetImmDeviceContext()->SetVertexBuffer(Mesh->GetVertexBuffer());

	//Set the index buffer
	GraphicsEngine::Get()->GetRenderSystem()->GetImmDeviceContext()->SetIndexBuffer(Mesh->GetIndexBuffer());


	for (size_t m = 0; m < Mesh->GetNumMaterials(); ++m) {

		if (m >= Materials.size()) break;

		MaterialSlot Mat = Mesh->GetMaterialSlot(m);
		//Set the mesh material
		GraphicsEngine::Get()->SetMaterial(Materials[m]);
		GraphicsEngine::Get()->GetRenderSystem()->GetImmDeviceContext()->DrawIndexTriangleList(Mat.NumIndices, 0, Mat.StartIndex);
	}
}

void SpaceShooter::OnLeftMouseButtonDown(const Point& MousePos)
{
	if (!bPlayState) {
		bPlayState = true;
		Input::InputSystem::Get()->HideCursor(!bPlayState);
	}
}

void SpaceShooter::OnLeftMouseButtonUp(const Point& MousePos)
{

}

void SpaceShooter::OnRightMouseButtonDown(const Point& MousePos)
{

}

void SpaceShooter::OnRightMouseButtonUp(const Point& MousePos)
{

}

void SpaceShooter::UpdateCamera()
{
	Math::Matrix4X4 WorldCam, TempMatrix;
	WorldCam.SetIdentity();

	mCameraRotation.X += mDeltaMouseY * mDeltaTime * 0.1f;
	mCameraRotation.Y += mDeltaMouseX * mDeltaTime * 0.1f;

	TempMatrix.SetIdentity();
	TempMatrix.SetRotationX(mCameraRotation.X);
	WorldCam *= TempMatrix;


	TempMatrix.SetIdentity();
	TempMatrix.SetRotationY(mCameraRotation.Y);
	WorldCam *= TempMatrix;



	Math::Vector3D NewPos = mWorldCam.GetTranslation() + WorldCam.GetZDirection() * (Forward * 0.15f);
	NewPos = NewPos + WorldCam.GetXDirection() * (Right * 0.15f);

	WorldCam.SetTranslation(NewPos);
	mWorldCam = WorldCam;

	WorldCam.Inverse();

	mViewCam = WorldCam;
	int Width = GetClientWindowRect().right - GetClientWindowRect().left;
	int Height = GetClientWindowRect().bottom - GetClientWindowRect().top;

	mProjCam.SetPerspectiveFOVLh(1.57f, static_cast<float>(Width / Height), 0.1f, 100.f);
}

void SpaceShooter::UpdateThirdPersonCamera()
{
	Math::Matrix4X4 WorldCam, TempMatrix;
	WorldCam.SetIdentity();

	mCameraRotation.X += mDeltaMouseY * mDeltaTime * 0.1f;
	mCameraRotation.Y += mDeltaMouseX * mDeltaTime * 0.1f;

	if (mCameraRotation.X >= 1.57f)
		mCameraRotation.X = 1.57f;
	else if (mCameraRotation.X <= -1.57f)
		mCameraRotation.X = -1.57f;

	mCurrentCameraRotation = Math::Vector3D::Lerp(mCurrentCameraRotation, mCameraRotation, 3.0f * mDeltaTime);

	TempMatrix.SetIdentity();
	TempMatrix.SetRotationX(mCurrentCameraRotation.X);
	WorldCam *= TempMatrix;


	TempMatrix.SetIdentity();
	TempMatrix.SetRotationY(mCurrentCameraRotation.Y);
	WorldCam *= TempMatrix;

	//Update the camera distance on moving forward||backwards
	if (Forward) {
		
		if (bTurboMode) {
			if (Forward > 0.0f)
				mCameraDistance = 37.0f;
			else
				mCameraDistance = 9.0f;
		}
		else {
			if (Forward > 0.0f)
				mCameraDistance = 28.0f;
			else
				mCameraDistance = 12.0f;
		}
			
	}
	else {
		mCameraDistance = 24.0f;
	}

	//Lerp between the camera distance and the current camera distance
	mCurrentCameraDistance = Math::Lerp(mCurrentCameraDistance, mCameraDistance, 2.0f * mDeltaTime);

	mCameraPosition = mCurrentShipPos;

	Math::Vector3D NewPos = mCameraPosition + WorldCam.GetZDirection() * (-mCurrentCameraDistance);
	NewPos = NewPos + WorldCam.GetYDirection() * (5.0f);

	WorldCam.SetTranslation(NewPos);
	mWorldCam = WorldCam;

	WorldCam.Inverse();

	mViewCam = WorldCam;
	int Width = GetClientWindowRect().right - GetClientWindowRect().left;
	int Height = GetClientWindowRect().bottom - GetClientWindowRect().top;

	mProjCam.SetPerspectiveFOVLh(1.57f, static_cast<float>(Width / Height), 0.1f, 5000);
}

void SpaceShooter::UpdateModel(Math::Vector3D Position, Math::Vector3D Rotaion, Math::Vector3D Scale, const std::vector<MaterialPtr>& Materials)
{
	Constants constant;

	Math::Matrix4X4 TempMatrix;

	constant.World.SetIdentity();

	//Compute Scale
	TempMatrix.SetIdentity();
	TempMatrix.SetScale(Scale);
	constant.World *= TempMatrix;

	//Compute rotation
	TempMatrix.SetIdentity();
	TempMatrix.SetRotationX(Rotaion.X);
	constant.World *= TempMatrix;

	TempMatrix.SetIdentity();
	TempMatrix.SetRotationY(Rotaion.Y);
	constant.World *= TempMatrix;

	TempMatrix.SetIdentity();
	TempMatrix.SetRotationZ(Rotaion.Z);
	constant.World *= TempMatrix;

	//Compute trranlation
	TempMatrix.SetIdentity();
	TempMatrix.SetTranslation(Position);
	constant.World *= TempMatrix;

	constant.View = mViewCam;
	constant.Projection = mProjCam;
	constant.CameraPosition = mWorldCam.GetTranslation();


	constant.LightPosition = mLightPosition;
	constant.LightRadius = 0.0f;
	constant.LightDirection = LightRotMatrix.GetZDirection();

	constant.Time = mTime;
	//Update the constant buffer
	for (size_t m = 0; m < Materials.size(); ++m) {
		Materials[m]->SetData(&constant, sizeof(Constants));
	}
}

void SpaceShooter::UpdateSkyBox()
{
	Constants constant;

	constant.World.SetIdentity();
	constant.World.SetScale(Math::Vector3D(4000.0f, 4000.0f, 4000.0f));
	constant.World.SetTranslation(mWorldCam.GetTranslation());
	constant.View = mViewCam;
	constant.Projection = mProjCam;

	//Update the constant buffer
	SkySphereMaterial->SetData(&constant, sizeof(Constants));
}
void SpaceShooter::UpdateLight()
{
	Math::Matrix4X4 TempMat;
	TempMat.SetIdentity();
	//Light rotation matrix
	LightRotMatrix.SetIdentity();

	TempMat.SetIdentity();
	TempMat.SetRotationX(-0.707f);
	LightRotMatrix *= TempMat;

	TempMat.SetIdentity();
	TempMat.SetRotationY(0.707f);
	LightRotMatrix *= TempMat;
}

void SpaceShooter::UpdateShip()
{
	Math::Matrix4X4 WorldModel, TempMatrix;
	WorldModel.SetIdentity();

	mShipRotation.X += mDeltaMouseY * mDeltaTime * 0.1f;
	mShipRotation.Y += mDeltaMouseX * mDeltaTime * 0.1f;

	if (mShipRotation.X >= 1.57f)
		mShipRotation.X = 1.57f;
	else if (mShipRotation.X <= -1.57f)
		mShipRotation.X = -1.57f;


	mCurrentShipRotation = Math::Vector3D::Lerp(mCurrentShipRotation, mShipRotation, 5.0f * mDeltaTime);

	TempMatrix.SetIdentity();
	TempMatrix.SetRotationX(mCurrentShipRotation.X);
	WorldModel *= TempMatrix;


	TempMatrix.SetIdentity();
	TempMatrix.SetRotationY(mCurrentShipRotation.Y);
	WorldModel *= TempMatrix;

	//Update the ship's speed
	if (bTurboMode)
		ShipSpeed = 405.0f;

	mShipPosition = mShipPosition + WorldModel.GetZDirection() * (Forward)*ShipSpeed*mDeltaTime;

	mCurrentShipPos = Math::Vector3D::Lerp(mCurrentShipPos, mShipPosition, 3.0f*mDeltaTime);
}

void SpaceShooter::Render()
{
	//CLEAR RENDER TARGET COLOR
	GraphicsEngine::Get()->GetRenderSystem()->GetImmDeviceContext()->ClearRenderTargetColor(mSwapChain, 0.0f, 0.4f, 0.2f, 1);

	RECT rc = GetClientWindowRect();

	//SET THE VIEWPORT SIZE
	GraphicsEngine::Get()->GetRenderSystem()->GetImmDeviceContext()->SetViewportSize(rc.right - rc.left, rc.bottom - rc.top);	

	//Render the space ship mesh player
	mMaterials.clear();
	mMaterials.push_back(ShipMaterial);
	UpdateModel(mCurrentShipPos, mCurrentShipRotation, Math::Vector3D(1.0f, 1.0f, 1.0f), mMaterials);
	DrawMesh(mShipMesh, mMaterials);

	//Render the asteroid mesh 
	for (size_t i = 0; i < 500; ++i) {
		mMaterials.clear();
		mMaterials.push_back(AsteroidMaterial);
		UpdateModel(mAsteroidsPositions[i], mAsteroidsRotations[i], mAteroidsScales[i], mMaterials);
		DrawMesh(mAsterioidMesh, mMaterials);
	}

	//Render the sky sphere
	mMaterials.clear();
	mMaterials.push_back(SkySphereMaterial);
	DrawMesh(mSkyMesh, mMaterials);

	mSwapChain->Preseant(true);


	//Delta time computations
	mOldDeltaTime = mNewDeltaTime;
	mNewDeltaTime = static_cast<float>(GetTickCount64());
	//mDeltaTime = (mNewDeltaTime) ? ((mNewDeltaTime - mOldDeltaTime) / 1000.0f) : 0;
	mDeltaTime = 1.0f / 60.0f;

	//Clouds anim offset
	mTime += mDeltaTime;
}

//May be deleted, but later
void SpaceShooter::OnCreate()
{
	DWindow::OnCreate();

	//Register input system
	Input::InputSystem::Get()->AddListener(this);

	bPlayState = true;
	Input::InputSystem::Get()->HideCursor(false);

	//Initalize the graphics pipline
	RECT rc = GetClientWindowRect();
	mSwapChain = GraphicsEngine::Get()->GetRenderSystem()->MakeSwapChain(hwnd, rc.right - rc.left, rc.bottom - rc.top);

	//rand vector pos
	srand((size_t)time(NULL));

	//Init asteroids arrays
	for (size_t p = 0; p < 500; ++p) {
		mAsteroidsPositions[p] = Math::Vector3D(rand() % 4000 + (-2000), rand() % 4000 + (-2000), rand() % 4000 + (-2000));
		mAsteroidsRotations[p] = Math::Vector3D((rand() % 628)/100.0f , (rand() % 628) / 100.0f, (rand() % 628) / 100.0f);
		float Scale = rand() % 10 + 1;
		mAteroidsScales[p] = Math::Vector3D(Scale, Scale, Scale);
	}

	/** TEXTURES  */

	//Load the sky sphere texture
	SkyTexture = GraphicsEngine::Get()->GetTextureManager()->CreatTextureFromFile(L"Assets\\Textures\\stars_map.jpg");
	ShipTexture = GraphicsEngine::Get()->GetTextureManager()->CreatTextureFromFile(L"Assets\\Textures\\spaceship.jpg");
	AsteroidTexture = GraphicsEngine::Get()->GetTextureManager()->CreatTextureFromFile(L"Assets\\Textures\\asteroid.jpg");

	/** MESHES  */

	//Load the sky box mesh
	mSkyMesh = GraphicsEngine::Get()->GetMeshManager()->CreateMeshFromFile(L"Assets\\Meshes\\sphere.obj");
	mShipMesh = GraphicsEngine::Get()->GetMeshManager()->CreateMeshFromFile(L"Assets\\Meshes\\spaceship.obj");
	mAsterioidMesh = GraphicsEngine::Get()->GetMeshManager()->CreateMeshFromFile(L"Assets\\Meshes\\asteroid.obj");

	/** MATERIALS */

	//Mesh material
	MeshMaterial = GraphicsEngine::Get()->MakeMaterial(L"DirLightVertexShader.hlsl", L"DirLightPixelShader.hlsl");
	MeshMaterial->SetCullMode(ECullMode::Back);

	//Load the space ship material
	ShipMaterial = GraphicsEngine::Get()->MakeMaterial(MeshMaterial);
	ShipMaterial->AddTexture(ShipTexture);
	ShipMaterial->SetCullMode(ECullMode::Back);

	//Load the asteroid  material
	AsteroidMaterial = GraphicsEngine::Get()->MakeMaterial(MeshMaterial);
	AsteroidMaterial->AddTexture(AsteroidTexture);
	AsteroidMaterial->SetCullMode(ECullMode::Back);

	//Sky sphere material
	SkySphereMaterial = GraphicsEngine::Get()->MakeMaterial(L"SkyBoxVertexShader.hlsl", L"SkyBoxShader.hlsl");
	SkySphereMaterial->AddTexture(SkyTexture);
	SkySphereMaterial->SetCullMode(ECullMode::Front);

	//Set camera starting trasnlastion
	mWorldCam.SetTranslation(Math::Vector3D(2.0f, 2.0f, 0.0f));

	mMaterials.reserve(32);
}

void SpaceShooter::OnUpdate()
{
	DWindow::OnUpdate();

	Input::InputSystem::Get()->Update();

	Update();

	Render();

	mDeltaMouseX = 0.0f;
	mDeltaMouseY = 0.0f;
}

void SpaceShooter::OnDestroy()
{
	DWindow::OnDestroy();
	mSwapChain->SetFullScreen(false, 1, 1);
}

void SpaceShooter::OnFocus()
{
	DWindow::OnFocus();
	Input::InputSystem::Get()->AddListener(this);
}

void SpaceShooter::StopFocus()
{
	DWindow::StopFocus();
	Input::InputSystem::Get()->RemoveListener(this);
}

void SpaceShooter::OnSize()
{
	//Get the window size
	RECT rc = GetClientWindowRect();

	//Resize the swap chain buffers
	if (mSwapChain) {
		mSwapChain->Resize(rc.right - rc.left, rc.bottom - rc.top);
	}
	//Update the window while resizing
	Update();
	Render();
}