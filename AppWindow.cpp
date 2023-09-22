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

#include "InputSystem.h"
#include "Vector3D.h"
#include "Matrix4X4.h"
#include "Point.h"

struct Vertex
{
	Math::Vector3D Position;
	Math::Vector3D Color;
	Math::Vector3D Color_1;
};

__declspec(align(16))
struct Constants
{
	Math::Matrix4X4 World;
	Math::Matrix4X4 View;
	Math::Matrix4X4 Projection;
	unsigned int mAngle{ 0 };
	//float mAngle{ 0.0f };
};

AppWindow::AppWindow()
{
}

void AppWindow::Update()
{
	//Calculate the delta time
	/*unsigned long NewTime = 0;
	if (mOldTime)
		NewTime = static_cast<unsigned long>(GetTickCount64()) - mOldTime;
	mDeltaTime = NewTime / 1000.0f;
	mOldTime = static_cast<unsigned long>(GetTickCount64());

	mAngle += 1.75f * mDeltaTime;*/
	Constants constant;
	constant.mAngle = static_cast<unsigned int>(GetTickCount64());

	mDeltaPos += mDeltaTime / 10.5f;
	if (mDeltaPos > 1.0f)
		mDeltaPos = 0.0f;

	Math::Matrix4X4 TempMatrix;

	//Delta scale
	mDeltaScale += mDeltaTime / 0.55f;
	
	//Setup scale animation
	/*
	//constant.World.SetScale(Math::Vector3D::Lerp(Math::Vector3D(0.5f, 0.5f, 0.0f), Math::Vector3D(1.0f, 1.0f, 0.0f), (sin(mDeltaScale) + 1.0f)/2.0f));

	//TempMatrix.SetTranslation(Math::Vector3D::Lerp(Math::Vector3D(-1.5f, -1.5f, 0.0f), Math::Vector3D(1.5f, 1.5f, 0.0f), mDeltaPos));

	//constant.World *= TempMatrix;

	*/
	
	//constant.World.SetScale(Math::Vector3D(Scale, Scale, Scale));

	//Setup rotation animation through the rotation matrix
	/*
	{
		TempMatrix.SetIdentity();
		TempMatrix.SetRotationZ(0.0f);
		constant.World *= TempMatrix;

		TempMatrix.SetIdentity();
		TempMatrix.SetRotationY(RotationY);
		constant.World *= TempMatrix;

		TempMatrix.SetIdentity();
		TempMatrix.SetRotationX(RotationX);
		constant.World *= TempMatrix;
	}
	*/
	
	constant.World.SetIdentity();

	Math::Matrix4X4 WorldCam;
	WorldCam.SetIdentity();

	TempMatrix.SetIdentity();
	TempMatrix.SetRotationX(RotationX);
	WorldCam *= TempMatrix;
	

	TempMatrix.SetIdentity();
	TempMatrix.SetRotationY(RotationY);
	WorldCam *= TempMatrix;

	Math::Vector3D NewPos = mWorldCam.GetTranslation() + WorldCam.GetZDirection() * (Forward * 0.3f);
	NewPos = NewPos + WorldCam.GetXDirection() * (Right * 0.3f);

	WorldCam.SetTranslation(NewPos);

	mWorldCam = WorldCam;

	WorldCam.Inverse();

	constant.View = WorldCam;

	//constant.Projection.SetOrthiH((GetClientWindowRect().right - GetClientWindowRect().left)/400.f,
								//(GetClientWindowRect().bottom - GetClientWindowRect().top)/400.f, -4.f, 4.f);

	int Width = GetClientWindowRect().right - GetClientWindowRect().left;
	int Height = GetClientWindowRect().bottom - GetClientWindowRect().top;

	constant.Projection.SetPerspectiveFOVLh(1.57f, static_cast<float>(Width / Height), 0.1f, 100.f);

	//Update the constant buffer
	mConstantBuffer->Update(GraphicsEngine::Get()->GetRenderSystem()->GetImmDeviceContext(), &constant);

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
}

