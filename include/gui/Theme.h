#ifndef _THEME_H_
#define _THEME_H_

#include "UIPrerequisites.h"

namespace hare
{
    /** Class for rendering one kind of control
    */
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

    typedef HashMap<size_t, Theme*> ThemeHashMap;

    /** A set of theme representing a skin
    */
    class UI_API ThemePackage : public Object
    {
        HARE_DECLARE_DYNAMIC_CLASS(ThemePackage)
    public:
        Theme* getTheme(const Window* window);
        Theme* getTheme(const ClassInfo* classInfo);

        Theme::Array& getThemes() { return themes; }

    protected:
        void postLoaded();

    protected:
        ThemeHashMap themeMap;
        Theme::Array themes;
    };

    void drawThemeInternal(Material* mtrl, const RectUV& windowRect,
        const RectUV& rect, const RectUV& rectInner, bool drawTop = true);
}

#endif
