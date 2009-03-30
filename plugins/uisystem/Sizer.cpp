#include "PCH.h"
#include "Sizer.h"
#include "Window.h"

namespace hare_ui
{
    // ---------------------------------------------------------------
    // SizerItem
    // ---------------------------------------------------------------
    HARE_IMPLEMENT_ABSTRACT_CLASS(SizerItem, Object, 0)
    {
    }

    SizerItem::SizerItem(int prop, int flg, int bdr)
        : proportion(prop), flag(flg), border(bdr)
    {
    }

    SizerItem::~SizerItem()
    {
    }

    SizeF SizerItem::getMinSize() const
    {
        return minSize;
    }

    SizeF SizerItem::getMinSizeWithBorder() const
    {
        SizeF ret = minSize;

        if (flag & uiWest)
            ret.cx += border;
        if (flag & uiEast)
            ret.cx += border;
        if (flag & uiNorth)
            ret.cy += border;
        if (flag & uiSouth)
            ret.cy += border;

        return ret;
    }

    // ---------------------------------------------------------------
    // SizerItemWindow
    // ---------------------------------------------------------------
    HARE_IMPLEMENT_ABSTRACT_CLASS(SizerItemWindow, SizerItem, 0)
    {
    }
    SizeF SizerItemWindow::getSize() const
    {   
        SizeF ret = window->getSize();

        if (flag & uiWest)
            ret.cx += border;
        if (flag & uiEast)
            ret.cx += border;
        if (flag & uiNorth)
            ret.cy += border;
        if (flag & uiSouth)
            ret.cy += border;

        return ret;
    }

    SizeF SizerItemWindow::calcMinSize()
    {
        minSize = window->getEffectiveMinSize();

        return getMinSizeWithBorder();
    }

    // ---------------------------------------------------------------
    // SizerItemSizer
    // ---------------------------------------------------------------
    HARE_IMPLEMENT_ABSTRACT_CLASS(SizerItemSizer, SizerItem, 0)
    {
    }
    SizeF SizerItemSizer::getSize() const
    {   
        SizeF ret = sizer->getSize();

        if (flag & uiWest)
            ret.cx += border;
        if (flag & uiEast)
            ret.cx += border;
        if (flag & uiNorth)
            ret.cy += border;
        if (flag & uiSouth)
            ret.cy += border;

        return ret;
    }

    SizeF SizerItemSizer::calcMinSize()
    {
        minSize = sizer->getMinSize();

        return getMinSizeWithBorder();
    }

    bool SizerItemSizer::isShown()
    {
        if (sizer->getChildren().size() == 0)
            return true;

        SizerItem::List::iterator it = sizer->getChildren().begin();
        for (; it != sizer->getChildren().end(); ++it)
        {
            if ((*it)->isShown())
                return true;
        }

        return false;
    }

    // ---------------------------------------------------------------
    // SizerItemSpacer
    // ---------------------------------------------------------------
    HARE_IMPLEMENT_ABSTRACT_CLASS(SizerItemSpacer, SizerItem, 0)
    {
    }
    SizeF SizerItemSpacer::getSize() const
    {   
        SizeF ret = spacer->getSize();

        if (flag & uiWest)
            ret.cx += border;
        if (flag & uiEast)
            ret.cx += border;
        if (flag & uiNorth)
            ret.cy += border;
        if (flag & uiSouth)
            ret.cy += border;

        return ret;
    }

    SizeF SizerItemSpacer::calcMinSize()
    {
        return getMinSizeWithBorder();
    }

    bool SizerItemSpacer::isShown()
    {
        return spacer->isShown();
    }

    // ---------------------------------------------------------------
    // Sizer
    // ---------------------------------------------------------------
    HARE_IMPLEMENT_ABSTRACT_CLASS(Sizer, Object, 0)
    {
    }
    Sizer::Sizer()
    {
    }

    Sizer::~Sizer()
    {
    }

    SizerItem* Sizer::add(f32 width, f32 height, int proportion /* = 0 */, int flag /* = 0 */, int border /* = 0 */)
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

    SizerItem* Sizer::insert(size_t index, f32 width, f32 height, int proportion /* = 0 */, int flag /* = 0 */, int border /* = 0 */)
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

