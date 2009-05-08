#ifndef _SIZER_H_
#define _SIZER_H_

#include "UIPrerequisites.h"
#include "SizerItem.h"

namespace hare
{
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

        SizerItem* add(float width,
                    float height,
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
                    float width,
                    float height,
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
        Window* containerWindow;

        SizerItem::List items;
        SizeF size;
        SizeF minSize;
        PointF position;
    };
}

#endif