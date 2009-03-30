#ifndef _BUTTON_H_
#define _BUTTON_H_

#include "UIPrerequisites.h"
#include "Window.h"
#include "Theme.h"

namespace hare_ui
{
    class Button : public Window
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

    class ButtonTheme : public Theme
    {
    public:
        virtual void drawWindow(Window* window);

        virtual bool canHandle(Window* window)
        {
            return window && window->isA(&Button::CLASS_INFO);
        }

    protected:
        Material::Ptr material;

        RectF normalRect;
        RectF hoverRect;
        RectF pushedRect;
        RectF disabledRect;
    };
}

#endif