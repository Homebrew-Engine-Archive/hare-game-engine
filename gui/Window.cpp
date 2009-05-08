#include "PCH.h"
#include "Window.h"

namespace hare
{
    PointF windowToScreen(const Window& window, const PointF& pos)
    {
        Window* parent = window.getParent();
        if (parent)
            return windowToScreen(*parent, window.getPosition() + pos);
        else
            return window.getPosition() + pos;
    }

    RectF windowToScreen(const Window& window, const RectF& rect)
    {
        RectF temp(rect);
        temp.move(windowToScreen(window, PointF(0, 0)));
        return temp;
    }

    //////////////////////////////////////////////////////////////////////////
    Window* Window::capturedWindow = 0;

    HARE_IMPLEMENT_ABSTRACT_CLASS(Window, EventHandler, 0)
    {
        HARE_OBJ_F(windowSizer, Sizer, propHide)
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

    void Window::setArea_impl(const PointF& pos, const SizeF& size, bool topLeftSizing, bool fireEvents)
    {
    }

    RectF Window::getPixelRect() const
    {
        if (!pixelRectValid)
        {
            pixelRect = getPixelRect_impl();
            pixelRectValid = true;
        }

        return pixelRect;
    }

    RectF Window::getPixelRect_impl() const
    {
        RectF& rect = getUnclippedPixelRect();

        if (parent && isClippedByParent())
        {
            rect.intersect(rect, parent->getInnerRect());
        }
        return rect;
    }

    RectF Window::getUnclippedPixelRect() const
    {
        if (!unclippedRectValid)
        {
            RectF localArea(0, 0, pixelSize.cx, pixelSize.cy);
            unclippedRect = windowToScreen(*this, localArea);
            unclippedRectValid = true;
        }

        return unclippedRect;
    }

    RectF Window::getUnclippedInnerRect() const
    {
        if (!unclippedInnerRectValid)
        {
            unclippedInnerRect = getUnclippedInnerRect_impl();
            unclippedInnerRectValid = true;
        }

        return unclippedInnerRect;
    }

    RectF Window::getUnclippedInnerRect_impl() const
    {
        return getUnclippedPixelRect();
    }

    RectF Window::getInnerRect() const
    {
        RectF& rect = getUnclippedPixelRect();

        if (!innerRectValid)
        {
            if (parent && isClippedByParent())
            {
                innerRect.intersect(rect, parent->getInnerRect());
            }
            else
            {
                innerRect = rect;
            }
            innerRectValid = true;
        }

        return innerRect;
    }
}