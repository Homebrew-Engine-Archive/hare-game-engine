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

    void reparentAllBySizer(Window* parent, Sizer* sizer)
    {
        if (!sizer)
            return;

        SizerItem::List& items = sizer->getChildren();
        SizerItem::List::iterator it = items.begin();
        for (; it != items.end(); ++it)
        {
            SizerItem* item = *it;

            if (item->getSizer())
                reparentAllBySizer(parent, item->getSizer());

            if (item->getWindow())
                item->getWindow()->reparent(parent);
        }
    }

    //////////////////////////////////////////////////////////////////////////
    Window* Window::capturedWindow = 0;

    HARE_IMPLEMENT_ABSTRACT_CLASS(Window, EventHandler, 0)
    {
        HARE_OBJ_F(windowSizer, Sizer, propHide)
        HARE_META_F(position, PointF, propHide)
        HARE_META_F(size, PointF, propHide)
        HARE_META(minSize, SizeF)
        HARE_META(maxSize, SizeF)
        HARE_META(shown, bool)
        HARE_META(enabled, bool)
    }

    Window::Window() : parent(0), shown(true), enabled(true), windowId(uiID_Any),
        minSize(-1, -1), maxSize(-1, -1), clippedByParent(false), position(0, 0),
        size(20, 20), pixelRect(0, 0, 0, 0), pixelRectValid(false), unclippedRect(0, 0, 0, 0),
        unclippedRectValid(false), unclippedInnerRect(0, 0, 0, 0),
        unclippedInnerRectValid(false), innerRect(0, 0, 0, 0), 
        innerRectValid(false), bestSize(-1, -1), bestSizeValid(false)
    {
    }

    Window::Window(Window* parent) 
        : parent(0), shown(true), enabled(true), windowId(uiID_Any),
        minSize(-1, -1), maxSize(-1, -1), clippedByParent(false), position(0, 0),
        size(20, 20), pixelRect(0, 0, 0, 0), pixelRectValid(false), unclippedRect(0, 0, 0, 0),
        unclippedRectValid(false), unclippedInnerRect(0, 0, 0, 0),
        unclippedInnerRectValid(false), innerRect(0, 0, 0, 0), 
        innerRectValid(false), bestSize(-1, -1), bestSizeValid(false)
    {
        setParent(parent);
        parent->addChild(this);
    }

    void Window::postLoaded()
    {
        // After window loaded, if we have a sizer that means we may have child
        // windows, we need to set its parent and add the child to the list.
        if (windowSizer)
        {
            windowSizer->setContainingWindow(this);

            reparentAllBySizer(this, windowSizer);
        }
    }

    void Window::postEdited(Attribute* attr)
    {
        layout();
        setArea(position, size);
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
            windowSizer->setDimension(PointF(0, 0), size);
        }

        return true;
    }

    SizeF Window::getEffectiveMinSize()
    {
        SizeF min = getMinSize();
        if (min.cx < 0 || min.cy < 0)
        {
            SizeF best = getBestSize();
            if (min.cx < 0) min.cx =  best.cx;
            if (min.cy < 0) min.cy =  best.cy;
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

    void Window::setArea_impl(const PointF& pos, const SizeF& sz)
    {
        unclippedRectValid = false;
        unclippedInnerRectValid = false;
        pixelRectValid = false;
        innerRectValid = false;
        bestSizeValid = false;

        position = pos;
        size = sz;
    }

    SizeF Window::getBestSize()
    {
        if (!bestSizeValid)
        {
            bestSize = getBestSize_impl();
            bestSizeValid = true;
        }

        return bestSize;
    }

    SizeF Window::getBestSize_impl()
    {
        SizeF best(-1, -1);

        if (windowSizer)
        {
            best = windowSizer->getMinSize();
        }
        else if (children.size() == 0)
        {
            best = getMinSize();
            if (best.cx < 0 || best.cy < 0)
            {
                if (best.cx < 0) best.cx = getSize().cx;
                if (best.cy < 0) best.cy = getSize().cy;
                setMinSize(best);
            }
        }

        return best;
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
            RectF localArea(0, 0, size.cx, size.cy);
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