#pragma once
#include "Point.h"
#include <unordered_set>

class InputListener;
class Point;

namespace Input
{
	class InputSystem 
	{
		static InputSystem* mInput;

		//std::map< InputListener*, InputListener*> Listeners;
		std::unordered_set<InputListener*> Listeners;
		unsigned char KeysStates[256] = {};
		unsigned char OldKeysStates[256] = {};

		Point OldMousePos;

		bool bFirstTime{ true };

		InputSystem();
		~InputSystem();
	public:
		void AddListener(InputListener* Listener);
		void RemoveListener(InputListener* Listener);

		void SetCursorPosition(const Point& pos);
		void HideCursor(bool Show);

		void Update();
	
		static void Create();
		static void Release();

		static InputSystem* Get();
	};
}


