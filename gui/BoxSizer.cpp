#include "PCH.h"
#include "BoxSizer.h"
#include "Window.h"

namespace hare
{
    HARE_ENUM_BEGIN(uiOrientation)
        HARE_ENUM_VALUE(uiHorizontal)
        HARE_ENUM_VALUE(uiVertical)
    HARE_ENUM_END()

    HARE_IMPLEMENT_DYNAMIC_CLASS(BoxSizer, Sizer, 0)
    {
        HARE_ENUM(orient, int32, uiOrientation)
    }

    BoxSizer::BoxSizer(int orientation)
        : orient(orientation)
    {
    }

    void BoxSizer::recalcSizes()
    {
        if (items.size() == 0)
            return;

        float delta = 0;
        if (stretchable)
        {
            if (orient == uiHorizontal)
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

                if (orient == uiVertical)
                {
                    float height = sz.cy;
                    if (item->getProportion())
                    {
                        height = (delta * item->getProportion()) / oldStretchable;
                        delta -= height;
                        oldStretchable -= item->getProportion();
                    }

                    PointF child_pos(pt);
                    SizeF child_size(sz.cx, height);

                    if (item->getFlag() & (uiExpand | uiShaped))
                        child_size.cx = size.cx;
                    else if (item->getFlag() & uiAlign_Right)
                        child_pos.x += size.cx - sz.cx;
                    else if (item->getFlag() & uiAlign_Center_Horizontal)
                        child_pos.x += (size.cx - sz.cx) / 2;

                    item->setDimension(child_pos, child_size);

                    pt.y += height;
                }
                else
                {
                    float width = sz.cx;
                    if (item->getProportion())
                    {
                        width = (delta * item->getProportion()) / oldStretchable;
                        delta -= width;
                        oldStretchable -= item->getProportion();
                    }

                    PointF child_pos(pt);
                    SizeF child_size(width, sz.cy);

                    if (item->getFlag() & (uiExpand | uiShaped))
                        child_size.cy = size.cy;
                    else if (item->getFlag() & uiAlign_Bottom)
                        child_pos.y += size.cy - sz.cy;
                    else if (item->getFlag() & uiAlign_Center_Vertical)
                        child_pos.y += (size.cy - sz.cy) / 2;

                    if (containingWindow)
                    {
                        child_pos.x = containingWindow->adjustForLayoutDirection(child_pos.x, width, size.cx);
                    }

                    item->setDimension(child_pos, child_size);

                    pt.x += width;
                }
            }
        }
    }

    SizeF BoxSizer::calcMinSize()
    {
        if (items.size() == 0)
            return SizeF(0, 0);

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
                item->calcMinSize();
                stretchable += item->getProportion();
            }
        }

        // Total minimum size (width or height) of sizer
        float maxMinSize = 0;
        for (SizerItem::List::iterator it = items.begin(); it != items.end(); ++it)
        {
            SizerItem* item = *it;

            if (item->isShown() && item->getProportion() != 0)
            {
                int stretch = item->getProportion();
                SizeF sz(item->getMinSizeWithBorder());
                float minSize;

                if (orient == uiHorizontal)
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
                    if (orient == uiHorizontal)
                        sz.cx = (maxMinSize * item->getProportion()) / stretchable;
                    else
                        sz.cy = (maxMinSize * item->getProportion()) / stretchable;
                }
                else
                {
                    if (orient == uiVertical)
                    {
                        fixedHeight += sz.cy;
                        fixedWidth = std::max(fixedWidth, sz.cx);
                    }
                    else
                    {
                        fixedWidth += sz.cx;
                        fixedHeight = std::max(fixedHeight, sz.cy);
                    }
                }

                if (orient == uiHorizontal)
                {
                    minWidth += sz.cx;
                    minHeight = std::max(minHeight, sz.cy);
                }
                else
                {
                    minHeight += sz.cy;
                    minWidth = std::max(minWidth, sz.cx);
                }
            }
        }

        return SizeF(minWidth, minHeight);
    }
}

