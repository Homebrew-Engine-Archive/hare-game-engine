#include "PCH.h"
#include "Window.h"
#include "Theme.h"

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
        HARE_META(minSize, SizeF)
        HARE_META(maxSize, SizeF)
        HARE_META(shown, bool)
        HARE_META(enabled, bool)
    }

    Window::Window() : parent(0), shown(true), enabled(true), windowId(uiID_Any),
        minSize(0, 0), maxSize(100, 100), clippedByParent(false), area(0, 0, 0, 0),
        pixelRect(0, 0, 0, 0), pixelRectValid(false), unclippedRect(0, 0, 0, 0),
        unclippedRectValid(false), unclippedInnerRect(0, 0, 0, 0),
        unclippedInnerRectValid(false), innerRect(0, 0, 0, 0), 
        innerRectValid(false), pixelSize(10, 10)
    {
    }

    Window::Window(Window* parent) 
        : parent(0), shown(true), enabled(true), windowId(uiID_Any),
        minSize(0, 0), maxSize(100, 100), clippedByParent(false), area(0, 0, 0, 0),
        pixelRect(0, 0, 0, 0), pixelRectValid(false), unclippedRect(0, 0, 0, 0),
        unclippedRectValid(false), unclippedInnerRect(0, 0, 0, 0),
        unclippedInnerRectValid(false), innerRect(0, 0, 0, 0), 
        innerRectValid(false), pixelSize(10, 10)
    {
        setParent(parent);
        parent->addChild(this);
    }

    void Window::postLoaded()
    {
        if (windowSizer)
        {

        }
    }

    void Window::postEdited(Attribute* attr)
    {
        layout();
        setArea(area);
    }

    void Window::reparent(Window* window)
    {
        if (parent)
        {
            parent->removeChild(this);
        }
        
        parent = window;
        
        if (parent)
        {
            parent->addChild(this);
        }
    }

    void Window::addChild(Window* window)
    {
        children.push_back(window);
    }

    void Window::removeChild(Window* window)
    {
        children.remove(window);
    }

    void Window::setContainingSizer(Sizer* sizer)
    {
        assert(!sizer || containingSizer != sizer);
        containingSizer = sizer;
    }

    bool Window::layout()
    {
        if (windowSizer)
        {
            windowSizer->setDimension(getPixelRect());
        }

        return true;
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
        unclippedRectValid = false;
        unclippedInnerRectValid = false;
        pixelRectValid = false;
        innerRectValid = false;

        SizeF oldSize(pixelSize);
        pixelSize = size;

        // limit new pixel size to: minSize <= newSize <= maxSize
        MathUtil::clampMinMax(pixelSize.cx, minSize.cx, maxSize.cx);
        MathUtil::clampMinMax(pixelSize.cy, minSize.cy, maxSize.cy);

        area.set(area.minX, area.minY, area.minX + size.cx, area.minY + size.cy);

        bool moved = false;
        bool sized = (pixelSize != oldSize);

        if (!topLeftSizing || sized)
        {
            if (pos != PointF(area.minX, area.minY))
            {
                area.moveTo(pos);
                moved = true;
            }
        }

        // fire events as required
        if (fireEvents)
        {
            if (moved)
            {
            }

            if (sized)
            {
            }
        }
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

    void Window::render(ThemePackage* themes)
    {
        if (!shown)
            return;

        if (themes)
        {
            Theme* theme = themes->getTheme(this);
            if (theme)
            {
                theme->render(this);
            }
        }

        Window::List::iterator it = children.begin();
        for (; it != children.end(); ++it)
        {
            (*it)->render(themes);
        }
    }
}