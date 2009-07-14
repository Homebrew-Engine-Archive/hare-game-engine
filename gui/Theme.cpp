#include "PCH.h"
#include "Theme.h"
#include "Window.h"

namespace hare
{
    HARE_IMPLEMENT_ABSTRACT_CLASS(Theme, Object, 0)
    {
        HARE_OBJ_F(mtrl, Material, propImport)
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
            themeMap[theme->getWindowClass()] = theme;
        }
    }

    Theme* ThemePackage::getTheme(const Window* window)
    {
        return window ? getTheme(window->getClassInfo()) : NULL;
    }

    Theme* ThemePackage::getTheme(const ClassInfo* classInfo)
    {
        if (!classInfo)
            return NULL;

        if (classInfo->isDerivedFrom(&Window::CLASS_INFO))
        {
            ThemeHashMap::iterator it = themeMap.find(classInfo);

            if (it != themeMap.end())
            {
                return it->second;
            }
            else
            {
                return getTheme(classInfo->getBaseClass());
            }
        }

        return NULL;
    }

    void drawThemeInternal(Material* mtrl, const RectUV& windowRect, const RectUV& rect, const RectUV& rectInner, bool drawTop)
    {
        RectF uvRect(0, 0, 0, 0);

        float w = (float)mtrl->getTextureMtrl()->getTexture()->getWidth();
        float h = (float)mtrl->getTextureMtrl()->getTexture()->getHeight();

        // Lefttop Corner
        uvRect.set(rect.minX, rect.minY, rectInner.minX, rectInner.minY);
        RectF ltRect = uvRect;
        ltRect.scale(w, h);
        ltRect.moveTo(windowRect.minX, windowRect.minY);
        if (drawTop)
            getCanvas()->drawImage(ltRect, mtrl, uvRect);

        // Righttop Corner
        uvRect.set(rectInner.maxX, rect.minY, rect.maxX, rectInner.minY);
        RectF rtRect = uvRect;
        rtRect.scale(w, h);
        rtRect.moveTo(windowRect.maxX - rtRect.width(), windowRect.minY);
        if (drawTop)
            getCanvas()->drawImage(rtRect, mtrl, uvRect);

        // Top edge
        uvRect.set(rectInner.minX, rect.minY, rectInner.maxX, rectInner.minY);
        RectF tRect = uvRect;
        tRect.scale(w, h);
        tRect.moveTo(windowRect.minX + ltRect.width(), windowRect.minY);
        tRect.maxX = windowRect.maxX - rtRect.width();
        if (drawTop)
            getCanvas()->drawImage(tRect, mtrl, uvRect);

        // Leftbottom Corner
        uvRect.set(rect.minX, rectInner.maxY, rectInner.minX, rect.maxY);
        RectF lbRect = uvRect;
        lbRect.scale(w, h);
        lbRect.moveTo(windowRect.minX, windowRect.maxY - lbRect.height());
        getCanvas()->drawImage(lbRect, mtrl, uvRect);

        // Left edge
        uvRect.set(rect.minX, rectInner.minY, rectInner.minX, rectInner.maxY);
        RectF lRect = uvRect;
        lRect.scale(w, h);
        lRect.moveTo(windowRect.minX, windowRect.minY + ltRect.height());
        lRect.maxY = windowRect.maxY - lbRect.height();
        getCanvas()->drawImage(lRect, mtrl, uvRect);

        // Rightbottom Corner
        uvRect.set(rectInner.maxX, rectInner.maxY, rect.maxX, rect.maxY);
        RectF rbRect = uvRect;
        rbRect.scale(w, h);
        rbRect.moveTo(windowRect.maxX - rbRect.width(), windowRect.maxY - rbRect.height());
        getCanvas()->drawImage(rbRect, mtrl, uvRect);

        // Bottom edge
        uvRect.set(rectInner.minX, rectInner.maxY, rectInner.maxX, rect.maxY);
        RectF bRect = uvRect;
        bRect.scale(w, h);
        bRect.moveTo(windowRect.minX + lbRect.width(), windowRect.maxY - bRect.height());
        bRect.maxX = windowRect.maxX - rbRect.width();
        getCanvas()->drawImage(bRect, mtrl, uvRect);

        // Right edge
        uvRect.set(rectInner.maxX, rectInner.minY, rect.maxX, rectInner.maxY);
        RectF rRect = uvRect;
        rRect.scale(w, h);
        rRect.moveTo(windowRect.maxX - rtRect.width(), windowRect.minY + rtRect.height());
        rRect.maxY = windowRect.maxY - rbRect.height();
        getCanvas()->drawImage(rRect, mtrl, uvRect);

        // Center
        uvRect = rectInner;
        RectF cRect;
        cRect.minX = windowRect.minX + ltRect.width();
        cRect.minY = windowRect.minY + ltRect.height();
        cRect.maxX = windowRect.maxX - rbRect.width();
        cRect.maxY = windowRect.maxY - rbRect.height();
        getCanvas()->drawImage(cRect, mtrl, uvRect);
    }

}


