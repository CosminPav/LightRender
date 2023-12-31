#include "AppWindow.h"
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

#include "Mesh.h"
#include "Material.h"

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

AppWindow::AppWindow()
{
}

void AppWindow::Update()
{
	UpdateCamera();
	UpdateLight();
	UpdateSkyBox();
}

void AppWindow::OnKeyDown(int key)
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
	}case 'O': case 'o':
	{
		mLightRadius -= 1.0f * mDeltaTime;
		break;
	}case 'P': case 'p':
	{
		mLightRadius += 1.0f * mDeltaTime;
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

void AppWindow::OnKeyUp(int key)
{
	Forward = 0.0f;
	Right = 0.0f;

	if (key == 'G' || key == 'g') {
		bPlayState = (bPlayState) ? false : true;
		Input::InputSystem::Get()->HideCursor(!bPlayState);
	}
	else if (key == 'F' || key == 'f') {
		bFullScreenState = (bFullScreenState) ? false : true;
		if (mSwapChain) {
			RECT ScreenSize = GetScreenSize();
			mSwapChain->SetFullScreen(bFullScreenState, ScreenSize.right, ScreenSize.bottom);
		}
	}
}

void AppWindow::OnMouseMove(const Point& MousePos)
{
	if (!bPlayState) return;
	int Width = GetClientWindowRect().right - GetClientWindowRect().left;
	int Height = GetClientWindowRect().bottom - GetClientWindowRect().top;

	RotationX += (MousePos.Y - (static_cast<float>(Height) / 2.0f)) * mDeltaTime * 0.1f;
	RotationY += (MousePos.X-(static_cast<float>(Width) /2.0f)) * mDeltaTime  * 0.1f;

	Input::InputSystem::Get()->SetCursorPosition(Point(Width / 2, Height / 2));
}

void AppWindow::OnLeftMouseButtonDown(const Point& MousePos)
{
	Scale = 0.5f;
}

void AppWindow::DrawMesh(const MeshPtr& Mesh, const std::vector<MaterialPtr>& Materials)
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
		GraphicsEngine::Get()->GetRenderSystem()->GetImmDeviceContext()->DrawIndexTriangleList(Mat.NumIndices,0, Mat.StartIndex);
	}
}

void AppWindow::OnLeftMouseButtonUp(const Point& MousePos)
{
	Scale = 1.0f;
}

void AppWindow::OnRightMouseButtonDown(const Point& MousePos)
{
	Scale = 2.0f;
}

void AppWindow::OnRightMouseButtonUp(const Point& MousePos)
{
	Scale = 1.0f;
}

