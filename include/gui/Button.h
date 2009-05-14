#ifndef _BUTTON_H_
#define _BUTTON_H_

#include "UIPrerequisites.h"
#include "Window.h"
#include "Theme.h"
#include "GUISystem.h"

namespace hare
{
    class UI_API Button : public Window
    {
        HARE_DECLARE_DYNAMIC_CLASS(Button)
    public:
        bool isPushed() const
        {
            return pushed;
        }
        bool isHovering() const
        {
            return hovering;
        }

    protected:
        void onMouseEvent(MouseEvent& event);

    protected:
        bool pushed;
        bool hovering;

        HARE_DECLARE_EVENT_TABLE()
    };

    class UI_API ButtonTheme : public Theme
    {
        HARE_DECLARE_DYNAMIC_CLASS(ButtonTheme)
    public:
        virtual ClassInfo* getWindowClass()
        {
            return &Button::CLASS_INFO;
        }
        virtual void render(Window* window);

    protected:
        Material::Ptr mtrl;
        RectUV rectNormal;
        RectUV rectNormalInner;
        RectUV rectHover;
        RectUV rectHoverInner;
        RectUV rectPushed;
        RectUV rectPushedInner;
        RectUV rectDisabled;
        RectUV rectDisabledInner;
    };
}

#endif