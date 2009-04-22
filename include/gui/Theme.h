#ifndef _THEME_H_
#define _THEME_H_

#include "UIPrerequisites.h"

namespace hare
{
    class UI_API Theme : public Object
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

    class UI_API MaterialTheme : public Theme
    {
        HARE_DECLARE_ABSTRACT_CLASS(MaterialTheme)
    public:
        Material* getMaterial()
        {
            return material;
        }
        void setMaterial(Material* mtrl)
        {
            material = mtrl;
        }
    protected:
        Material::Ptr material;
    };
}

#endif