void AppWindow::UpdateCamera()
{
	Math::Matrix4X4 WorldCam, TempMatrix;
	WorldCam.SetIdentity();

	TempMatrix.SetIdentity();
	TempMatrix.SetRotationX(RotationX);
	WorldCam *= TempMatrix;


	TempMatrix.SetIdentity();
	TempMatrix.SetRotationY(RotationY);
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

void AppWindow::UpdateModel(Math::Vector3D Position, const std::vector<MaterialPtr>& Materials)
{
	Constants constant;

	//Light rotation matrix
	Math::Matrix4X4 LightRotMatrix;
	LightRotMatrix.SetIdentity();
	LightRotMatrix.SetRotationY(RotationY_Light);

	constant.World.SetIdentity();
	constant.World.SetTranslation(Position);
	constant.View = mViewCam;
	constant.Projection = mProjCam;
	constant.CameraPosition = mWorldCam.GetTranslation();


	constant.LightPosition = mLightPosition;
	constant.LightRadius = mLightRadius;
	constant.LightDirection = LightRotMatrix.GetZDirection();

	constant.Time = mTime;
	//Update the constant buffer
	for (size_t m = 0; m < Materials.size(); ++m) {
		Materials[m]->SetData(&constant, sizeof(Constants));
	}
}

void AppWindow::UpdateSkyBox()
{
	Constants constant;

	constant.World.SetIdentity();
	constant.World.SetScale(Math::Vector3D(100.0f, 100.0f, 100.0f));
	constant.World.SetTranslation(mWorldCam.GetTranslation());
	constant.View = mViewCam;
	constant.Projection = mProjCam;

	//Update the constant buffer
	SkySphereMaterial->SetData(&constant, sizeof(Constants));
}
void AppWindow::UpdateLight()
{
	//go about 45 degrees
	RotationY_Light += .707f * mDeltaTime;

	float DistFromOrigin = 3.0f;

	//mLightPosition = Math::Vector4D(cos(RotationY_Light) * DistFromOrigin, 2.0f, sin(RotationY_Light) * DistFromOrigin, 1.0f);
	mLightPosition = Math::Vector4D(180.0f, 140.0f,70.0f, 1.0f);
}

void AppWindow::Render()
{
	//CLEAR RENDER TARGET COLOR
	GraphicsEngine::Get()->GetRenderSystem()->GetImmDeviceContext()->ClearRenderTargetColor(mSwapChain, 0.0f, 0.4f, 0.2f, 1);

	RECT rc = GetClientWindowRect();

	//SET THE VIEWPORT SIZE
	GraphicsEngine::Get()->GetRenderSystem()->GetImmDeviceContext()->SetViewportSize(rc.right - rc.left, rc.bottom - rc.top);

	//Update the quad pos
	Update();

	//Render the terrain model
	mMaterials.clear();
	mMaterials.push_back(TerrainMaterial);
	UpdateModel(Math::Vector3D(0.0f, 0.0f, 0.0f), mMaterials);

	DrawMesh(mTerrainMesh, mMaterials);

	//Render the scene house
	mMaterials.clear();
	mMaterials.push_back(BarrelMaterial);
	mMaterials.push_back(BricksMaterial);
	mMaterials.push_back(WindowMaterial);
	mMaterials.push_back(WoodMaterial);

	for (int i = 0; i < 3; ++i) {
		for (int j = 0; j < 3; ++j) {
			UpdateModel(Math::Vector3D(-14.0f + 14.0f*i, 0.0f, 14.0f + 14.0f*j), mMaterials);
			DrawMesh(mHouseMesh, mMaterials);
		}
	}

	mMaterials.clear();
	mMaterials.push_back(SkySphereMaterial);
	
	//Render the sky sphere
	DrawMesh(mSkyMesh, mMaterials);

	mSwapChain->Preseant(true);


	//Delta time computations
	mOldDeltaTime = mNewDeltaTime;
	mNewDeltaTime = static_cast<float>(GetTickCount64());
	mDeltaTime = (mNewDeltaTime) ? ((mNewDeltaTime - mOldDeltaTime) / 1000.0f) : 0;

	//Clouds anim offset
	mTime += mDeltaTime;
}

//May be deleted, but later
void AppWindow::OnCreate()
{
	DWindow::OnCreate();

	//Register input system
	Input::InputSystem::Get()->AddListener(this);

	bPlayState = true;
	Input::InputSystem::Get()->HideCursor(false);

	//Load textures
	//WallTexture = GraphicsEngine::Get()->GetTextureManager()->CreatTextureFromFile(L"Assets\\Textures\\wall.jpg");
	//EarthColorTexture = GraphicsEngine::Get()->GetTextureManager()->CreatTextureFromFile(L"Assets\\Textures\\earth_color.jpg");

	//Load the sky sphere texture
	SkyTexture = GraphicsEngine::Get()->GetTextureManager()->CreatTextureFromFile(L"Assets\\Textures\\stars_map.jpg");
	TerrainTexture = GraphicsEngine::Get()->GetTextureManager()->CreatTextureFromFile(L"Assets\\Textures\\ground.jpg");

	//House scene textures
	BarrelTexture = GraphicsEngine::Get()->GetTextureManager()->CreatTextureFromFile(L"Assets\\Textures\\barrel.jpg");
	WindowTexture = GraphicsEngine::Get()->GetTextureManager()->CreatTextureFromFile(L"Assets\\Textures\\house_windows.jpg");
	BricksTexture = GraphicsEngine::Get()->GetTextureManager()->CreatTextureFromFile(L"Assets\\Textures\\house_brick.jpg");
	WoodTexture = GraphicsEngine::Get()->GetTextureManager()->CreatTextureFromFile(L"Assets\\Textures\\house_wood.jpg");

	//Load the actual static mesh
	//mMesh = GraphicsEngine::Get()->GetMeshManager()->CreateMeshFromFile(L"Assets\\Meshes\\scene.obj");

	//mTorusMesh = GraphicsEngine::Get()->GetMeshManager()->CreateMeshFromFile(L"Assets\\Meshes\\torus.obj");
	//mMonkeyHeadMesh = GraphicsEngine::Get()->GetMeshManager()->CreateMeshFromFile(L"Assets\\Meshes\\suzanne.obj");
	//mPlaneMesh = GraphicsEngine::Get()->GetMeshManager()->CreateMeshFromFile(L"Assets\\Meshes\\plane.obj");

	//Load the sky box mesh
	mSkyMesh = GraphicsEngine::Get()->GetMeshManager()->CreateMeshFromFile(L"Assets\\Meshes\\sphere.obj");
	mTerrainMesh = GraphicsEngine::Get()->GetMeshManager()->CreateMeshFromFile(L"Assets\\Meshes\\terrain.obj");

	mHouseMesh = GraphicsEngine::Get()->GetMeshManager()->CreateMeshFromFile(L"Assets\\Meshes\\house.obj");

	//Initalize the graphics pipline
	RECT rc = GetClientWindowRect();
	mSwapChain = GraphicsEngine::Get()->GetRenderSystem()->MakeSwapChain(hwnd, rc.right - rc.left, rc.bottom - rc.top);

	mWorldCam.SetTranslation(Math::Vector3D(0.0f, 0.0f, -1.0f));


	/** MAKE THE MATERIAL */

	//Mesh material
	MeshMaterial = GraphicsEngine::Get()->MakeMaterial(L"PointLightVertexShader.hlsl", L"PointLightPixelShader.hlsl");
	MeshMaterial->AddTexture(WallTexture);
	MeshMaterial->SetCullMode(ECullMode::Back);


	TerrainMaterial = GraphicsEngine::Get()->MakeMaterial(MeshMaterial);
	TerrainMaterial->AddTexture(TerrainTexture);
	TerrainMaterial->SetCullMode(ECullMode::Back);

	//House scene material loadings

	BarrelMaterial = GraphicsEngine::Get()->MakeMaterial(MeshMaterial);
	BarrelMaterial->AddTexture(BarrelTexture);
	BarrelMaterial->SetCullMode(ECullMode::Back);

	WindowMaterial = GraphicsEngine::Get()->MakeMaterial(MeshMaterial);
	WindowMaterial->AddTexture(WindowTexture);
	WindowMaterial->SetCullMode(ECullMode::Back);

	WoodMaterial = GraphicsEngine::Get()->MakeMaterial(MeshMaterial);
	WoodMaterial->AddTexture(WoodTexture);
	WoodMaterial->SetCullMode(ECullMode::Back);

	BricksMaterial = GraphicsEngine::Get()->MakeMaterial(MeshMaterial);
	BricksMaterial->AddTexture(BricksTexture);
	BricksMaterial->SetCullMode(ECullMode::Back);

	//Sky sphere material
	SkySphereMaterial = GraphicsEngine::Get()->MakeMaterial(L"PointLightVertexShader.hlsl", L"SkyBoxShader.hlsl");
	SkySphereMaterial->AddTexture(SkyTexture);
	SkySphereMaterial->SetCullMode(ECullMode::Front);
	
	//Set camera starting trasnlastion
	mWorldCam.SetTranslation(Math::Vector3D(2.0f, 2.0f, 0.0f));

	mMaterials.reserve(32);
}

void AppWindow::OnUpdate()
{
	DWindow::OnUpdate();

	Input::InputSystem::Get()->Update();

	Render();
}

void AppWindow::OnDestroy()
{
	DWindow::OnDestroy();	
	mSwapChain->SetFullScreen(false, 1, 1);
}

void AppWindow::OnFocus()
{
	DWindow::OnFocus();
	Input::InputSystem::Get()->AddListener(this);
}

void AppWindow::StopFocus()
{
	DWindow::StopFocus();
	Input::InputSystem::Get()->RemoveListener(this);
}

void AppWindow::OnSize()
{
	//Get the window size
	RECT rc = GetClientWindowRect();

	//Resize the swap chain buffers
	if (mSwapChain) {
		mSwapChain->Resize(rc.right, rc.bottom);
	}
	//Update the window while resizing
	Render();
}

