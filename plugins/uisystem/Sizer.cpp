#include "Sizer.h"

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

    if (flag & uiWEST)
        ret.cx += border;
    if (flag & uiEAST)
        ret.cx += border;
    if (flag & uiNORTH)
        ret.cy += border;
    if (flag & uiSOUTH)
        ret.cy += border;

    return ret;
}

// ---------------------------------------------------------------
// SizerItemWindow
// ---------------------------------------------------------------
SizeF SizerItemWindow::getSize() const
{   
    SizeF ret = window->getSize();
    
    if (flag & uiWEST)
        ret.cx += border;
    if (flag & uiEAST)
        ret.cx += border;
    if (flag & uiNORTH)
        ret.cy += border;
    if (flag & uiSOUTH)
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
SizeF SizerItemWindow::getSize() const
{   
    SizeF ret = sizer->getSize();

    if (flag & uiWEST)
        ret.cx += border;
    if (flag & uiEAST)
        ret.cx += border;
    if (flag & uiNORTH)
        ret.cy += border;
    if (flag & uiSOUTH)
        ret.cy += border;

    return ret;
}

SizeF SizerItemWindow::calcMinSize()
{
    minSize = sizer->getMinSize();

    return getMinSizeWithBorder();
}

// ---------------------------------------------------------------
// SizerItemSpacer
// ---------------------------------------------------------------
SizeF SizerItemSpacer::getSize() const
{   
    SizeF ret = spacer->getSize();

    if (flag & uiWEST)
        ret.cx += border;
    if (flag & uiEAST)
        ret.cx += border;
    if (flag & uiNORTH)
        ret.cy += border;
    if (flag & uiSOUTH)
        ret.cy += border;

    return ret;
}

SizeF SizerItemSpacer::calcMinSize()
{
    return getMinSizeWithBorder();
}

// ---------------------------------------------------------------
// Sizer
// ---------------------------------------------------------------
Sizer::Sizer()
{
}

Sizer::~Sizer()
{
}

SizerItem* Sizer::add(int width, int height, int proportion /* = 0 */, int flag /* = 0 */, int border /* = 0 */)
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

SizerItem* Sizer::insert(size_t index, int width, int height, int proportion /* = 0 */, int flag /* = 0 */, int border /* = 0 */)
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
        items.push_back(item)
    }
    else
    {
        items.insert(items.begin() + index, item);
    }

    return item;
}

void Sizer::layout()
{
    calcMin();

    recalcSizes();
}