//***************************************************************
//  File:    DynamicLibrary.h
//  Data:    03/03/2009
//  Author:  littlesome (littlesome@live.cn)
//-------------------------------------------------------------
//
//-------------------------------------------------------------
//  This file is part of Hare2D Game Engine.
//  Copyright (C) All Rights Reserved
//***************************************************************
//
//***************************************************************
#ifndef DYNAMICLIBRARY_H
#define DYNAMICLIBRARY_H

#include "CorePrerequisites.h"
#include "ReferenceCounted.h"

namespace hare
{
    class CORE_API DynamicLibrary : public ReferenceCounted
    {
    public:
        DynamicLibrary() : handle(0) {}

        DynamicLibrary(const String& path)
        {
            load(path);
        }
       
        ~DynamicLibrary()
        {
            unload();
        }

        void load(const String& path);

        void* getSymbol(const String& name);

        bool isLoaded() { return handle != 0; }

        void* getHandle() const { return handle; }

        void unload() 
        {
            if (handle)
            {
                unload(handle);
                handle = 0;
            }
        }

        static void unload(void* libHandle);

    private:
        void* handle;

        virtual void _doRelease()
        {
            delete this;
        }

        HARE_DECLARE_NO_COPY_CLASS(DynamicLibrary)
    };
}

#endif
