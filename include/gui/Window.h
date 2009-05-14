#ifndef _WINDOW_H_
#define _WINDOW_H_

#include "UIPrerequisites.h"
#include "Event.h"
#include "Sizer.h"

namespace hare
{
    class UI_API Window : public EventHandler
    {
        HARE_DECLARE_ABSTRACT_CLASS(Window)
    
    public:
        Window();

        void setId(int id) 
        { 
            windowId = id; 
        }
        int getId() const 
        { 
            return windowId; 
        }

        void setName(const String &name) 
        { 
            windowName = name; 
        }
        const String& getName() const 
        { 
            return windowName; 
        }

        virtual bool show(bool show = true)
        {
            return false;
        }
        bool hide() 
        { 
            return show(false); 
        }

        virtual bool enable(bool enable = true)
        {
            return false;
        }
        bool disable()
        { 
            return enable(false); 
        }

        virtual bool isShown() const 
        { 
            return shown; 
        }
        virtual bool isEnabled() const 
        { 
            return enabled; 
        }

        virtual bool isTopLevel() const
        {
            return false;
        }

        Window* getParent() const
        {
            return parent;
        }

        const Window::List& getChildren() const 
        { 
            return children; 
        }

        Window::List& getChildren() 
        { 
            return children; 
        }

        Window *findWindow(int id) const;
        Window *findWindow(const String& name) const;

        void setSizer(Sizer* sizer)
        {
            windowSizer = sizer;
        }

        Sizer* getSizer()
        {
            return windowSizer;
        }

        void setParentSizer(Sizer* sizer)
        {
            parentSizer = sizer;
        }

        Sizer* getParentSizer()
        {
            return parentSizer;
        }

        void setSize(const SizeF& size)
        {
        }

        void setPositon(const PointF& pos)
        { 
        }

        void setDimension(const PointF& ps, const SizeF& sz)
        {
        }

        void captureMouse();
        void releaseMouse();

        // get the window which currently captures the mouse or NULL
        static Window *getCapture()
        {
            return capturedWindow;
        }

        // does this window have the capture?
        virtual bool hasCapture() const
        { 
            return (Window*)this == getCapture(); 
        }

        virtual bool hitTest(const PointF& pt) const
        {
            return false;
        }

        virtual void setMinSize(const SizeF& size) { minSize = size; }
        virtual void setMaxSize(const SizeF& size) { maxSize = size; }

        virtual SizeF getMinSize() const { return minSize; }
        virtual SizeF getMaxSize() const { return maxSize; }

        SizeF getEffectiveMinSize() const;

        virtual uiLayoutDirection getLayoutDirection() const
        { 
            return uiLayout_Default; 
        }

        virtual float adjustForLayoutDirection(float x, float width, float widthTotal) const;

    public:
        void setArea(float x, float y, float width, float height)
        {
            setArea(PointF(x, y), SizeF(width, height));
        }

        void setArea(const RectF& rect)
        {
            setArea(PointF(rect.minX, rect.minY), SizeF(rect.width(), rect.height()));
        }

        void setArea(const PointF& pos, const SizeF& size)
        {
            setArea_impl(pos, size);
        }

        void setPosition(float x, float y)
        {
            setArea(PointF(x, y), getSize());
        }

        void setSize(float cx, float cy)
        {
            setArea(getPosition(), SizeF(cx, cy));
        }

        virtual void setArea_impl(const PointF& pos, const SizeF& size, bool topLeftSizing = false, bool fireEvents = true);
        
        SizeF getSize() const
        {
            return SizeF(area.width(), area.height());
        }

        PointF getPosition() const
        {
            return PointF(area.minX, area.minY);
        }

        RectF getArea() const { return area; }
        SizeF getPixelSize() const { return pixelSize; }
        RectF getPixelRect() const;
        RectF getPixelRect_impl() const;
        RectF getUnclippedPixelRect() const;
        RectF getUnclippedInnerRect() const;
        RectF getUnclippedInnerRect_impl() const;
        RectF getInnerRect() const;

        bool isClippedByParent() const { return clippedByParent; }

        virtual void raise()
        {
        }
        virtual void lower()
        {
        }

        virtual void render(ThemePackage* themes);

    protected:
        SizerPtr windowSizer;         // sizer of this window
        SizerPtr parentSizer;         // which sizer this window belongs to

        Window::List children;
        Window* parent;

        static Window* capturedWindow;

        bool shown;
        bool enabled;

        int windowId;
        String windowName;

        SizeF minSize;
        SizeF maxSize;

        bool clippedByParent;

        RectF area;

        mutable RectF pixelRect;
        mutable bool  pixelRectValid;
        mutable RectF unclippedRect;
        mutable bool  unclippedRectValid;
        mutable RectF unclippedInnerRect;
        mutable bool  unclippedInnerRectValid;
        mutable RectF innerRect;
        mutable bool  innerRectValid;

        SizeF pixelSize;
    };


    PointF windowToScreen(const Window& window, const PointF& pos);
    RectF windowToScreen(const Window& window, const RectF& rect);
}

#endif
