#include "PCH.h"
#include "GUISystem.h"
#include "Window.h"
#include "MouseCursor.h"

namespace hare
{
    HARE_IMPLEMENT_DYNAMIC_CLASS(GUISystem, Object, 0)
    {
        HARE_OBJ_F(themes, ThemePackage, propImport)
    }

    GUISystem::GUISystem() : windowWithMouse(0), modalTarget(0), mousePos(0, 0),
        clickTimeout(0.2f), dbclickTimeout(0.33f), dblclickSize(12, 12)
    {
    }

    MouseEvent& GUISystem::fillMouseEvent(MouseEvent& event, float wheelDelta)
    {
        event.leftDown = trackers[0].state;
        event.rightDown = trackers[1].state;
        event.middleDown = trackers[2].state;
        event.wheelDelta = wheelDelta;
        event.position = mousePos;

        return event;
    }

    enum MouseEventType { MouseDown, MouseUp, MouseClick, MouseDClick, MouseTClick, MouseMAX, };

    MouseEvent GUISystem::makeMouseEvent(uint32 button, uint32 type)
    {
        static const EventType types[] = {
            uiEVT_LEFT_DOWN,
            uiEVT_LEFT_UP,
            uiEVT_LEFT_CLICK,
            uiEVT_LEFT_DCLICK,
            uiEVT_LEFT_TCLICK,

            uiEVT_RIGHT_DOWN,
            uiEVT_RIGHT_UP,
            uiEVT_RIGHT_CLICK,
            uiEVT_RIGHT_DCLICK,
            uiEVT_RIGHT_TCLICK,

            uiEVT_MIDDLE_DOWN,
            uiEVT_MIDDLE_UP,
            uiEVT_MIDDLE_CLICK,
            uiEVT_MIDDLE_DCLICK,
            uiEVT_MIDDLE_TCLICK,
        };

        MouseEvent e(types[button * MouseMAX + type]);
        fillMouseEvent(e);
        return e;
    }

    bool GUISystem::notifyMousePosition(float posX, float posY)
    {
        mousePos.x = posX;
        mousePos.y = posY;
        return notifyMouseMove(0, 0);
    }

    bool GUISystem::notifyMouseMove(float deltaX, float deltaY)
    {
        mousePos.x += deltaX;
        mousePos.y += deltaY;

        Window* dest = getTargetWindow(mousePos);

        // has window containing mouse changed?
        if (dest != windowWithMouse)
        {
            // store previous window that contained mouse
            Window* oldWindow = windowWithMouse;

            // set the new window that contains the mouse.
            windowWithMouse = dest;

            // inform previous window the mouse has left it
            if (oldWindow)
            {
                MouseEvent event(uiEVT_LEAVE_WINDOW);
                return oldWindow->processEvent(fillMouseEvent(event));
            }

            // inform window containing mouse that mouse has entered it
            if (windowWithMouse)
            {
                MouseEvent event(uiEVT_ENTER_WINDOW);
                return windowWithMouse->processEvent(fillMouseEvent(event));
            }
        }

        // inform appropriate window of the mouse movement event
        if (dest)
        {
            MouseEvent event(uiEVT_MOTION);
            return dest->processEvent(fillMouseEvent(event));
        }

        return false;
    }

    bool GUISystem::notifyMouseWheel(float delta)
    {
        Window* dest = getTargetWindow(mousePos);

        if (dest)
        {
            MouseEvent event(uiEVT_MOUSEWHEEL);
            return dest->processEvent(fillMouseEvent(event, delta));
        }

        return false;
    }

    bool GUISystem::notifyMouseButtonUp(uint32 button)
    {
        if (button < MOUSE_BUTTON_COUNT)
        {
            MouseClickTracker& tkr = trackers[button];
            tkr.state = false;

            Window* dest = getTargetWindow(mousePos);

            bool upHandled = false;
            bool clickHandled = false;

            if (dest)
            {
                MouseEvent e = makeMouseEvent(button, MouseUp);
                upHandled = dest->processEvent(e);

                if (tkr.timer.elapsed() <= clickTimeout &&
                    tkr.area.isPointIn(mousePos) &&
                    tkr.target == dest)
                {
                    MouseEvent e = makeMouseEvent(button, MouseClick);
                    clickHandled = dest->processEvent(e);
                }
            }

            return (upHandled | clickHandled);
        }

        return false;
    }

