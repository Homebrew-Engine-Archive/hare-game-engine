//***************************************************************
//  File:    ReferenceCounted.h
//  Date:    09/29/2008
//  Author:  littlesome (littlesome@live.cn)
//-------------------------------------------------------------
//
//-------------------------------------------------------------
//  This file is part of Hare2D Game Engine.
//  Copyright (C) All Rights Reserved
//***************************************************************
//
//***************************************************************
#ifndef _REFERENCECOUNTED_H_
#define _REFERENCECOUNTED_H_

#include <assert.h>

namespace hare_core
{
    /** Reference-counted base class
    */
    class CORE_API ReferenceCounted
    {
    public:
        ReferenceCounted() : numRef(0)
        {
        }
        virtual ~ReferenceCounted()
        {
        }
        void addRef()
        {
            ++numRef;
        }
        void decRef()
        {
            assert(numRef > 0);
            --numRef;
            if (numRef == 0)
                doRelease();
        }
        int getRef() const
        {
            return numRef;
        }

    protected:
        ReferenceCounted(const ReferenceCounted &)
        {
        }

        ReferenceCounted& operator=(const ReferenceCounted &)
        {   
            return *this;
        }

    private:
        // do "delete this;"
        virtual void doRelease() = 0;

        int numRef;
    };
}

#endif