void AppWindow::OnMouseMove(const Point& MousePos)
{
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

void AppWindow::OnCreate()
{
	DWindow::OnCreate();

	//Register input system
	Input::InputSystem::Get()->AddListener(this);
	Input::InputSystem::Get()->HideCursor(false);

	//Initalize the graphics pipline
	RECT rc = GetClientWindowRect();
	mSwapChain = GraphicsEngine::Get()->GetRenderSystem()->MakeSwapChain(hwnd, rc.right - rc.left, rc.bottom - rc.top);

	mWorldCam.SetTranslation(Math::Vector3D(0.0f, 0.0f, -2.0f));

	//Vertex list
	Vertex VList[] =
	{
		//FRONT FACE
		{Math::Vector3D (-0.5f, -0.5f, -0.5f),		Math::Vector3D(1,0,0),		Math::Vector3D(0,1,0)},	
		{Math::Vector3D (-0.5f, 0.5f, -0.5f),		Math::Vector3D(0,1,0),		Math::Vector3D(1,1,0)},	
		{Math::Vector3D(0.5f, 0.5f, -0.5f),			Math::Vector3D(0,0,1),		Math::Vector3D(1,0,0)},	
		{Math::Vector3D(0.5f, -0.5f, -0.5f),		Math::Vector3D(1,1,0),		Math::Vector3D(0,0,1)},	

		//Back face
		{Math::Vector3D(0.5f, -0.5f, 0.5f),			Math::Vector3D(1,0,0),		Math::Vector3D(0,1,0)},
		{Math::Vector3D(0.5f, 0.5f, 0.5f),			Math::Vector3D(0,1,0),		Math::Vector3D(1,1,0)},
		{Math::Vector3D(-0.5f, 0.5f, 0.5f),			Math::Vector3D(0,0,1),		Math::Vector3D(1,0,0)},
		{Math::Vector3D(-0.5f, -0.5f, 0.5f),		Math::Vector3D(1,1,0),		Math::Vector3D(0,0,1)},
	};

	unsigned int IndexList[] =
	{
		//Front side
		0,1,2,
		2,3,0,
		//Back side
		4,5,6,
		6,7,4,
		//Top
		1,6,5,
		5,2,1,
		//Bottom
		7,0,3,
		3,4,7,
		//Right side
		3,2,5,
		5,4,3,
		//Left side
		7,6,1,
		1,0,7
	};


	//INDEX BUFFER
	mIndexBuffer = GraphicsEngine::Get()->GetRenderSystem()->MakeIndexBuffer(IndexList, ARRAYSIZE(IndexList));

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
	mVertexBuffer = GraphicsEngine::Get()->GetRenderSystem()->CreateVertexBuffer(VList, sizeof(Vertex), ARRAYSIZE(VList), ShaderByteCode, static_cast<UINT>(SizeOfShader));

	GraphicsEngine::Get()->GetRenderSystem()->ReleaseCompiledShaders();

	//PIXEL SHADER
	
	//Compile the pixel shader file
	GraphicsEngine::Get()->GetRenderSystem()->CompilePixelShader(L"PixelShader.hlsl", "psmain", &ShaderByteCode, &SizeOfShader);
	//Make the pixel shader
	mPixelShader = GraphicsEngine::Get()->GetRenderSystem()->MakePixelShader(ShaderByteCode, SizeOfShader);
	//release the pixel shader
	GraphicsEngine::Get()->GetRenderSystem()->ReleaseCompiledShaders();


	Constants constant;
	constant.mAngle = 0;

	//CONSTANT BUFFER
	//Make the constant buffer
	mConstantBuffer = GraphicsEngine::Get()->GetRenderSystem()->MakeConstantBuffer(&constant, sizeof(constant));
}

void AppWindow::OnUpdate()
{
	DWindow::OnUpdate();

	Input::InputSystem::Get()->Update();

	//CLEAR RENDER TARGET COLOR
	GraphicsEngine::Get()->GetRenderSystem()->GetImmDeviceContext()->ClearRenderTargetColor(mSwapChain, 0.0f, 0.4f, 0.2f, 1);

	RECT rc = GetClientWindowRect();

	//SET THE VIEWPORT SIZE
	GraphicsEngine::Get()->GetRenderSystem()->GetImmDeviceContext()->SetViewportSize(rc.right - rc.left, rc.bottom - rc.top);
	
	//Update the quad pos
	Update();

	//Set the constant buffer with vertex and pixel shader
	GraphicsEngine::Get()->GetRenderSystem()->GetImmDeviceContext()->SetConstantBuffer(mVertexShader, mConstantBuffer);
	GraphicsEngine::Get()->GetRenderSystem()->GetImmDeviceContext()->SetConstantBuffer(mPixelShader, mConstantBuffer);

	//SET THE VERTEX AND PIXEL SHADERS
	GraphicsEngine::Get()->GetRenderSystem()->GetImmDeviceContext()->SetVertexShader(mVertexShader);
	GraphicsEngine::Get()->GetRenderSystem()->GetImmDeviceContext()->SetPixelShader(mPixelShader);

	//Set the vertex buffer
	GraphicsEngine::Get()->GetRenderSystem()->GetImmDeviceContext()->SetVertexBuffer(mVertexBuffer);

	//Set the index buffer
	GraphicsEngine::Get()->GetRenderSystem()->GetImmDeviceContext()->SetIndexBuffer(mIndexBuffer);

	//GraphicsEngine::Get()->GetImmDeviceContext()->DrawTriangleStrip(mVertexBuffer->GetVertexSize(), 0);
	GraphicsEngine::Get()->GetRenderSystem()->GetImmDeviceContext()->DrawIndexTriangleList(mIndexBuffer->GetSizeIndexList(), 0, 0);

	mSwapChain->Preseant(true);


	//Delta time computations
	mOldDeltaTime = mNewDeltaTime;
	mNewDeltaTime = static_cast<float>(GetTickCount64());
	mDeltaTime = (mNewDeltaTime) ? ((mNewDeltaTime - mOldDeltaTime) / 1000.0f) : 0;

}

void AppWindow::OnDestroy()
{
	DWindow::OnDestroy();	
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

