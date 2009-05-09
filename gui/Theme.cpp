#include "PCH.h"
#include "Theme.h"
#include "Window.h"

namespace hare
{
    HARE_IMPLEMENT_ABSTRACT_CLASS(Theme, Object, 0)
    {
    }

    HARE_IMPLEMENT_DYNAMIC_CLASS(ThemePackage, Object, 0)
    {
        HARE_OBJ_ARRAY(themes, Theme)
    }

    void ThemePackage::postLoaded()
    {
        themeMap.clear();
        Theme::Array::iterator it = themes.begin();
        for (; it != themes.end(); ++it)
        {
            Theme* theme = *it;
            themeMap[theme->getWindowClass()->className] = theme;
        }
    }

    Theme* ThemePackage::getTheme(Window* window)
    {
        return window ? getTheme(window->getClassInfo()->className) : NULL;
    }

    Theme* ThemePackage::getTheme(ClassInfo* classInfo)
    {
        return classInfo ? getTheme(classInfo->className) : NULL;
    }
    
    Theme* ThemePackage::getTheme(const String& className)
    {
        ThemeHashMap::iterator it = themeMap.find(className);
        if (it != themeMap.end())
            return it->second;
        return NULL;
    }
}


