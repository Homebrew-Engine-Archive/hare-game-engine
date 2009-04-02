#include "PCH.h"
#include "Window.h"

namespace hare_ui
{
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

    f32 Window::adjustForLayoutDirection(f32 x, f32 width, f32 widthTotal) const
    {
        if (getLayoutDirection() == uiLayout_RightToLeft)
        {
            x = widthTotal - x - width;
        }

        return x;
    }
}