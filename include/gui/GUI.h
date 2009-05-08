#ifndef GUI_H
#define GUI_H

#include "UIPrerequisites.h"
#include "Event.h"

namespace hare
{
    class UI_API GUI
    {
    public:
        void setRoot(Window* window);
        Window*	getRoot() const	{ return root; }

        bool notifyMouseMove(float deltaX, float deltaY);
        bool notifyMouseWheel(float delta);
        bool notifyMousePosition(float posX, float posY);
        bool notifyMouseButtonUp(MouseButton button);
        bool notifyMouseButtonDown(MouseButton button);

        bool notifyKeyDown(uint32 key);
        bool notifyKeyUp(uint32 key);
        bool notifyChar(uint32 cp);

        Window* getWindowFromPoint(const PointF& pt);

        Window* getMouseInputWindow(const PointF& pt)
        {
            return 0;
        }

        bool updateWindowContainingMouse();

    private:
        Window* root;
        Window* windowWithMouse;
        PointF mousePos;
    };
}

#endif
