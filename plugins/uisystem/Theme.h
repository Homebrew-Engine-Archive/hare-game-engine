#ifndef _THEME_H_
#define _THEME_H_

#include "UIPrerequisites.h"

namespace hare_ui
{
    class Theme : public Object
    {
        HARE_DECLARE_ABSTRACT_CLASS(Theme)
    public:
        virtual void drawWindow(Window* window) = 0;

        virtual bool canHandle(Window* window)
        {
            return false;
        }

    protected:
        String name;
    };

    class ThemePackage
    {


    protected:
        Theme::Array themes;
    };
}

#endif
