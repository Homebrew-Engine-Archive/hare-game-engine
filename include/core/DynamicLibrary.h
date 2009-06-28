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
    /** Class for dynamic-loading libraries
    */
    class CORE_API DynamicLibrary : public ReferenceCounted
    {
    public:
        DynamicLibrary() : handle(0) {}

        /// Constructor
        DynamicLibrary(const String& path)
        {
            load(path);
        }
       
        virtual ~DynamicLibrary()
        {
            unload();
        }

        /// Load a dynamic library form path
        void load(const String& path);

        /// Get a symbol from loaded library
        void* getSymbol(const String& name);

        /// Is dynamic library loaded ?
        bool isLoaded() { return handle != 0; }

        /// Get dynamic library's handle 
        void* getHandle() const { return handle; }

        /// Unload the dynamic library 
        void unload() 
        {
            if (handle)
            {
                unload(handle);
                handle = 0;
            }
        }

        /// Unload a dynamic library 
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
