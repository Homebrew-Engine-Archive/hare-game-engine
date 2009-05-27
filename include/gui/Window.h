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
        Window(Window* parent, int32 id = uiID_Any);

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

        EventHandler* getEventHandler() const 
        {
            return eventHandler;
        }

        void setEventHandler(EventHandler* handler)
        {
            eventHandler = handler;
        }

        virtual bool tryParent(Event& event);

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
            enabled = enable;
            return true;
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

        Window* getTargetChildAtPosition(const PointF& pt);

        bool distributesCapturedInputs() const;
        bool wantsMultiClickEvents() const;
        bool isMousePassThroughEnabled() const;

        bool isActive() const;

        bool isAncestor(const Window* window) const;

        Window* getActiveChild();

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
            if (!isEnabled())
                return false;

            RectF clippedArea(getPixelRect());

            if (clippedArea.width() == 0)
                return false;

            return clippedArea.isPointIn(pt);
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

        virtual void _doRelease()
        {
            if (scriptRunner && scriptRunner->isLoaded())
            {
                scriptRunner->notifyOwnerDestroyed();
                scriptRunner->setOwner(NULL);
                scriptRunner = NULL;
            }

            delete this;
        }

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

        void setPosition(const PointF& pos)
        {
            setArea(pos, getSize());
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

        ScriptRunner* getScriptRunner() { return scriptRunner; }

    protected:
        mutable SizerPtr windowSizer;   // Sizer of this window, use Ptr to hold the ref
        Sizer* containingSizer;         // Who contains 'this' window, do NOT hold the ref

        Window::List children;
        Window* parent;

        static Window* capturedWindow;

        EventHandler* eventHandler;

        bool shown;
        bool enabled;
        bool active;

        int32 windowId;
        String windowName;

        SizeF minSize;
        SizeF maxSize;

        bool clippedByParent;

        bool distCapturedInputs;
        bool wantsMultiClicks;
        bool mousePassThroughEnabled;

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

        ScriptRunner::Ptr scriptRunner;
    };


    PointF UI_API windowToScreen(const Window& window, const PointF& pos);
    RectF  UI_API windowToScreen(const Window& window, const RectF& rect);
    PointF UI_API screenToWindow(const Window& window, const PointF& pos);
    RectF  UI_API screenToWindow(const Window& window, const RectF& rect);
}

#endif
