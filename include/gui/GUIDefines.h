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
        uiAlign_Left                = 0x0100,
        uiAlign_Top                 = 0x0200,
        uiAlign_Right               = 0x0400,
        uiAlign_Bottom              = 0x0800,
        uiAlign_Default             = (uiAlign_Left | uiAlign_Top),

        uiAlign_Center_Horizontal   = 0x1000,
        uiAlign_Center_Vertical     = 0x2000,
        uiAlign_Center              = (uiAlign_Center_Horizontal | uiAlign_Center_Vertical),
        
        uiAlign_HorizontalMask      = (uiAlign_Left | uiAlign_Center_Horizontal | uiAlign_Right),
        uiAlign_VerticalMask        = (uiAlign_Top | uiAlign_Center_Vertical | uiAlign_Bottom),
        uiAlign_Mask                = 0xFF00,

        // Direction
        uiWest                      = 0x0010,
        uiEast                      = 0x0020,
        uiNorth                     = 0x0040,
        uiSouth                     = 0x0080,
        uiAll_Direction             = uiWest | uiEast | uiNorth | uiSouth,

        // Stretch
        uiExpand                    = 0x00020000,
        uiShaped                    = 0x00040000,
        uiFixed_Minsize             = 0x00080000,
    };

    enum uiOrientation
    {
        uiHorizontal                = 0x0004,
        uiVertical                  = 0x0008,
        uiBoth                      = uiVertical | uiHorizontal,
    };
}

#endif