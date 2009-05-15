#ifndef _THEME_H_
#define _THEME_H_

#include "UIPrerequisites.h"

namespace hare
{
    class UI_API Theme : public Object
    {
        HARE_DECLARE_ABSTRACT_CLASS(Theme)
    public:
        virtual ClassInfo* getWindowClass() = 0;
        virtual void render(Window* window) = 0;

        Material* getMaterial() { return mtrl; }
        void setMaterial(Material* mat) { mtrl = mat; }

    protected:
        Material::Ptr mtrl;
    };

    typedef HashMap<String, Theme*> ThemeHashMap;

    class UI_API ThemePackage : public Object
    {
        HARE_DECLARE_DYNAMIC_CLASS(ThemePackage)
    public:
        Theme* getTheme(Window* window);
        Theme* getTheme(ClassInfo* classInfo);
        Theme* getTheme(const String& className);

        Theme::Array& getThemes() { return themes; }

    protected:
        void postLoaded();

    protected:
        ThemeHashMap themeMap;
        Theme::Array themes;
    };
}

#endif
