#include "InputSystem.h"
#include "InputInterface.h"
#include <Windows.h>
#include "Point.h"

void Input::InputSystem::AddListener(InputListener* Listener)
{
    Listeners.insert(Listener);
}

void Input::InputSystem::RemoveListener(InputListener* Listener)
{
    auto it = Listeners.find(Listener);
    if (it != Listeners.end())
        Listeners.erase(it);
}

void Input::InputSystem::Update()
{
    POINT CurrentMousePos = {};

    GetCursorPos(&CurrentMousePos);

    if (bFirstTime) {
        
        OldMousePos =Point(CurrentMousePos.x, CurrentMousePos.y);
        bFirstTime = false;

    }

    if (CurrentMousePos.x != OldMousePos.X || CurrentMousePos.y != OldMousePos.Y) {
        auto it = Listeners.begin();
        while (it != Listeners.end()) {
            (*it)->OnMouseMove(Point(CurrentMousePos.x, CurrentMousePos.y));
            ++it;
        }
    }

    OldMousePos = Point(CurrentMousePos.x, CurrentMousePos.y);

    if (GetKeyboardState(KeysStates)) {
        for (unsigned int i = 0; i < 255; ++i) {
            //Key is down
            if (KeysStates[i] & 0x80) {
                auto it = Listeners.begin();
                while (it != Listeners.end()) {
                    if (i == VK_LBUTTON) {
                        if (KeysStates[i] != OldKeysStates[i]) {
                            (*it)->OnLeftMouseButtonDown(Point(CurrentMousePos.x, CurrentMousePos.y));
                        }
                    }
                    else if (i == VK_RBUTTON) {
                        if(KeysStates[i] != OldKeysStates[i]) {
                            (*it)->OnRightMouseButtonDown(Point(CurrentMousePos.x, CurrentMousePos.y));
                        }
                    }
                    else {
                        (*it)->OnKeyDown(i);
                    }
                    ++it;
                }
            }
            //key is up
            else {
                if (KeysStates[i] != OldKeysStates[i]) {
                    auto it = Listeners.begin();
                    while (it != Listeners.end()) {
                        if (i == VK_LBUTTON) {
                            (*it)->OnLeftMouseButtonUp(Point(CurrentMousePos.x, CurrentMousePos.y));
                        }
                        else if (i == VK_RBUTTON) {
                            (*it)->OnRightMouseButtonUp(Point(CurrentMousePos.x, CurrentMousePos.y));
                        }else
                            (*it)->OnKeyUp(i);
                        ++it;
                    }
                }
            }
        }
        memcpy(OldKeysStates, KeysStates, sizeof(unsigned char) * 256);
   }
}

void Input::InputSystem::SetCursorPosition(const Point& pos)
{
    SetCursorPos(pos.X, pos.Y);
}

Input::InputSystem* Input::InputSystem::Get()
{
    static InputSystem system;
    return &system;
}

void Input::InputSystem::HideCursor(bool Show)
{
    ShowCursor(Show);
}
