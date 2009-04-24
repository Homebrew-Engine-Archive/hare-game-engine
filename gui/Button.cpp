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
        HARE_META_F(rect_normal, RectUV, propHide)
        HARE_META_F(rect_hover, RectUV, propHide)
        HARE_META_F(rect_pushed, RectUV, propHide)
        HARE_META_F(rect_disabled, RectUV, propHide)
    }

    void ButtonTheme::drawWindow(Window* window)
    {
        if (!window)
            return;

        Button* button = (Button*)window;
        RectF* rect = NULL;

        if (!button->isEnabled())
        {
            rect = &rect_disabled;
        }
        else if (button->isPushed())
        {
            rect = &rect_pushed;
        }
        else if (button->isHovering())
        {
            rect = &rect_hover;
        }
        else
        {
            rect = &rect_normal;
        }

        //getCanvas()->drawImage(button->getRect(), getMaterial(), *rect);
    }
}

