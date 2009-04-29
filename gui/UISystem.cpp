#include "PCH.h"
#include "UISystem.h"
#include "Window.h"

namespace hare
{
    struct MouseClickTracker
    {
        MouseClickTracker(void) : click(0), window(0) {}
        int click;
        RectF rect;
        Window* window;
    };
    MouseClickTracker clickTrackers[MB_MAX];

    bool UISystem::notifyMousePosition(float posX, float posY)
    {
        return notifyMouseMove(posX - mousePos.x, posY - mousePos.y);
    }

    bool UISystem::notifyMouseMove(float deltaX, float deltaY)
    {
        updateWindowContainingMouse();

        Window* window = Window::getCapture();

        if (!window)
            window = windowWithMouse;

        if (window)
        {
            MouseEvent event(uiEVT_MOTION);
            makeMouseEvent(event);
            return window->processEvent(event);
        }

        return false;
    }

    bool UISystem::notifyMouseWheel(float delta)
    {
        MouseEvent event(uiEVT_MOUSEWHEEL);
        event.wheelDelta = delta;

        Window* window = getMouseInputWindow(mousePos);

        if (window)
            return window->processEvent(event);

        return false;
    }

    bool UISystem::notifyMouseButtonUp(MouseButton button)
    {
        Window* window = 0;

        switch (button)
        {
        case LeftButton:
            {
                MouseEvent event(uiEVT_LEFT_UP);
                window->processEvent(event);
            }
            break;
        case RightButton:
            {
                MouseEvent event(uiEVT_RIGHT_UP);
                window->processEvent(event);
            }
            break;
        case MiddleButton:
            {
                MouseEvent event(uiEVT_MIDDLE_UP);
                window->processEvent(event);
            }
            break;
        }
        
        // click event

        return true;
    }

    bool UISystem::notifyMouseButtonDown(MouseButton button)
    {
        return true;
    }


    Window* UISystem::getWindowFromPoint(const PointF& pt)
    {
        return 0;
    }

    bool UISystem::updateWindowContainingMouse()
    {
        Window* window = getWindowFromPoint(mousePos);

        if (windowWithMouse == window)
            return false;

        Window* oldWindow = windowWithMouse;
        windowWithMouse = window;

        if (oldWindow)
        {
            // mouse leave
        }

        if (windowWithMouse)
        {
            // mouse enter
        }

        return true;
    }

    //bool UIManager::notifyMouseLeaves();
    //bool UIManager::notifyMouseWheel(float delta);

    //bool UIManager::notifyMouseEvent();
    //
    //bool UIManager::notifyKeyDown(uint32 key);
    //bool UIManager::notifyKeyUp(uint32 key);
    //bool UIManager::notifyChar(uint32 cp);
}
