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
        HARE_META_F(normalRect, RectUV, propHide)
        HARE_META_F(hoverRect, RectUV, propHide)
        HARE_META_F(pushedRect, RectUV, propHide)
        HARE_META_F(disabledRect, RectUV, propHide)
    }

    void ButtonTheme::drawWindow(Window* window)
    {
        if (!window)
            return;

        Button* button = (Button*)window;
        RectF* rect = NULL;

        if (!button->isEnabled())
        {
            rect = &disabledRect;
        }
        else if (button->isPushed())
        {
            rect = &pushedRect;
        }
        else if (button->isHovering())
        {
            rect = &hoverRect;
        }
        else
        {
            rect = &normalRect;
        }

        //getCanvas()->drawImage(button->getRect(), getMaterial(), *rect);
    }
}