    bool GUISystem::notifyMouseButtonDown(uint32 button)
    {
        if (button < MOUSE_BUTTON_COUNT)
        {
            Window* dest = getTargetWindow(mousePos);

            MouseClickTracker& tkr = trackers[button];
            tkr.state = true;

            tkr.click++;

            if (tkr.timer.elapsed() > dbclickTimeout ||
               !tkr.area.isPointIn(mousePos) ||
                tkr.target != dest ||
                tkr.click > 3)
            {
                // reset to single down event.
                tkr.click = 1;

                // build new allowable area for multi-clicks
                tkr.area.set(mousePos.x, mousePos.y, mousePos.x + dblclickSize.cx, mousePos.y + dblclickSize.cy);
                tkr.area.move(-dblclickSize.cx / 2, -dblclickSize.cy / 2);

                // set target window for click events on this tracker
                tkr.target = dest;
            }

            bool handled = false;

            if (dest)
            {
                if (dest->wantsMultiClickEvents())
                {
                    if (tkr.click == 1)
                    {
                        MouseEvent e = makeMouseEvent(button, MouseDown);
                        handled = dest->processEvent(e);
                    }
                    else if (tkr.click == 2)
                    {
                        MouseEvent e = makeMouseEvent(button, MouseDClick);
                        handled = dest->processEvent(e);
                    }
                    else if (tkr.click == 3)
                    {
                        MouseEvent e = makeMouseEvent(button, MouseTClick);
                        handled = dest->processEvent(e);
                    }
                }
                else
                {
                    MouseEvent e = makeMouseEvent(button, MouseDown);
                    handled = dest->processEvent(e);
                }
            }

            tkr.timer.reset();

            return handled;
        }

        return false;
    }

    bool GUISystem::notifyKeyDown(uint32 key)
    {
        if (root)
        {
            Window* dest = getKeyboardTargetWindow();

            if (dest)
            {
                KeyEvent event(uiEVT_KEY_DOWN);
                event.keyCode = key;
                return dest->processEvent(event);
            }
        }

        return false;
    }

    bool GUISystem::notifyKeyUp(uint32 key)
    {
        if (root)
        {
            Window* dest = getKeyboardTargetWindow();

            if (dest)
            {
                KeyEvent event(uiEVT_KEY_UP);
                event.keyCode = key;
                return dest->processEvent(event);
            }
        }

        return false;
    }

    bool GUISystem::notifyChar(uint32 cp)
    {
        if (root)
        {
            Window* dest = getKeyboardTargetWindow();

            if (dest)
            {
                KeyEvent event(uiEVT_CHAR);
                event.text = cp;
                return dest->processEvent(event);
            }
        }

        return false;
    }

    Window*	GUISystem::getTargetWindow(const PointF& pt)
    {
        Window* dest = 0;

        if (root)
        {
            dest = Window::getCapture();

            if (!dest)
            {
                dest = root->getTargetChildAtPosition(pt);

                if (!dest)
                {
                    dest = root;
                }
            }
            else
            {
                if (dest->distributesCapturedInputs())
                {
                    Window* child = dest->getTargetChildAtPosition(pt);

                    if (child)
                    {
                        dest = child;
                    }

                }
            }

            // modal target overrules
            if (modalTarget != 0 && dest != modalTarget)
            {
                if (!dest->isAncestor(modalTarget))
                {
                    dest = modalTarget;
                }
            }
        }

        return dest;
    }

    Window* GUISystem::getKeyboardTargetWindow()
    {
        Window* dest = 0;

        if (!modalTarget)
        {
            dest = root->getActiveChild();
        }
        else
        {
            dest = modalTarget->getActiveChild();
            if (!dest)
            {
                dest = modalTarget;
            }
        }

        return dest;
    }

    void GUISystem::setRoot(Window* window)
    {
        this->root = window;
    }

    void GUISystem::setTheme(ThemePackage* themes)
    {
        this->themes = themes;
    }

    void GUISystem::render()
    {
        if (root)
        {
            root->render(themes);
        }

        Sprite* sprite = MouseCursor::getSingleton().getCursorSprite();

        if (sprite)
        {
            sprite->moveTo(mousePos.x, mousePos.y);
            sprite->render();
        }
    }

    MouseCursor* theMouse = NULL;

    void UI_API gui_init()
    {
        if (!theMouse)
        {
            theMouse = new MouseCursor();
        }
    }

    void UI_API gui_quit()
    {
        SAFE_DELETE(theMouse);
    }
}
