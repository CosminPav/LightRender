//#include "AppWindow.h"
#include "GraphicsEngine.h"
#include "InputSystem.h"
#include "NormalMapping.h"

int main()
{
	try {
	
		GraphicsEngine::Create();
		Input::InputSystem::Create();
	}
	catch (...) {
		return -1;
	}

	try {
		NormalMapping App;
		while (App.IsRunning());
	}
	catch (...) {
		Input::InputSystem::Release();
		GraphicsEngine::Release();
		return 1;
	}

	//RELEASE THE INPUT SYSTEM AND THE GRAPHICS ENGINE WHEN THE WINDOW MESSAGE IS 0
	Input::InputSystem::Release();
	GraphicsEngine::Release();
	
	return 0;
} 
