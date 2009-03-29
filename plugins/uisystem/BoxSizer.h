#include "Sizer.h"

namespace hare_ui
{
    class BoxSizer : public Sizer
    {
        HARE_DECLARE_DYNAMIC_CLASS(BoxSizer)
    public:
        BoxSizer(int orientation);

        virtual void recalcSizes();
        virtual SizeF calcMin();

        int getOrientation() const
        { 
            return orient; 
        }

        void setOrientation(int orientation)
        { 
            orient = orientation; 
        }

    protected:
        int orient;
        int stretchable;
        f32 minWidth;
        f32 minHeight;
        f32 fixedWidth;
        f32 fixedHeight;
    };
}

