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

    Sizer::Sizer() : containingWindow(0), size(0, 0), minSize(-1, -1), position(0, 0)
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

        if (item->getWindow())
            item->getWindow()->setContainingSizer(this);

        if (item->getSizer())
            item->getSizer()->setContainingWindow(containingWindow);

        return item;
    }

    void Sizer::setContainingWindow(Window *window)
    {
        if (window == containingWindow)
            return;

        containingWindow = window;

        // set the same window for all nested sizers as well, they also are in the
        // same window
        SizerItem::List::iterator it = items.begin();
        for (; it != items.end(); ++it)
        {
            SizerItem* item = *it;
            Sizer* sizer = item->getSizer();
            if (sizer)
            {
                sizer->setContainingWindow(window);
            }
        }
    }

    void Sizer::postLoaded()
    {
        SizerItem::List::iterator it = items.begin();
        for (; it != items.end(); ++it)
        {
            SizerItem* item = *it;

            if (item->getWindow())
                item->getWindow()->setContainingSizer(this);

            if (item->getSizer())
                item->getSizer()->setContainingWindow(containingWindow);
        }
    }

    SizeF Sizer::getMinSize()
    {
        SizeF ret(calcMinSize());
        if (ret.cx < minSize.cx) ret.cx = minSize.cx;
        if (ret.cy < minSize.cy) ret.cy = minSize.cy;
        return ret;
    }

    void Sizer::layout()
    {
        calcMinSize();

        recalcSizes();
    }
}

