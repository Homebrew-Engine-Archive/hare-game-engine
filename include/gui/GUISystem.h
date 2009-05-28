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
        GUISystem();

        void setRoot(Window* window);
        Window*	getRoot() { return root; }

        void setTheme(ThemePackage* themes);
        ThemePackage* getTheme() { return themes; }

        void render();

        bool notifyMouseMove(float deltaX, float deltaY);
        bool notifyMouseWheel(float delta);
        bool notifyMousePosition(float posX, float posY);
        bool notifyMouseButtonUp(uint32 button);
        bool notifyMouseButtonDown(uint32 button);

        bool notifyKeyDown(uint32 key);
        bool notifyKeyUp(uint32 key);
        bool notifyChar(uint32 cp);

        Window*	getTargetWindow(const PointF& pt);
        Window* getKeyboardTargetWindow();

    protected:
        Window::Ptr root;
        ThemePackage::Ptr themes;

        Window* windowWithMouse;
        Window* modalTarget;

        float clickTimeout;
        float dbclickTimeout;
        SizeF dblclickSize;

        PointF mousePos;

        class TrackerTimer
        {
        public:
            TrackerTimer() : lastTime(getTime()) {}
            void reset() { lastTime = getTime(); }
            float elapsed() { return getTime() - lastTime; }
        private:
            float lastTime;
        };

        struct MouseClickTracker
        {
            MouseClickTracker() : click(0), area(0, 0, 0, 0), state(false) {}

            TrackerTimer timer;	//!< Timer used to track clicks for this button.
            int	click;	        //!< count of clicks made so far.
            RectF area;	        //!< area used to detect multi-clicks
            Window* target;     //!< target window for any events generated.
            bool state;
        };

        enum { MOUSE_BUTTON_COUNT = 5 };

        MouseClickTracker trackers[MOUSE_BUTTON_COUNT];

    private:
        MouseEvent& fillMouseEvent(MouseEvent& event, float wheelDelta = 0);
        MouseEvent  makeMouseEvent(uint32 button, uint32 type);
    };

    void UI_API gui_init();
    void UI_API gui_quit();
}

#endif
