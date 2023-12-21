#include "NormalMapping.h"

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

NormalMapping::NormalMapping()
{
}

void NormalMapping::Update()
{
	UpdateThirdPersonCamera();
	UpdateLight();
	UpdateSkyBox();
}

void NormalMapping::OnKeyDown(int key)
{

}

void NormalMapping::OnKeyUp(int key)
{

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
}

void NormalMapping::OnMouseMove(const Point& MousePos)
{
	if (!bPlayState) return;

	RECT WindowSize = GetClientWindowRect();

	int Width = WindowSize.right - WindowSize.left;
	int Height = WindowSize.bottom - WindowSize.top;

	mDeltaMouseX = (float)(MousePos.X - (WindowSize.left + (static_cast<float>(Width) / 2.0f)));
	mDeltaMouseY = (float)(MousePos.Y - (WindowSize.top + (static_cast<float>(Height) / 2.0f)));

	Input::InputSystem::Get()->SetCursorPosition(Point(WindowSize.left + (int)Width / 2, WindowSize.top + (int)Height / 2));
}

void NormalMapping::DrawMesh(const MeshPtr& Mesh, const std::vector<MaterialPtr>& Materials)
{
	for (size_t m = 0; m < Mesh->GetNumMaterials(); ++m) {

		if (m >= Materials.size()) break;

		MaterialSlot Mat = Mesh->GetMaterialSlot(m);
		//Set the mesh material
		GraphicsEngine::Get()->SetMaterial(Materials[m]);
		//Set the vertex buffer
		GraphicsEngine::Get()->GetRenderSystem()->GetImmDeviceContext()->SetVertexBuffer(Mesh->GetVertexBuffer());

		//Set the index buffer
		GraphicsEngine::Get()->GetRenderSystem()->GetImmDeviceContext()->SetIndexBuffer(Mesh->GetIndexBuffer());
		GraphicsEngine::Get()->GetRenderSystem()->GetImmDeviceContext()->DrawIndexTriangleList(Mat.NumIndices, 0, Mat.StartIndex);
	}
}

void NormalMapping::OnLeftMouseButtonDown(const Point& MousePos)
{
	if (!bPlayState) {
		bPlayState = true;
		Input::InputSystem::Get()->HideCursor(!bPlayState);
	}
}

void NormalMapping::OnLeftMouseButtonUp(const Point& MousePos)
{

}

void NormalMapping::OnRightMouseButtonDown(const Point& MousePos)
{

}

void NormalMapping::OnRightMouseButtonUp(const Point& MousePos)
{

}

void NormalMapping::UpdateViewportProj()
{
	int Width = (GetClientWindowRect().right - GetClientWindowRect().left);
	int Height = (GetClientWindowRect().bottom - GetClientWindowRect().top);
	mProjCam.SetPerspectiveFOVLh(1.57f, ((float)Width / (float)Height), 0.1f, 5000.0f);
}

void NormalMapping::UpdateThirdPersonCamera()
{
	Math::Matrix4X4 WorldCam, TempMatrix;
	WorldCam.SetIdentity();

	mCameraRotation.X += mDeltaMouseY  * 0.001f;
	mCameraRotation.Y += mDeltaMouseX  * 0.001f;

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
	mCameraDistance = 2.0f;

	//Lerp between the camera distance and the current camera distance
	mCurrentCameraDistance = Math::Lerp(mCurrentCameraDistance, mCameraDistance, 2.0f * mDeltaTime);

	mCameraPosition = Math::Vector3D();

	Math::Vector3D NewPos = mCameraPosition + WorldCam.GetZDirection() * (-mCurrentCameraDistance);

	WorldCam.SetTranslation(NewPos);
	mWorldCam = WorldCam;

	WorldCam.Inverse();

	mViewCam = WorldCam;

	UpdateViewportProj();
}

void NormalMapping::UpdateModel(Math::Vector3D Position, Math::Vector3D Rotaion, Math::Vector3D Scale, const std::vector<MaterialPtr>& Materials)
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

void NormalMapping::UpdateSkyBox()
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
void NormalMapping::UpdateLight()
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


