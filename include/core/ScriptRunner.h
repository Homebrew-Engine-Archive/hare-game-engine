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
    /** ScriptRunner for running scripts
    */
    class CORE_API ScriptRunner : public Object
    {
        HARE_DECLARE_ABSTRACT_CLASS(ScriptRunner)

    public:
        ScriptRunner();
        virtual ~ScriptRunner();

        /// Get script file path (filesystem url)
        const String& getScriptFile() const
        {
            return scriptFile;
        }

        /// Get script's owner object
        Object* getOwner() const
        {
            return owner;
        }

        /// Set script's owner object
        void setOwner(Object* object)
        {
            owner = object;
        }

        /// Is script loaded ?
        bool isLoaded() const
        {
            return loaded;
        }

        /// Notify script that owner object has been created
        virtual bool notifyOwnerCreated() = 0;
        /// Notify script that owner object will be destroyed
        virtual bool notifyOwnerDestroyed() = 0;

    protected:
        String scriptFile;
        Object* owner;
        bool loaded;
    };
}

#endif
