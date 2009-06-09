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

    PointF screenToWindow(const Window& window, const PointF& pos)
    {
        return pos - windowToScreen(window, PointF(0, 0));
    }

    RectF screenToWindow(const Window& window, const RectF& rect)
    {
        RectF temp(rect);
        PointF winPos = windowToScreen(window, PointF(0, 0));
        temp.moveTo(-winPos);
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
        HARE_META_F(size, SizeF, propHide)
        HARE_META(minSize, SizeF)
        HARE_META(maxSize, SizeF)
        HARE_META(shown, bool)
        HARE_META(enabled, bool)
        HARE_OBJ_F(scriptRunner, ScriptRunner, propAllowSubs)
    }

    Window::Window() : 
        parent(0), 
        shown(true), 
        enabled(true), 
        active(false),
        windowId(uiID_Any),
        minSize(-1, -1), 
        maxSize(-1, -1),
        clippedByParent(false), 
        distCapturedInputs(false), 
        wantsMultiClicks(true),
        mousePassThroughEnabled(false),
        position(0, 0), 
        size(20, 20), 
        pixelRect(0, 0, 0, 0), 
        pixelRectValid(false), 
        unclippedRect(0, 0, 0, 0),
        unclippedRectValid(false), 
        unclippedInnerRect(0, 0, 0, 0),
        unclippedInnerRectValid(false), 
        innerRect(0, 0, 0, 0),
        innerRectValid(false), 
        bestSize(-1, -1), 
        bestSizeValid(false)
    {
        eventHandler = this;
    }

    Window::Window(Window* parent, int32 id) : 
        parent(parent), 
        shown(true), 
        enabled(true), 
        active(false),
        windowId(id),
        minSize(-1, -1), 
        maxSize(-1, -1), 
        clippedByParent(false), 
        distCapturedInputs(false), 
        wantsMultiClicks(true),
        mousePassThroughEnabled(false),
        position(0, 0),
        size(20, 20), 
        pixelRect(0, 0, 0, 0), 
        pixelRectValid(false), 
        unclippedRect(0, 0, 0, 0),
        unclippedRectValid(false), 
        unclippedInnerRect(0, 0, 0, 0),
        unclippedInnerRectValid(false), 
        innerRect(0, 0, 0, 0),
        innerRectValid(false), 
        bestSize(-1, -1), 
        bestSizeValid(false)
    {
        eventHandler = this;
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

            if (size.cx < 0 && size.cy < 0)
            {
                windowSizer->fit(this);
            }
        }

        if (scriptRunner && scriptRunner->isLoaded())
        {
            scriptRunner->setOwner(this);
            scriptRunner->notifyOwnerCreated();
        }

        windowId = newId(windowName);
    }

    void Window::postEdited(Attribute* attr)
    {
        layout();
        setArea(position, size);
    }

    bool Window::tryParent(Event& event)
    {
        if (event.shouldPropagate())
        {
            Window *parent = getParent();
            if (parent)
            {
                PropagateOnce locker(event);
                return parent->getEventHandler()->processEvent(event);
            }
        }

        return EventHandler::tryParent(event);
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
        
        if (minSize.cx > 0)
        {
            MathUtil::clampMin(size.cx, minSize.cx);
        }
        if (minSize.cy > 0)
        {
            MathUtil::clampMin(size.cy, minSize.cy);
        }
        if (maxSize.cx > 0)
        {
            MathUtil::clampMax(size.cx, maxSize.cx);
        }
        if (maxSize.cy > 0)
        {
            MathUtil::clampMax(size.cy, maxSize.cy);
        }
    }

    void Window::setSizeHints(const SizeF& newMinSize, const SizeF& newMaxSize)
    {
        minSize = newMinSize;
        maxSize = newMaxSize;
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
        RectF rect = getUnclippedPixelRect();

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
        RectF rect = getUnclippedPixelRect();

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

    Window* Window::getTargetChildAtPosition(const PointF& pt)
    {
        Window::List::reverse_iterator it = children.rbegin();

        for (; it != children.rend(); ++it)
        {
            Window* child = *it;
            if (child->isShown())
            {
                Window* wnd = child->getTargetChildAtPosition(pt);

                if (wnd)
                {
                    return wnd;
                }
                else if (!child->isMousePassThroughEnabled() && child->hitTest(pt))
                {
                    return child;
                }
            }
        }

        return 0;
    }

    bool Window::distributesCapturedInputs() const
    {
        return distCapturedInputs;
    }

    bool Window::wantsMultiClickEvents() const
    {
        return wantsMultiClicks;
    }

    bool Window::isMousePassThroughEnabled() const
    {
        return mousePassThroughEnabled;
    }

    bool Window::isAncestor(const Window* window) const
    {
        if (!parent)
        {
            return false;
        }

        if (parent == window)
        {
            return true;
        }

        return parent->isAncestor(window);
    }

    Window* Window::getActiveChild()
    {
        if (!isActive())
        {
            return 0;
        }

        Window::List::reverse_iterator it = children.rbegin();

        for (; it != children.rend(); ++it)
        {
            Window* child = *it;
            if (child->isActive())
            {
                return child->getActiveChild();
            }
        }

        return this;
    }

    bool Window::isActive() const
    {
        bool parentActive = (parent == 0) ? true : parent->isActive();

        return active && parentActive;
    }

    void Window::captureMouse()
    {
        if (capturedWindow)
        {

        }

        capturedWindow = this;
    }

    void Window::releaseMouse()
    {

        capturedWindow = NULL;
    }
}
