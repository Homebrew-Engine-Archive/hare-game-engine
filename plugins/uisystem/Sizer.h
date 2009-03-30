#ifndef _SIZER_H_
#define _SIZER_H_

#include "UIPrerequisites.h"

namespace hare_ui
{
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

    class SizerItem : public Object
    {
        HARE_DECLARE_ABSTRACT_CLASS(SizerItem)
    public:
        SizerItem(int prop, int flg, int bdr);
        virtual ~SizerItem();

        SizeF getMinSize() const;
        SizeF getMinSizeWithBorder() const;

        void setProportion(int value)
        { 
            proportion = value; 
        }
        int getProportion() const
        { 
            return proportion; 
        }
        void setFlag(int value)
        { 
            flag = value; 
        }
        int getFlag() const
        { 
            return flag; 
        }
        void setBorder(int value)
        { 
            border = value; 
        }
        int getBorder() const
        { 
            return border; 
        }

        virtual SizeF getSize() const = 0;
        virtual SizeF calcMinSize() = 0;
        virtual bool isShown() = 0;

        void setDimension(const PointF& pos, const SizeF& size);

    protected:
        int proportion;
        int border;
        int flag;

        PointF pos;
        SizeF minSize;
        RectF rect;
    };

    class SizerItemWindow : public SizerItem
    {
        HARE_DECLARE_DYNAMIC_CLASS(SizerItemWindow)
    public:
        SizerItemWindow(Window* win, int prop, int flg, int bdr);
        virtual ~SizerItemWindow();

        virtual SizeF getSize() const;
        virtual SizeF calcMinSize();
        virtual bool isShown();
    protected:
        Window* window;
    };

    class SizerItemSizer : public SizerItem
    {
        HARE_DECLARE_DYNAMIC_CLASS(SizerItemSizer)
    public:
        SizerItemSizer(Sizer* szr, int prop, int flg, int bdr);
        virtual ~SizerItemSizer();

        virtual SizeF getSize() const;
        virtual SizeF calcMinSize();
        virtual bool isShown();
    protected:
        Sizer* sizer;
    };

    class SizerItemSpacer : public SizerItem
    {
        HARE_DECLARE_DYNAMIC_CLASS(SizerItemSpacer)
    public:
        SizerItemSpacer(f32 w, f32 h, int prop, int flg, int bdr);
        virtual ~SizerItemSpacer();

        virtual SizeF getSize() const;
        virtual SizeF calcMinSize();
        virtual bool isShown();
    protected:
        SizerSpacer* spacer;
    };

    class SizerSpacer
    {
    public:
        SizerSpacer(const SizeF& sz) : size(sz), shown(true) 
        {}

        void setSize(const SizeF& sz) 
        { 
            size = sz; 
        }
        
        const SizeF& getSize() const 
        { 
            return size; 
        }

        void show(bool show) 
        { 
            shown = show; 
        }
        
        bool isShown() const 
        { 
            return shown; 
        }

    private:
        SizeF size;
        bool shown;
    };

    class Sizer : public Object
    {
        HARE_DECLARE_ABSTRACT_CLASS(Sizer)
    public:
        Sizer();
        virtual ~Sizer();

    public:
        SizerItem* add(Window *window, 
                    int proportion = 0,
                    int flag = 0,
                    int border = 0);

        SizerItem* add(Sizer *sizer,
                    int proportion = 0,
                    int flag = 0,
                    int border = 0);

        SizerItem* add(f32 width,
                    f32 height,
                    int proportion = 0,
                    int flag = 0,
                    int border = 0);

        SizerItem* insert(size_t index,
                    Window *window, 
                    int proportion = 0,
                    int flag = 0,
                    int border = 0);

        SizerItem* insert(size_t index,
                    Sizer *sizer,
                    int proportion = 0,
                    int flag = 0,
                    int border = 0);

        SizerItem* insert(size_t index,
                    f32 width,
                    f32 height,
                    int proportion = 0,
                    int flag = 0,
                    int border = 0);

        virtual SizerItem* insert(size_t index, SizerItem* item);

        void setContainerWindow(Window *window);
        
        Window* getContainerWindow() const 
        { 
            return containerWindow;
        }

        SizerItem::List& getChildren()
        {
            return items;
        }

        virtual void layout();

        SizeF getSize() const
        { 
            return size; 
        }

        SizeF getMinSize() const
        {
            return minSize;
        }

    public:
        virtual void recalcSizes() = 0;

        virtual SizeF calcMinSize() = 0;
        
    protected:
        Window* containerWindow;

        SizerItem::List items;
        SizeF size;
        SizeF minSize;
        PointF position;
    };
}

#endif