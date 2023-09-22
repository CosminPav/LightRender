#pragma once

#include <Windows.h>

class DWindow
{
	//Check if the window is running or not
	bool bIsRunning{ false };

public:
	DWindow();

	bool Broadcast();
	//Check if the window is opend and running
	inline bool IsRunning() { return bIsRunning; }

	//Get the size of the window
	RECT GetClientWindowRect();

	//Set the window handle
	void SetWindowHandle(HWND HandleToSet) {hwnd = HandleToSet; }

	virtual void OnCreate();
	virtual void OnUpdate();
	virtual void OnDestroy();
	virtual void OnFocus();
	virtual void StopFocus();

	//Default destructor
	~DWindow() noexcept;
protected:
	HWND hwnd = {};
};

