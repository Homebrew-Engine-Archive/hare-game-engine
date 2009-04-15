//***************************************************************
//  File:    TManager.h
//  Date:    10/23/2008
//  Author:  littlesome (littlesome@live.cn)
//-------------------------------------------------------------
//
//-------------------------------------------------------------
//  This file is part of Hare2D Game Engine.
//  Copyright (C) All Rights Reserved
//***************************************************************
//
//***************************************************************
#ifndef TMANAGER_H
#define TMANAGER_H

#include "EditorPrerequisites.h"
#include <assert.h>

namespace hare
{
    template <typename T> class TManager
    {
    private:
        explicit TManager<T>(const TManager<T>&);
        TManager<T>& operator=(const TManager<T>&);

    private:
        static T* instance;
        static bool autoCreate;

    protected:

        TManager()
        {
            assert(TManager<T>::instance == 0);
        }
        virtual ~TManager()
        {
            TManager<T>::instance = 0;
        }

    public:

        static inline bool isValid()
        {
            return instance != 0;
        }

        static inline T* getInstancePtr()
        {
            if (instance == 0 && autoCreate == true)
                instance = new T();

            return instance;
        }

        static void free()
        {
            autoCreate = false;
            delete instance;
            instance = 0;
        }
    };
}



#endif
