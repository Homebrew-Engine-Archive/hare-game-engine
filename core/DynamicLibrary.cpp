//***************************************************************
//  File:    DynamicLibrary.cpp
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
#include "PCH.h"
#include "DynamicLibrary.h"

#if HARE_PLATFORM == HARE_PLATFORM_LINUX
    #include <dlfcn.h>
    namespace hare_core
    {
        void DynamicLibrary::load(const String& path)
        {
            handle = dlopen(path.c_str(), RTLD_NOW);
        }

        void* DynamicLibrary::getSymbol(const String& name)
        {
            return dlsym(handle, name.c_str());
        }

        void DynamicLibrary::unload(void* libHandle)
        {
            dlclose(libHandle);
        }
    }
#elif HARE_PLATFORM == HARE_PLATFORM_WIN32
    #include <windows.h>
    namespace hare_core
    {
        void DynamicLibrary::load(const String& path)
        {
            handle = LoadLibraryA(path.c_str());
        }

        void* DynamicLibrary::getSymbol(const String& name)
        {
            return (void*)GetProcAddress((HINSTANCE)handle, name.c_str());
        }

        void DynamicLibrary::unload(void* libHandle)
        {
            FreeLibrary((HINSTANCE)libHandle);
        }
    }
#elif HARE_PLATFORM == HARE_PLATFORM_APPLE
    #include <mach-o/dyld.h>
    namespace hare_core
    {
        void DynamicLibrary::load(const String& path)
        {
            NSObjectFileImage img;
            NSObjectFileImageReturnCode ret;

            ret = NSCreateObjectFileImageFromFile(path.c_str(), &img);
            if (ret == NSObjectFileImageSuccess)
            {
                handle = NSLinkModule(img, path.c_str(), NSLINKMODULE_OPTION_PRIVATE |
                    NSLINKMODULE_OPTION_RETURN_ON_ERROR);
                NSDestroyObjectFileImage(img);
            }
            else
                handle = 0;
        }

        void* DynamicLibrary::getSymbol(const String& name)
        {
            NSSymbol nss = NSLookupSymbolInModule((NSModule)handle, name.c_str());
            if (nss == NULL) {
                return NULL;
            }
            return NSAddressOfSymbol(nss);
        }

        void DynamicLibrary::unload(void* libHandle)
        {
            NSUnLinkModule((NSModule)libHandle, NSUNLINKMODULE_OPTION_RESET_LAZY_REFERENCES);
        }
    }
#endif
