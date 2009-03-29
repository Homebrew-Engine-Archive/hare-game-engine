#include "PCH.h"
#include "BoxSizer.h"

namespace hare_ui
{
    HARE_IMPLEMENT_DYNAMIC_CLASS(BoxSizer, Sizer, 0)
    {
        HARE_META(orient, int)
    }

    BoxSizer::BoxSizer(int orientation)
        : orient(orientation)
    {
    }

    void BoxSizer::recalcSizes()
    {
        if (items.size() == 0)
            return;

        int delta = 0;
        if (stretchable)
        {
            if (orient == uiHORIZONTAL)
                delta = size.cx - fixedWidth;
            else
                delta = size.cy - fixedHeight;
        }

        PointF pt(position);

        int oldStretchable = stretchable;
        SizerItem::List::iterator it = items.begin();
        for (; it != items.end(); ++it)
        {
            SizerItem* item = *it;

            if (item->isShown())
            {
                SizeF sz(item->getMinSizeWithBorder());

                if (orient == uiVERTICAL)
                {
                    f32 height = sz.cy;
                    if (item->getProportion())
                    {
                        height = (delta * item->getProportion()) / oldStretchable;
                        delta -= height;
                        oldStretchable -= item->getProportion();
                    }

                    PointF child_pos(pt);
                    SizeF child_size(sz.cx, height);

                    if (item->getFlag() & (uiEXPAND | uiSHAPED))
                        child_size.cx = size.cx;
                    else if (item->getFlag() & uiALIGN_RIGHT)
                        child_pos.x += size.cx - sz.cx;
                    else if (item->getFlag() & uiALIGN_CENTER_HORIZONTAL)
                        child_pos.x += (size.cx - sz.cx) / 2;

                    item->setDimension(child_pos, child_size);

                    pt.y += height;
                }
                else
                {
                    f32 width = sz.cx;
                    if (item->getProportion())
                    {
                        width = (delta * item->getProportion()) / oldStretchable;
                        delta -= width;
                        oldStretchable -= item->getProportion();
                    }

                    PointF child_pos(pt);
                    SizeF child_size(width, sz.cy);

                    if (item->getFlag() & (uiEXPAND | uiSHAPED))
                        child_size.cy = size.cy;
                    else if (item->getFlag() & uiALIGN_BOTTOM)
                        child_pos.y += size.cy - sz.cy;
                    else if (item->getFlag() & uiALIGN_CENTER_VERTICAL)
                        child_pos.y += (size.cy - sz.cy) / 2;

                    if (containerWindow)
                    {
                        child_pos.x = containerWindow->adjustForLayoutDirection(child_pos.x, width, size.cx);
                    }

                    item->setDimension(child_pos, child_size);

                    pt.x += width;
                }
            }
        }
    }

    SizeF BoxSizer::calcMin()
    {
        if (items.size() == 0)
            return SizeF();

        stretchable = 0;
        minWidth = 0;
        minHeight = 0;
        fixedWidth = 0;
        fixedHeight = 0;

        for (SizerItem::List::iterator it = items.begin(); it != items.end(); ++it)
        {
            SizerItem* item = *it;

            if (item->isShown())
            {
                item->calcMin();
                stretchable += item->getProportion();
            }
        }

        // Total minimum size (width or height) of sizer
        int maxMinSize = 0;
        for (SizerItem::List::iterator it = items.begin(); it != items.end(); ++it)
        {
            SizerItem* item = *it;

            if (item->isShown() && item->getProportion() != 0)
            {
                int stretch = item->getProportion();
                SizeF sz(item->getMinSizeWithBorder());
                int minSize;

                if (orient == uiHORIZONTAL)
                    minSize = (sz.cx * stretchable + stretch - 1) / stretch;
                else
                    minSize = (sz.cy * stretchable + stretch - 1) / stretch;

                if (minSize > maxMinSize)
                    maxMinSize = minSize;
            }
        }

        // Calculate overall minimum size
        for (SizerItem::List::iterator it = items.begin(); it != items.end(); ++it)
        {
            SizerItem* item = *it;

            if (item->isShown())
            {
                SizeF sz(item->getMinSizeWithBorder());
                if (item->getProportion() != 0)
                {
                    if (orient == uiHORIZONTAL)
                        sz.cx = (maxMinSize * item->getProportion()) / stretchable;
                    else
                        sz.cy = (maxMinSize * item->getProportion()) / stretchable;
                }
                else
                {
                    if (orient == uiVERTICAL)
                    {
                        fixedHeight += sz.cy;
                        fixedWidth = max(fixedWidth, sz.cx);
                    }
                    else
                    {
                        fixedWidth += sz.cx;
                        fixedHeight = max(fixedHeight, sz.cy);
                    }
                }

                if (orient == uiHORIZONTAL)
                {
                    minWidth += sz.cx;
                    minHeight = max(minHeight, sz.cy);
                }
                else
                {
                    minHeight += sz.cy;
                    minWidth = max(minWidth, sz.cx);
                }
            }
        }

        return SizeF(minWidth, minHeight);
    }
}

