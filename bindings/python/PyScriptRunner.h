//***************************************************************
//  File:    PyScriptRunner.h
//  Data:    05/30/2009
//  Author:  littlesome (littlesome@live.cn)
//-------------------------------------------------------------
//  
//-------------------------------------------------------------
//  This file is part of Hare2D Game Engine.
//  Copyright (C) All Rights Reserved
//***************************************************************
// 
//***************************************************************
#ifndef PYSCRIPTRUNNER_H
#define PYSCRIPTRUNNER_H

class PyScriptRunner : public ScriptRunner
{
    HARE_DECLARE_DYNAMIC_CLASS(PyScriptRunner)
public:
    PyScriptRunner();

    String getLastError(bool clear = true)
    {
        String err = error;
        if (clear) error.clear();
        return err;
    }

    bool loadScript(const String& fileName = StringUtil::EMPTY);

    bool callFunction(const String& name);

    virtual bool notifyOwnerCreated();
    virtual bool notifyOwnerDestroyed();

    virtual void postLoaded();

private:
    PyObject* module;
    String error;
};

#endif