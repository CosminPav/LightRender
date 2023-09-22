#include "Window.h"
#include <memory>
#include <exception>

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
	//GetWindowLong(hwnd,)
	switch (msg)
	{
	case WM_CREATE:
	{
		// Event fired when the window is created
		// collected here..
		DWindow* window = (DWindow*)((LPCREATESTRUCT)lparam)->lpCreateParams;
		// .. and then stored for later lookup
		SetWindowLongPtr(hwnd, GWLP_USERDATA, (LONG_PTR)window);
		window->SetWindowHandle(hwnd);
		window->OnCreate();
		break;
	}

	case WM_DESTROY:
	{
		//Event fired when the window is destroyed
		DWindow* window = (DWindow*)GetWindowLongPtr(hwnd, GWLP_USERDATA);
		window->OnDestroy();
		PostQuitMessage(0);
		break;
	}
	case WM_SETFOCUS:
	{
		DWindow* window = (DWindow*)GetWindowLongPtr(hwnd, GWLP_USERDATA);
		window->OnFocus();
		break;
	}
	case WM_KILLFOCUS:
	{
		DWindow* window = (DWindow*)GetWindowLongPtr(hwnd, GWLP_USERDATA);
		window->StopFocus();
		break;
	}

	default:
		return ::DefWindowProc(hwnd, msg, wparam, lparam);
	}

	return 0;
}


DWindow::DWindow()
{
	//Setting up WNDCLASSEX object
	WNDCLASSEX wc;
	wc.cbClsExtra = NULL;
	wc.cbSize = sizeof(WNDCLASSEX);
	wc.cbWndExtra = NULL;
	wc.hbrBackground = (HBRUSH)COLOR_WINDOW;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wc.hIconSm = LoadIcon(NULL, IDI_APPLICATION);
	wc.hInstance = NULL;
	wc.lpszClassName = L"MyWindowClass";
	wc.lpszMenuName = L"";
	wc.style = NULL;
	wc.lpfnWndProc = &WndProc;

	if (!RegisterClassEx(&wc)) // If the registration of class will fail, the function will return false
	{
		throw std::exception("Window failed to create");
	}

	//Creation of the window
	hwnd = CreateWindowEx(0, L"MyWindowClass", L"DirectX Application",
		WS_CAPTION | WS_SYSMENU | WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, 1024, 768,
		NULL, NULL, NULL, this);

	//if the creation fail return false
	if (!hwnd)
	{
		throw std::exception("Hwnd is false");
	}

	//show up the window
	ShowWindow(hwnd, SW_SHOW);
	UpdateWindow(hwnd);

	//set this flag to true to indicate that the window is initialized and running
	bIsRunning = true;
}



bool DWindow::Broadcast()
{
	MSG msg;

	this->OnUpdate();

	while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE) > 0)
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	Sleep(1);

	return true;
}

RECT DWindow::GetClientWindowRect()
{
	RECT rc;
	GetClientRect(hwnd, &rc);
	return rc;
}


void DWindow::OnCreate()
{
}

void DWindow::OnUpdate()
{
}

void DWindow::OnDestroy()
{
	bIsRunning = false;
}

void DWindow::OnFocus()
{
}

void DWindow::StopFocus()
{
}

DWindow::~DWindow() noexcept
{
	DestroyWindow(hwnd);
}
