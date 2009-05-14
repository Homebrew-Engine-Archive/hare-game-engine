#ifndef _SIZER_ITEM_H_
#define _SIZER_ITEM_H_

#include "UIPrerequisites.h"

namespace hare
{
    class UI_API SizerItem : public Object
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
        virtual void setDimension(const PointF& ps, const SizeF& sz);

    protected:
        uint32 proportion;
        uint32 border;
        uint32 flag;

        PointF pos;
        SizeF minSize;
        RectF rect;
    };

    class UI_API SizerItemWindow : public SizerItem
    {
        HARE_DECLARE_DYNAMIC_CLASS(SizerItemWindow)
    public:
        SizerItemWindow(Window* win, int prop, int flg, int bdr);
        virtual ~SizerItemWindow();

        virtual SizeF getSize() const;
        virtual SizeF calcMinSize();
        virtual bool isShown();
        virtual void setDimension(const PointF& ps, const SizeF& sz);
        
        Window* getWindow() { return window; }

    protected:
        WindowPtr window;
    private:
        SizerItemWindow();
    };

    class UI_API SizerItemSizer : public SizerItem
    {
        HARE_DECLARE_DYNAMIC_CLASS(SizerItemSizer)
    public:
        SizerItemSizer(Sizer* szr, int prop, int flg, int bdr);
        virtual ~SizerItemSizer();

        virtual SizeF getSize() const;
        virtual SizeF calcMinSize();
        virtual bool isShown();
        virtual void setDimension(const PointF& ps, const SizeF& sz);

        Sizer* getSizer() { return sizer; }

    protected:
        SizerPtr sizer;
    private:
        SizerItemSizer();
    };

    class UI_API SizerItemSpacer : public SizerItem
    {
        HARE_DECLARE_DYNAMIC_CLASS(SizerItemSpacer)
    public:
        SizerItemSpacer(float w, float h, int prop, int flg, int bdr);
        virtual ~SizerItemSpacer();

        virtual SizeF getSize() const;
        virtual SizeF calcMinSize();
        virtual bool isShown();
        virtual void setDimension(const PointF& ps, const SizeF& sz);
    protected:
        SizerSpacerPtr spacer;
    private:
        SizerItemSpacer();
    };

    class UI_API SizerSpacer : public Object
    {
        HARE_DECLARE_DYNAMIC_CLASS(SizerSpacer)
    public:
        SizerSpacer();
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
}

#endif