#ifndef _WINDOW_H_
#define _WINDOW_H_

#include "UIPrerequisites.h"
#include "Event.h"
#include "Sizer.h"

namespace hare_ui
{
    class Window : public EventHandler
    {
        HARE_DECLARE_ABSTRACT_CLASS(Window)
    public:

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

        virtual bool show(bool show = true);
        bool hide() 
        { 
            return show(false); 
        }

        virtual bool enable(bool enable = true);
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

        void setSizer(Sizer* sizer);

        Sizer* getSizer() const
        {
            return windowSizer;
        }

        void setParentSizer(Sizer* sizer)
        {
            parentSizer = sizer;
        }

        Sizer* getParentSizer() const
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

        virtual bool hitTest(const PointF& pt) const;

        virtual void setMinSize(const SizeF& size) { minSize = size; }
        virtual void setMaxSize(const SizeF& size) { maxSize = size; }

        virtual SizeF getMinSize() const { return minSize; }
        virtual SizeF getMaxSize() const { return maxSize; }

        SizeF getEffectiveMinSize() const;

        virtual uiLayoutDirection getLayoutDirection() const
        { 
            return uiLayout_Default; 
        }

        virtual f32 adjustForLayoutDirection(f32 x, f32 width, f32 widthTotal) const;

    public:
        virtual void setTitle(const String& title) = 0;
        virtual String getTitle() const = 0;

        virtual SizeF getSize() = 0;

        virtual void raise() = 0;
        virtual void lower() = 0;

    protected:
        Sizer* windowSizer; // sizer of this window
        Sizer* parentSizer; // which sizer this window belongs to

        Window::List children;
        Window* parent;

        static Window* capturedWindow;

        bool shown;
        bool enabled;

        int windowId;
        String windowName;

        SizeF minSize;
        SizeF maxSize;

        String themeName;
    };
}

#endif
