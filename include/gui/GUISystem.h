#ifndef _GUISYSTEM_H_
#define _GUISYSTEM_H_

#include "UIPrerequisites.h"
#include "Window.h"
#include "Event.h"
#include "Theme.h"

namespace hare
{
    class UI_API GUISystem : public Object
    {
        HARE_DECLARE_DYNAMIC_CLASS(GUISystem)
    public:
        void setRoot(Window* window);
        Window*	getRoot() { return root; }

        void setTheme(ThemePackage* themes);
        ThemePackage* getTheme() { return themes; }

        void render();

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
        Window::Ptr root;
        ThemePackage::Ptr themes;

        Window* windowWithMouse;
        PointF mousePos;
    };
}

#endif
