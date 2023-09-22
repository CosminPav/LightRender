#pragma once
#include "Point.h"

class InputListener
{
public:
	InputListener() = default;

	virtual void OnKeyDown(int key) = 0;
	virtual void OnKeyUp(int key) = 0;
	
	//Mouse movement
	virtual void OnMouseMove(const Point& MousePos) = 0;
	
	//mouse buttons
	virtual void OnLeftMouseButtonDown(const Point& MousePos) = 0;
	virtual void OnLeftMouseButtonUp(const Point& MousePos) = 0;

	virtual void OnRightMouseButtonDown(const Point& MousePos) = 0;
	virtual void OnRightMouseButtonUp(const Point& MousePos) = 0;
	~InputListener() = default;
};