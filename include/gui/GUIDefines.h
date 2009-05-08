#ifndef _GUI_DEFINES_H_
#define _GUI_DEFINES_H_

#define HARE_GUI_UV(X)  HARE_META_F(X, RectUV, propHide)

namespace hare
{
    enum uiWindowID
    {
        uiID_Any = -1,
    };

    enum MouseButton
    {
        LeftButton,
        RightButton,
        MiddleButton,
        MB_MAX,
    };

    enum uiLayoutDirection
    {
        uiLayout_Default,
        uiLayout_LeftToRight,
        uiLayout_RightToLeft
    };

    enum uiEnumFlags
    {
        // Alignment
        uiAlign_Default             = 0x0000,
        uiAlign_Center_Horizontal   = 0x0100,
        uiAlign_Left                = uiAlign_Default,
        uiAlign_Top                 = uiAlign_Default,
        uiAlign_Right               = 0x0200,
        uiAlign_Bottom              = 0x0400,
        uiAlign_Center_Vertical     = 0x0800,
        uiAlign_Center              = (uiAlign_Center_Horizontal | uiAlign_Center_Vertical),
        uiAlign_Mask                = 0x0F00,

        // Direction
        uiWest                      = 0x0010,
        uiEast                      = 0x0020,
        uiNorth                     = 0x0040,
        uiSouth                     = 0x0080,
        uiAll_Direction             = uiWest | uiEast | uiNorth | uiSouth,

        // Stretch
        uiExpand                    = 0x2000,
        uiShaped                    = 0x4000,
        uiFixed_Minsize             = 0x8000,
    };

    enum uiOrientation
    {
        uiHorizontal                = 0x0004,
        uiVertical                  = 0x0008,
        uiBoth                      = uiVertical | uiHorizontal,
    };
}

#endif