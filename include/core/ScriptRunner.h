//***************************************************************
//  File:    ScriptRunner.h
//  Data:    05/20/2009
//  Author:  littlesome (littlesome@live.cn)
//-------------------------------------------------------------
//  
//-------------------------------------------------------------
//  This file is part of Hare2D Game Engine.
//  Copyright (C) All Rights Reserved
//***************************************************************
// 
//***************************************************************
#ifndef SCRIPTRUNNER_H
#define SCRIPTRUNNER_H

#include "CorePrerequisites.h"
#include "Object.h"

namespace hare
{
    class CORE_API ScriptRunner : public Object
    {
        HARE_DECLARE_ABSTRACT_CLASS(ScriptRunner)

    public:
        ScriptRunner();
        virtual ~ScriptRunner();

        const String& getScriptFile() const
        {
            return scriptFile;
        }

        Object* getOwner() const
        {
            return owner;
        }

        void setOwner(Object* object)
        {
            owner = object;
        }

    protected:
        String scriptFile;
        Object* owner;
    };
}

#endif
