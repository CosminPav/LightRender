#include "AppWindow.h"
#include "GraphicsEngine.h"
#include <iostream>

int main()
{
	try {
		GraphicsEngine::Create();
	}
	catch (...) {
		return 1;
	}


	AppWindow App;
	while (App.IsRunning()) {
		App.Broadcast();	
	}
	
	GraphicsEngine::Release();
	return 0;
} 