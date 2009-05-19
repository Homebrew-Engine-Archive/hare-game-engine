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
        Window(Window* parent);

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
            shown = show;
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

        void setParent(Window* window)
        {
            parent = window;
        }

        void reparent(Window* window);

        void addChild(Window* window);
        void removeChild(Window* window);

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
            if (sizer == windowSizer)
                return;

            if (windowSizer)
            {
                windowSizer->setContainingWindow(NULL);
            }

            windowSizer = sizer;
            if (windowSizer)
            {
                windowSizer->setContainingWindow(this);
            }
        }

        Sizer* getSizer()
        {
            return windowSizer;
        }

        void setContainingSizer(Sizer* sizer);

        Sizer* getContainingSizer()
        {
            return containingSizer;
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


        SizeF getBestSize();
        virtual SizeF getBestSize_impl();
        SizeF getEffectiveMinSize();

        virtual uiLayoutDirection getLayoutDirection() const
        { 
            return uiLayout_Default; 
        }

        virtual float adjustForLayoutDirection(float x, float width, float widthTotal) const;

        virtual bool layout(); 

        virtual void postLoaded();
        virtual void postEdited(Attribute* attr);

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

        virtual void setArea_impl(const PointF& pos, const SizeF& sz);
        
        SizeF getSize() const
        {
            return size;
        }

        PointF getPosition() const
        {
            return position;
        }

        RectF getArea() const 
        { 
            return RectF(position.x, position.y, position.x + size.cx, position.y + size.cy);
        }

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
        mutable SizerPtr windowSizer;   // Sizer of this window, use Ptr to hold the ref
        Sizer* containingSizer;         // Who contains 'this' window, do NOT hold the ref

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

        PointF position;
        SizeF size;

        mutable SizeF bestSize;
        mutable bool  bestSizeValid;
        mutable RectF pixelRect;
        mutable bool  pixelRectValid;
        mutable RectF unclippedRect;
        mutable bool  unclippedRectValid;
        mutable RectF unclippedInnerRect;
        mutable bool  unclippedInnerRectValid;
        mutable RectF innerRect;
        mutable bool  innerRectValid;
    };


    PointF UI_API windowToScreen(const Window& window, const PointF& pos);
    RectF  UI_API windowToScreen(const Window& window, const RectF& rect);
}

#endif
