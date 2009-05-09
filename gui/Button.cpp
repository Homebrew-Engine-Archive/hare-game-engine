#include "PCH.h"
#include "Button.h"

namespace hare
{
    HARE_BEGIN_EVENT_TABLE(Button, Window)
        HARE_EVT_MOUSE_EVENTS(onMouseEvent)
    HARE_END_EVENT_TABLE()

    void Button::onMouseEvent(MouseEvent& event)
    {
        if (hitTest(event.getPosition()))
        {

        }

        if (hasCapture())
        {
            hovering = true;
        }
    }

    HARE_IMPLEMENT_DYNAMIC_CLASS(ButtonTheme, Theme, 0)
    {
        HARE_GUI_UV(rectNormal)
        HARE_GUI_UV(rectHover)
        HARE_GUI_UV(rectPushed)
        HARE_GUI_UV(rectDisabled)
    }

    void ButtonTheme::render(Window* window)
    {
        if (!window)
            return;
        
        RectF* rect = NULL;
        RectF* rectInner = NULL;
        Button* button = (Button*)window;

        if (!button->isEnabled())
        {
            rect = &rectDisabled;
        }
        else if (button->isPushed())
        {
            rect = &rectPushed;
        }
        else if (button->isHovering())
        {
            rect = &rectHover;
        }
        else
        {
            rect = &rectNormal;
        }

        getCanvas()->drawImage(button->getPixelRect(), mtrl, *rect);
    }
}

