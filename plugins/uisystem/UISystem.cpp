#include "PCH.h"
#include "UISystem.h"
#include "Window.h"

namespace hare_ui
{
    struct MouseClickTracker
    {
        MouseClickTracker(void) : click(0), window(0) {}
        int click;
        RectF rect;
        Window* window;
    };
    MouseClickTracker clickTrackers[MouseButtonMax];

    bool UISystem::notifyMousePosition(f32 posX, f32 posY)
    {
        return notifyMouseMove(posX - mousePos.x, posY - mousePos.y);
    }

    bool UISystem::notifyMouseMove(f32 deltaX, f32 deltaY)
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

    bool UISystem::notifyMouseWheel(f32 delta)
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

        
        event.position = mousePos;

        // get the tracker that holds the number of down events seen so far for this button
        clickTrackers[button].rect.isPointIn(mousePos);
        // set click count in the event args
        ma.clickCount = tkr.d_click_count;

        Window* const initial_dest_window = getTargetWindow(ma.position);
        Window* dest_window = initial_dest_window;

        // loop backwards until event is handled or we run out of windows.
        while ((!ma.handled) && (dest_window != 0))
        {
            ma.window = dest_window;
            dest_window->onMouseButtonUp(ma);
            dest_window = getNextTargetWindow(dest_window);
        }

        bool wasUpHandled = ma.handled;

        // if requirements for click events are met
        if (((d_click_timeout == 0) || (tkr.d_timer.elapsed() <= d_click_timeout)) &&
            (tkr.d_click_area.isPointInRect(ma.position)) &&
            (tkr.d_target_window == initial_dest_window))
        {
            ma.handled = false;
            dest_window = initial_dest_window;

            // loop backwards until event is handled or we run out of windows.
            while ((!ma.handled) && (dest_window != 0))
            {
                ma.window = dest_window;
                dest_window->onMouseClicked(ma);
                dest_window = getNextTargetWindow(dest_window);
            }

        }

        return (ma.handled | wasUpHandled);
    }

    bool UISystem::notifyMouseButtonDown(MouseButton button)
    {

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
    //bool UIManager::notifyMouseWheel(f32 delta);

    //bool UIManager::notifyMouseEvent();
    //
    //bool UIManager::notifyKeyDown(u32 key);
    //bool UIManager::notifyKeyUp(u32 key);
    //bool UIManager::notifyChar(u32 cp);
}