void NormalMapping::Render()
{
	//CLEAR RENDER TARGET COLOR
	GraphicsEngine::Get()->GetRenderSystem()->GetImmDeviceContext()->ClearRenderTargetColor(mSwapChain, 0.0f, 0.4f, 0.2f, 1);

	RECT rc = GetClientWindowRect();

	//SET THE VIEWPORT SIZE
	GraphicsEngine::Get()->GetRenderSystem()->GetImmDeviceContext()->SetViewportSize(rc.right - rc.left, rc.bottom - rc.top);

	//Render the base mesh
	mMaterials.clear();
	mMaterials.push_back(BrickMaterial);
	UpdateModel(Math::Vector3D(.0f, .0f, .0f), Math::Vector3D(0.0f, 0.0f, 0.0f), Math::Vector3D(1.f, 1.f, 1.f), mMaterials);
	DrawMesh(mMesh, mMaterials);


	//Render the sky sphere
	mMaterials.clear();
	mMaterials.push_back(SkySphereMaterial);
	DrawMesh(mSkyMesh, mMaterials);

	//GraphicsEngine::Get()->GetRenderSystem()->GetImmDeviceContext()->Clear

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
void NormalMapping::OnCreate()
{
	DWindow::OnCreate();

	//Register input system
	Input::InputSystem::Get()->AddListener(this);

	bPlayState = true;
	Input::InputSystem::Get()->HideCursor(false);

	//Initalize the graphics pipline
	RECT rc = GetClientWindowRect();
	mSwapChain = GraphicsEngine::Get()->GetRenderSystem()->MakeSwapChain(hwnd, rc.right - rc.left, rc.bottom - rc.top);


	/** TEXTURES  */

	//Load the sky sphere texture
	SkyTexture = GraphicsEngine::Get()->GetTextureManager()->CreatTextureFromFile(L"Assets\\Textures\\stars_map.jpg");
	BrickTexture = GraphicsEngine::Get()->GetTextureManager()->CreatTextureFromFile(L"Assets\\Textures\\brick_d.jpg");
	BrickNormalMap = GraphicsEngine::Get()->GetTextureManager()->CreatTextureFromFile(L"Assets\\Textures\\brick_n.jpg");

	/** MESHES  */
	mMesh = GraphicsEngine::Get()->GetMeshManager()->CreateMeshFromFile(L"Assets\\Meshes\\sphere.obj");

	//Load the sky box mesh
	mSkyMesh = GraphicsEngine::Get()->GetMeshManager()->CreateMeshFromFile(L"Assets\\Meshes\\sphere.obj");


	/** MATERIALS */

	//Mesh material
	MeshMaterial = GraphicsEngine::Get()->MakeMaterial(L"DirLightVertexShader.hlsl", L"DirLightPixelShader.hlsl");
	MeshMaterial->SetCullMode(ECullMode::Back);

	//Brick material
	BrickMaterial = GraphicsEngine::Get()->MakeMaterial(L"NormalMap_VertexShader_DirLight.hlsl", L"NormalMap_PixelShader_DirLight.hlsl");
	BrickMaterial->AddTexture(BrickTexture);
	BrickMaterial->SetCullMode(ECullMode::Back);
	
	//Sky sphere material
	SkySphereMaterial = GraphicsEngine::Get()->MakeMaterial(L"SkyBoxVertexShader.hlsl", L"SkyBoxShader.hlsl");
	SkySphereMaterial->AddTexture(SkyTexture);
	SkySphereMaterial->AddTexture(BrickNormalMap);
	SkySphereMaterial->SetCullMode(ECullMode::Front);

	//Set camera starting trasnlastion
	mWorldCam.SetTranslation(Math::Vector3D(.0f, .0f, -2.0f));

	mMaterials.reserve(32);
}

void NormalMapping::OnUpdate()
{
	DWindow::OnUpdate();

	Input::InputSystem::Get()->Update();

	Update();

	Render();

	mDeltaMouseX = 0.0f;
	mDeltaMouseY = 0.0f;
}

void NormalMapping::OnDestroy()
{
	DWindow::OnDestroy();
	mSwapChain->SetFullScreen(false, 1, 1);
}

void NormalMapping::OnFocus()
{
	DWindow::OnFocus();
	Input::InputSystem::Get()->AddListener(this);
}

void NormalMapping::StopFocus()
{
	DWindow::StopFocus();
	Input::InputSystem::Get()->RemoveListener(this);
}

void NormalMapping::OnSize()
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