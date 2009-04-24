#ifndef _BUTTON_H_
#define _BUTTON_H_

#include "UIPrerequisites.h"
#include "Window.h"
#include "Theme.h"
#include "UISystem.h"

namespace hare
{
    class UI_API Button : public Window
    {
    public:
        bool isPushed() const
        {
            return pushed;
        }
        bool isHovering() const
        {
            return hovering;
        }

    protected:
        void onMouseEvent(MouseEvent& event);

    protected:
        bool pushed;
        bool hovering;

        HARE_DECLARE_EVENT_TABLE()
    };

    class UI_API ButtonTheme : public MaterialTheme
    {
        HARE_DECLARE_DYNAMIC_CLASS(ButtonTheme)

    public:
        virtual void drawWindow(Window* window);

        virtual bool canHandle(Window* window)
        {
            return window && window->isA(&Button::CLASS_INFO);
        }

    protected:
        RectUV rect_normal;
        RectUV rect_hover;
        RectUV rect_pushed;
        RectUV rect_disabled;
    };
}

#endif