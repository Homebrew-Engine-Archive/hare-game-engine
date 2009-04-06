#ifndef UISYSTEM_H
#define UISYSTEM_H

#include "UIPrerequisites.h"
#include "Event.h"

namespace hare_ui
{
    enum MouseButton
    {
        LeftButton,
        RightButton,
        MiddleButton,
        MouseButtonMax,
    };

    class UI_API UISystem : public Singleton<UISystem>
    {
    public:
        bool notifyMouseMove(f32 deltaX, f32 deltaY);
        bool notifyMouseWheel(f32 delta);
        bool notifyMousePosition(f32 posX, f32 posY);
        bool notifyMouseButtonUp(MouseButton button);
        bool notifyMouseButtonDown(MouseButton button);

        bool notifyKeyDown(u32 key);
        bool notifyKeyUp(u32 key);
        bool notifyChar(u32 cp);

        Window* getWindowFromPoint(const PointF& pt);

        Window* getMouseInputWindow(const PointF& pt);

        bool updateWindowContainingMouse();

    private:
        void makeMouseEvent(MouseEvent& event);

    private:
        Window* windowWithMouse;
        PointF mousePos;


    };
}

#endif
