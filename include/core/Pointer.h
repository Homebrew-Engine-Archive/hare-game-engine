//***************************************************************
//  File:    Pointer.h
//  Date:    10/03/2008
//  Author:  littlesome (littlesome@live.cn)
//-------------------------------------------------------------
//
//-------------------------------------------------------------
//  This file is part of Hare2D Game Engine.
//  Copyright (C) All Rights Reserved
//***************************************************************
//
//***************************************************************
#ifndef _POINTER_H_
#define _POINTER_H_

namespace hare
{
    /** Shared pointer class
    */
    template <typename T> class Pointer
    {
    public:
        typedef T* value_type;

    private:
        T* ptr;

    public:
        Pointer() : ptr(0)
        {
        }

        Pointer(T* p) : ptr(p)
        {
            if (ptr)
                ptr->addRef();
        }

        Pointer(const Pointer &p) : ptr(0)
        {
            *this = p;
        }

        ~Pointer()
        {
            if (ptr)
                ptr->decRef();
        }

        /// Get raw pointer
        T* pointer()
        {
            return ptr;
        }

        /// Get raw pointer's ref
        T*& pointerRef()
        {
            return ptr;
        }

        /// Get raw pointer, const version
        const T* pointer() const
        {
            return ptr;
        }

        /// Get raw pointer's ref, const version
        const T*& pointerRef() const
        {
            return ptr;
        }

        operator T* ()
        {
            return ptr;
        }

        operator const T* () const
        {
            return ptr;
        }

        T& operator * ()
        {
            return *ptr;
        }

        T* operator -> ()
        {
            return ptr;
        }

        const T* operator -> () const
        {
            return ptr;
        }

        const Pointer& operator = (const Pointer<T> &p)
        {
            return operator=((const T *) p);
        }

        const Pointer& operator = (const T* p)
        {
            if (p)
                const_cast<T*>(p)->addRef();
            if (ptr)
                ptr->decRef();
            ptr = const_cast<T*>(p);
            return *this;
        }
    };
}

#endif
