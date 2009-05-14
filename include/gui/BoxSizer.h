#ifndef _BOXSIZER_H_
#define _BOXSIZER_H_

#include "UIPrerequisites.h"
#include "Sizer.h"

namespace hare
{
    class UI_API BoxSizer : public Sizer
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
        int32 orient;
        int32 stretchable;
        float minWidth;
        float minHeight;
        float fixedWidth;
        float fixedHeight;
    };
}

#endif