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
}

namespace hare
{
    // ---------------------------------------------------------------
    // SizerItem
    // ---------------------------------------------------------------
    HARE_IMPLEMENT_ABSTRACT_CLASS(SizerItem, Object, 0)
    {
        HARE_META(proportion, uint32)
        HARE_META(border, uint32)
        HARE_ENUM_F(flag, uint32, uiEnumFlags, propEnumIsFlag)
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

    void SizerItem::setDimension(const PointF& ps, const SizeF& sz)
    {
        PointF tpos = ps;
        SizeF tsize = sz;

        if (flag & uiShaped)
        {
            float ratio = minSize.cx / minSize.cy;
            float tw = tsize.cy * ratio;

            if (tw > tsize.cx)
            {
                float th = tsize.cx / ratio;
                if (flag & uiAlign_Center_Vertical)
                    tpos.y += (tsize.cy - th) / 2;
                else if (flag & uiAlign_Bottom)
                    tpos.y += (tsize.cy - th);
                tsize.cy = th;
            }
            else if (tw < tsize.cx)
            {
                if (flag & uiAlign_Center_Horizontal)
                    tpos.x += (tsize.cx - tw) / 2;
                else if (flag & uiAlign_Right)
                    tpos.x += (tsize.cx - tw);
                tsize.cx = tw;
            }
        }

        pos = tpos;

        if (flag & uiWest)
        {
            tpos.x += border;
            tsize.cx -= border;
        }
        if (flag & uiEast)
        {
            tsize.cx -= border;
        }
        if (flag & uiNorth)
        {
            tpos.y += border;
            tsize.cy -= border;
        }
        if (flag & uiSouth)
        {
            tsize.cy -= border;
        }

        if (tsize.cx < 0)
            tsize.cx = 0;
        if (tsize.cy < 0)
            tsize.cy = 0;

        rect.set(tpos.x, tpos.y, tpos.x + tsize.cx, tpos.y + tsize.cy);
    }

    // ---------------------------------------------------------------
    // SizerItemWindow
    // ---------------------------------------------------------------
    HARE_IMPLEMENT_DYNAMIC_CLASS(SizerItemWindow, SizerItem, 0)
    {
    }
    
    SizerItemWindow::SizerItemWindow()
        : SizerItem(0, 0, 5), window(0)
    {
    }

    SizerItemWindow::SizerItemWindow(Window* win, int prop, int flg, int bdr)
        : SizerItem(prop, flg, bdr), window(win)
    {
    }

    SizerItemWindow::~SizerItemWindow()
    {
        window->setParentSizer(0);
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

    bool SizerItemWindow::isShown()
    {
        return window->isShown();
    }

    void SizerItemWindow::setDimension(const PointF& ps, const SizeF& sz)
    {
        SizerItem::setDimension(ps, sz);
        //window->setSize();
    }
    // ---------------------------------------------------------------
    // SizerItemSizer
    // ---------------------------------------------------------------
    HARE_IMPLEMENT_DYNAMIC_CLASS(SizerItemSizer, SizerItem, 0)
    {
    }

    SizerItemSizer::SizerItemSizer()
        : SizerItem(0, 0, 5), sizer(0)
    {
    }

    SizerItemSizer::SizerItemSizer(Sizer* szr, int prop, int flg, int bdr)
        : SizerItem(prop, flg, bdr), sizer(szr)
    {
    }

    SizerItemSizer::~SizerItemSizer()
    {
        delete sizer;
        sizer = 0;
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

    void SizerItemSizer::setDimension(const PointF& ps, const SizeF& sz)
    {
        SizerItem::setDimension(ps, sz);
        sizer->setDimension(rect);
    }

    // ---------------------------------------------------------------
    // SizerItemSpacer
    // ---------------------------------------------------------------
    HARE_IMPLEMENT_DYNAMIC_CLASS(SizerItemSpacer, SizerItem, 0)
    {
    }

    SizerItemSpacer::SizerItemSpacer()
        : SizerItem(0, 0, 5), spacer(0)
    {
    }

    SizerItemSpacer::SizerItemSpacer(float w, float h, int prop, int flg, int bdr)
        : SizerItem(prop, flg, bdr)
    {
        spacer = new SizerSpacer(SizeF(w, h));
    }

    SizerItemSpacer::~SizerItemSpacer()
    {
        delete spacer;
        spacer = 0;
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

    void SizerItemSpacer::setDimension(const PointF& ps, const SizeF& sz)
    {
        SizerItem::setDimension(ps, sz);
        spacer->setSize(SizeF(rect.width(), rect.height()));
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

