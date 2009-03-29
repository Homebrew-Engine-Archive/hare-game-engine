#include "PCH.h"

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

    getCanvas()->drawImage(getPixelRect(), material, rect);
}