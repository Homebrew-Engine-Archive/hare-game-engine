//***************************************************************
//  File:    GameApp.h
//  Data:    05/21/2009
//  Author:  littlesome (littlesome@live.cn)
//-------------------------------------------------------------
//  
//-------------------------------------------------------------
//  This file is part of Hare2D Game Engine.
//  Copyright (C) All Rights Reserved
//***************************************************************
// 
//***************************************************************
#ifndef GAMEAPP_H
#define GAMEAPP_H

#include "CorePrerequisites.h"
#include "Object.h"
#include "ClassInfo.h"

namespace hare
{
    class CORE_API GameApp : public Object
    {
        HARE_DECLARE_ABSTRACT_CLASS(GameApp)
    public:
        GameApp();
        virtual ~GameApp();

        virtual bool go() = 0;
    };
}



#endif
