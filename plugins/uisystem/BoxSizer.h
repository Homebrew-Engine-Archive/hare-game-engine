#ifndef _BOXSIZER_H_
#define _BOXSIZER_H_

#include "UIPrerequisites.h"
#include "Sizer.h"

namespace hare_ui
{
    class BoxSizer : public Sizer
    {
        HARE_DECLARE_DYNAMIC_CLASS(BoxSizer)
    public:
        BoxSizer(int orientation = uiHorizontal);

        virtual void recalcSizes();
        virtual SizeF calcMinSize();

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

#endif