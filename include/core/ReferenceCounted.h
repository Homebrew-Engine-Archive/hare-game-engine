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

namespace hare
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

        /** Increase reference count.
        */
        void addRef()
        {
            ++numRef;
        }

        /** Decrease reference count, if reference count is zero, 
            _doRelease() will be called.
        */
        void decRef()
        {
            assert(numRef > 0);
            --numRef;
            if (numRef == 0)
                _doRelease();
        }

        /** Get reference count.
        */
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

        /** do 'delete this;'
        */
        virtual void _doRelease() = 0;

    private:
        int numRef;
    };
}

#endif
