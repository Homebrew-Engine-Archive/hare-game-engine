#include "PCH.h"
#include "Sizer.h"
#include "Window.h"

namespace hare
{
    HARE_ENUM_BEGIN(uiEnumFlags)
        HARE_ENUM_VALUE(uiAlign_Top)
        HARE_ENUM_VALUE(uiAlign_Bottom)
        HARE_ENUM_VALUE(uiAlign_Left)
        HARE_ENUM_VALUE(uiAlign_Right)
        HARE_ENUM_VALUE(uiAlign_Center_Vertical)
        HARE_ENUM_VALUE(uiAlign_Center_Horizontal)
        HARE_ENUM_VALUE(uiAlign_Center)
        HARE_ENUM_VALUE(uiWest)
        HARE_ENUM_VALUE(uiEast)
        HARE_ENUM_VALUE(uiNorth)
        HARE_ENUM_VALUE(uiSouth)
        HARE_ENUM_VALUE(uiAll_Direction)
        HARE_ENUM_VALUE(uiExpand)
        HARE_ENUM_VALUE(uiShaped)
        HARE_ENUM_VALUE(uiFixed_Minsize)
    HARE_ENUM_END()

    // ---------------------------------------------------------------
    // Sizer
    // ---------------------------------------------------------------
    HARE_IMPLEMENT_ABSTRACT_CLASS(Sizer, Object, 0)
    {
        HARE_OBJ_LIST(items, SizerItem)
        HARE_META(size, SizeF)
        HARE_META(minSize, SizeF)
        HARE_META(position, PointF)
    }

    Sizer::Sizer()
    {
    }

    Sizer::~Sizer()
    {
    }

    SizerItem* Sizer::add(float width, float height, int proportion /* = 0 */, int flag /* = 0 */, int border /* = 0 */)
    {
        return insert(items.size(), new SizerItemSpacer(width, height, proportion, flag, border));
    }

    SizerItem* Sizer::add(Sizer *sizer, int proportion /* = 0 */, int flag /* = 0 */, int border /* = 0 */)
    {
        return insert(items.size(), new SizerItemSizer(sizer, proportion, flag, border));
    }

    SizerItem* Sizer::add(Window *window, int proportion /* = 0 */, int flag /* = 0 */, int border /* = 0 */)
    {
        return insert(items.size(), new SizerItemWindow(window, proportion, flag, border));
    }

    SizerItem* Sizer::insert(size_t index, float width, float height, int proportion /* = 0 */, int flag /* = 0 */, int border /* = 0 */)
    {
        return insert(index, new SizerItemSpacer(width, height, proportion, flag, border));
    }

    SizerItem* Sizer::insert(size_t index, Sizer *sizer, int proportion /* = 0 */, int flag /* = 0 */, int border /* = 0 */)
    {
        return insert(index, new SizerItemSizer(sizer, proportion, flag, border));
    }

    SizerItem* Sizer::insert(size_t index, Window *window, int proportion /* = 0 */, int flag /* = 0 */, int border /* = 0 */)
    {
        return insert(index, new SizerItemWindow(window, proportion, flag, border));
    }

    SizerItem* Sizer::insert(size_t index, SizerItem* item)
    {
        if (index >= items.size())
        {
            items.push_back(item);
        }
        else
        {
            SizerItem::List::iterator it = items.begin();
            while (index--) it++;
            items.insert(it, item);
        }

        return item;
    }

    void Sizer::layout()
    {
        calcMinSize();

        recalcSizes();
    }
}

