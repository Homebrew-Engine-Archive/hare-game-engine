#include "PCH.h"
#include "Window.h"

namespace hare_ui
{
    Window* Window::capturedWindow = 0;

    HARE_IMPLEMENT_ABSTRACT_CLASS(Window, Object, 0)
    {
        HARE_OBJ_LIST(children, Window)
    }

    SizeF Window::getEffectiveMinSize() const
    {
        SizeF min = getMinSize();
        if (min.cx <= 0 || min.cy <= 0)
        {
            //SizeF best = getBestSize();
            //if (min.x <= 0) min.x =  best.x;
            //if (min.y <= 0) min.y =  best.y;
        }
        return min;
    }

    float Window::adjustForLayoutDirection(float x, float width, float widthTotal) const
    {
        if (getLayoutDirection() == uiLayout_RightToLeft)
        {
            x = widthTotal - x - width;
        }

        return x;
    }
}