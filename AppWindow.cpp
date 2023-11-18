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
	float Time = 0.0f;
};

AppWindow::AppWindow()
{
}

void AppWindow::Update()
{
	UpdateCamera();
	UpdateModel();
	UpdateSkyBox();
}

void AppWindow::OnKeyDown(int key)
{
	if (key == 'W' || key == 'w') {
		//RotationX += 3.19f * mDeltaTime;
		Forward = 1.0f;
	}
	else if (key == 'S' || key == 's') {
		//RotationX -= 3.19f * mDeltaTime;
		Forward = -1.0f;
	}else if (key == 'A' || key == 'a') {
		//RotationY += 3.19f * mDeltaTime;
		Right = -1.0f;
	}
	else if (key == 'D' || key == 'd') {
		//RotationY -= 3.19f * mDeltaTime;
		Right = 1.0f;
	}
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

void AppWindow::DrawMesh(const MeshPtr& Mesh, const VertexShaderPtr& vertexShader, const PixelShaderPtr& pixelShader, const ConstantBufferPtr& constantBuffer, const TexturePtr* textureList, UINT NumTextures)
{
	//Set the constant buffer with vertex and pixel shader
	GraphicsEngine::Get()->GetRenderSystem()->GetImmDeviceContext()->SetConstantBuffer(vertexShader, constantBuffer);
	GraphicsEngine::Get()->GetRenderSystem()->GetImmDeviceContext()->SetConstantBuffer(pixelShader, constantBuffer);

	//SET THE VERTEX AND PIXEL SHADERS
	GraphicsEngine::Get()->GetRenderSystem()->GetImmDeviceContext()->SetVertexShader(vertexShader);
	GraphicsEngine::Get()->GetRenderSystem()->GetImmDeviceContext()->SetPixelShader(pixelShader);

	//Set texture
	GraphicsEngine::Get()->GetRenderSystem()->GetImmDeviceContext()->SetTexture(pixelShader, textureList, NumTextures);

	//Set the vertex buffer
	GraphicsEngine::Get()->GetRenderSystem()->GetImmDeviceContext()->SetVertexBuffer(Mesh->GetVertexBuffer());

	//Set the index buffer
	GraphicsEngine::Get()->GetRenderSystem()->GetImmDeviceContext()->SetIndexBuffer(Mesh->GetIndexBuffer());

	//GraphicsEngine::Get()->GetImmDeviceContext()->DrawTriangleStrip(mVertexBuffer->GetVertexSize(), 0);
	GraphicsEngine::Get()->GetRenderSystem()->GetImmDeviceContext()->DrawIndexTriangleList(Mesh->GetIndexBuffer()->GetSizeIndexList(), 0, 0);
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

	Math::Vector3D NewPos = mWorldCam.GetTranslation() + WorldCam.GetZDirection() * (Forward * 0.1f);
	NewPos = NewPos + WorldCam.GetXDirection() * (Right * 0.1f);

	WorldCam.SetTranslation(NewPos);
	mWorldCam = WorldCam;

	WorldCam.Inverse();

	mViewCam = WorldCam;
	int Width = GetClientWindowRect().right - GetClientWindowRect().left;
	int Height = GetClientWindowRect().bottom - GetClientWindowRect().top;

	mProjCam.SetPerspectiveFOVLh(1.57f, static_cast<float>(Width / Height), 0.1f, 100.f);
}

void AppWindow::UpdateModel()
{
	Constants constant;

	//Light rotation matrix
	Math::Matrix4X4 LightRotMatrix;
	LightRotMatrix.SetIdentity();
	LightRotMatrix.SetRotationY(RotationY_Light);

	//go about 45 degrees
	RotationY_Light += 0.307 * mDeltaTime;

	constant.World.SetIdentity();

	constant.View = mViewCam;
	constant.Projection = mProjCam;
	constant.CameraPosition = mWorldCam.GetTranslation();

	constant.LightDirection = LightRotMatrix.GetZDirection();

	constant.Time = mTime;
	//Update the constant buffer
	mConstantBuffer->Update(GraphicsEngine::Get()->GetRenderSystem()->GetImmDeviceContext(), &constant);
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
	mSkyConstantBuffer->Update(GraphicsEngine::Get()->GetRenderSystem()->GetImmDeviceContext(), &constant);
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

	//Set the model face culling(back face culling, i.e. render the outside, cull the inside faces)
	GraphicsEngine::Get()->GetRenderSystem()->SetRazterizerState(false);

	//Texture list
	TexturePtr TexList[4];
	TexList[0] = EarthColorTexture;
	TexList[1] = EarthSpecularTexture;
	TexList[2] = EarthCloudsTexture;
	TexList[3] = EarthColorTexture_Night;

	//Render the earth model
	DrawMesh(mMesh, mVertexShader, mPixelShader, mConstantBuffer, TexList, 4);

	//Sky sphere face culling set to front face culling
	GraphicsEngine::Get()->GetRenderSystem()->SetRazterizerState(true);

	//Update the list for the sky texture
	TexList[0] = SkyTexture;

	//Render the sky sphere
	DrawMesh(mSkyMesh, mVertexShader, mSkyPixelShader, mSkyConstantBuffer, TexList, 1);

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
	EarthColorTexture = GraphicsEngine::Get()->GetTextureManager()->CreatTextureFromFile(L"Assets\\Textures\\earth_color.jpg");
	EarthSpecularTexture = GraphicsEngine::Get()->GetTextureManager()->CreatTextureFromFile(L"Assets\\Textures\\earth_spec.jpg");
	EarthCloudsTexture = GraphicsEngine::Get()->GetTextureManager()->CreatTextureFromFile(L"Assets\\Textures\\clouds.jpg");
	EarthColorTexture_Night = GraphicsEngine::Get()->GetTextureManager()->CreatTextureFromFile(L"Assets\\Textures\\earth_night.jpg");

	//Load the sky sphere texture
	SkyTexture = GraphicsEngine::Get()->GetTextureManager()->CreatTextureFromFile(L"Assets\\Textures\\stars_map.jpg");

	//Load the actual static mesh
	mMesh = GraphicsEngine::Get()->GetMeshManager()->CreateMeshFromFile(L"Assets\\Meshes\\sphere.obj");

	//Load the sky box mesh
	mSkyMesh = GraphicsEngine::Get()->GetMeshManager()->CreateMeshFromFile(L"Assets\\Meshes\\sphere.obj");

	//Initalize the graphics pipline
	RECT rc = GetClientWindowRect();
	mSwapChain = GraphicsEngine::Get()->GetRenderSystem()->MakeSwapChain(hwnd, rc.right - rc.left, rc.bottom - rc.top);

	mWorldCam.SetTranslation(Math::Vector3D(0.0f, 0.0f, -1.0f));

	{
		//Math::Vector3D PosList[] =
	//{
	//	{Math::Vector3D(-0.5f, -0.5f, -0.5f)},
	//	{Math::Vector3D(-0.5f, 0.5f, -0.5f)},
	//	{Math::Vector3D(0.5f, 0.5f, -0.5f)},
	//	{Math::Vector3D(0.5f, -0.5f, -0.5f)},

	//	//Back face
	//	{Math::Vector3D(0.5f, -0.5f, 0.5f)},
	//	{Math::Vector3D(0.5f, 0.5f, 0.5f)},
	//	{Math::Vector3D(-0.5f, 0.5f, 0.5f)},
	//	{Math::Vector3D(-0.5f, -0.5f, 0.5f)}
	//};


	//Math::Vector2D TextureList[] =
	//{
	//	{Math::Vector2D(0.0f, 0.0f)},
	//	{Math::Vector2D(0.0f, 1.0f)},
	//	{Math::Vector2D(1.0f, 0.0f)},
	//	{Math::Vector2D(1.0f, 1.0f)}
	//};

	////Vertex list
	//Vertex VList[] =
	//{
	//	//FRONT FACE
	//	{PosList[0], TextureList[1]},
	//	{PosList[1], TextureList[0]},
	//	{PosList[2], TextureList[2]},
	//	{PosList[3], TextureList[3]},

	//	//Back Face
	//	{PosList[4], TextureList[1]},
	//	{PosList[5], TextureList[0]},
	//	{PosList[6], TextureList[2]},
	//	{PosList[7], TextureList[3]},

	//	{PosList[1], TextureList[1]},
	//	{PosList[6], TextureList[0]},
	//	{PosList[5], TextureList[2]},
	//	{PosList[2], TextureList[3]},

	//	{PosList[3], TextureList[1]},
	//	{PosList[2], TextureList[0]},
	//	{PosList[5], TextureList[2]},
	//	{PosList[4], TextureList[3]},

	//	{PosList[7], TextureList[1]},
	//	{PosList[6], TextureList[0]},
	//	{PosList[1], TextureList[2]},
	//	{PosList[0], TextureList[3]}

	//};

	//unsigned int IndexList[] =
	//{
	//	//Front side
	//	0,1,2,
	//	2,3,0,
	//	//Back side
	//	4,5,6,
	//	6,7,4,
	//	//Top
	//	8,9,10,
	//	10,11,8,
	//	//Bottom
	//	12,13,14,
	//	14,15,12,
	//	//Right side
	//	16,17,18,
	//	18,19,16,
	//	//Left side
	//	20,21,22,
	//	22,23,20
	//};


	////INDEX BUFFER
	//mIndexBuffer = GraphicsEngine::Get()->GetRenderSystem()->MakeIndexBuffer(IndexList, ARRAYSIZE(IndexList));
	}
	//ptr to mem buffer
	void* ShaderByteCode = nullptr;
	//the zize if mem buffer in bytes
	size_t SizeOfShader = 0;

	//VERTEX SHADER

	//compile the vertex shader from file
	GraphicsEngine::Get()->GetRenderSystem()->CompileVertexShader(L"VertexShader.hlsl", "vsmain", &ShaderByteCode, &SizeOfShader);

	//Create the verrtex shader
	mVertexShader = GraphicsEngine::Get()->GetRenderSystem()->MakeVertexShader(ShaderByteCode, SizeOfShader);

	//make the vertex buffer
	//mVertexBuffer = GraphicsEngine::Get()->GetRenderSystem()->CreateVertexBuffer(VList, sizeof(Vertex), ARRAYSIZE(VList), ShaderByteCode, static_cast<UINT>(SizeOfShader));

	GraphicsEngine::Get()->GetRenderSystem()->ReleaseCompiledShaders();

	//PIXEL SHADER
	
	//Compile the pixel shader file
	GraphicsEngine::Get()->GetRenderSystem()->CompilePixelShader(L"PixelShader.hlsl", "psmain", &ShaderByteCode, &SizeOfShader);
	//Make the pixel shader
	mPixelShader = GraphicsEngine::Get()->GetRenderSystem()->MakePixelShader(ShaderByteCode, SizeOfShader);
	//release the pixel shader
	GraphicsEngine::Get()->GetRenderSystem()->ReleaseCompiledShaders();

	/** SKY SPHERE PIXEL SHADER */

	//Compile the pixel shader file
	GraphicsEngine::Get()->GetRenderSystem()->CompilePixelShader(L"SkyBoxShader.hlsl", "psmain", &ShaderByteCode, &SizeOfShader);
	//Make the pixel shader
	mSkyPixelShader = GraphicsEngine::Get()->GetRenderSystem()->MakePixelShader(ShaderByteCode, SizeOfShader);
	//release the pixel shader
	GraphicsEngine::Get()->GetRenderSystem()->ReleaseCompiledShaders();

	Constants constant;

	//CONSTANT BUFFER
	//Make the constant buffer
	mConstantBuffer = GraphicsEngine::Get()->GetRenderSystem()->MakeConstantBuffer(&constant, sizeof(constant));

	//Make the sky sphere's constant buffer
	mSkyConstantBuffer = GraphicsEngine::Get()->GetRenderSystem()->MakeConstantBuffer(&constant, sizeof(constant));

	mWorldCam.SetTranslation(Math::Vector3D(0.0f, 0.0f, -2.0f));
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

