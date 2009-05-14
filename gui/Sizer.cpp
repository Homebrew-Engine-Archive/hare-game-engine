#include "PCH.h"
#include "Sizer.h"
#include "Window.h"

namespace hare
{
    // ---------------------------------------------------------------
    // Sizer
    // ---------------------------------------------------------------
    HARE_IMPLEMENT_ABSTRACT_CLASS(Sizer, Object, 0)
    {
        HARE_OBJ_LIST(items, SizerItem)
        HARE_META(minSize, SizeF)
    }

    Sizer::Sizer() : containerWindow(0), size(20, 20), minSize(20, 20), position(0, 0)
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

