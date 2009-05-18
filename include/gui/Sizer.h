#ifndef _SIZER_H_
#define _SIZER_H_

#include "UIPrerequisites.h"
#include "SizerItem.h"

namespace hare
{
    class UI_API Sizer : public Object
    {
        HARE_DECLARE_ABSTRACT_CLASS(Sizer)
    public:
        Sizer();
        virtual ~Sizer();

    public:
        SizerItem* add(Window *window, 
                    int proportion = 0,
                    int flag = uiAlign_Default,
                    int border = 5);

        SizerItem* add(Sizer *sizer,
                    int proportion = 0,
                    int flag = uiAlign_Default,
                    int border = 5);

        SizerItem* add(float width,
                    float height,
                    int proportion = 0,
                    int flag = uiAlign_Default,
                    int border = 5);

        SizerItem* insert(size_t index,
                    Window *window, 
                    int proportion = 0,
                    int flag = uiAlign_Default,
                    int border = 5);

        SizerItem* insert(size_t index,
                    Sizer *sizer,
                    int proportion = 0,
                    int flag = uiAlign_Default,
                    int border = 5);

        SizerItem* insert(size_t index,
                    float width,
                    float height,
                    int proportion = 0,
                    int flag = uiAlign_Default,
                    int border = 5);

        virtual SizerItem* insert(size_t index, SizerItem* item);

        void setContainingWindow(Window *window);
        
        Window* getContainingWindow() const 
        { 
            return containingWindow;
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

        PointF getPosition() const
        { 
            return position; 
        }

        SizeF getMinSize();
       
        void setDimension(const PointF& ps, const SizeF& sz)
        {
            position = ps;
            size = sz;
            layout();
        }

        void setDimension(const RectF& rect)
        {
            position.set(rect.minX, rect.minY);
            size.set(rect.width(), rect.height());
            layout();
        }

    public:
        virtual void recalcSizes() = 0;

        virtual SizeF calcMinSize() = 0;
        
    protected:
        Window* containingWindow;

        SizerItem::List items;
        SizeF size;
        SizeF minSize;
        PointF position;
    };
}

#endif