#pragma once

#include <Windows.h>

class DWindow
{
	//Check if the window is running or not
	bool bIsRunning{ false };
	bool bIsInit{ false };
public:
	//Init the window
	DWindow();

	bool Broadcast();
	//Check if the window is opend and running
	bool IsRunning();

	//Get the size of the window
	RECT GetClientWindowRect();
	RECT GetScreenSize();

	virtual void OnCreate();
	virtual void OnUpdate();
	virtual void OnDestroy();
	virtual void OnFocus();
	virtual void StopFocus();
	virtual void OnSize();

	//Default destructor
	~DWindow() noexcept;
protected:
	HWND hwnd = {};
};

