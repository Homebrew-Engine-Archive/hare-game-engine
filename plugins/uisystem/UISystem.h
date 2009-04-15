#ifndef UISYSTEM_H
#define UISYSTEM_H

#include "UIPrerequisites.h"
#include "Event.h"

typedef TRect<float> RectUV;

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
        bool notifyMouseMove(float deltaX, float deltaY);
        bool notifyMouseWheel(float delta);
        bool notifyMousePosition(float posX, float posY);
        bool notifyMouseButtonUp(MouseButton button);
        bool notifyMouseButtonDown(MouseButton button);

        bool notifyKeyDown(uint32 key);
        bool notifyKeyUp(uint32 key);
        bool notifyChar(uint32 cp);

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
