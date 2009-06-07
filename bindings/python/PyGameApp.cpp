//***************************************************************
//  File:    PyGameApp.cpp
//  Data:    05/29/2009
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
#include "PyGameApp.h"

String PyErr_GetAsString()
{
    if (!PyErr_Occurred())
        return StringUtil::EMPTY;

    String errStr;

    PyObject *type, *value, *tb;
    PyErr_Fetch(&type, &value, &tb);

    PyObject *module = NULL, *cls = NULL, *stringIO = NULL, *result = NULL;
    PyObject *_stderr = PySys_GetObject("stderr");
    if (!_stderr)
        goto err;

    module = PyImport_ImportModule("cStringIO");
    if (!module)
        goto err;

    cls = PyObject_GetAttrString(module, "StringIO");
    Py_DECREF(module);
    if (!cls)
        goto err;

    stringIO = PyObject_CallObject(cls, NULL);
    Py_DECREF(cls);
    if (!stringIO)
        goto err;

    Py_INCREF(_stderr);
    PySys_SetObject("stderr", stringIO);

    PyErr_Restore(type, value, tb);
    PyErr_Print();

    result = PyObject_CallMethod(stringIO, "getvalue", NULL);
    Py_DECREF(stringIO);

    if (result != NULL)
    {
        errStr += PyString_AsString(result);
        Py_DECREF(result);
    }

    PySys_SetObject("stderr", _stderr);
    Py_DECREF(_stderr);

    return errStr;

err:
    PyErr_Restore(type, value, tb);
    return StringUtil::EMPTY;
}

bool notify_error(const String& err)
{
    Log::getSingleton().logError("Python error : %s", err.c_str());

    return true;
}

bool notify_error()
{
    if (!PyErr_Occurred())
        return false;

    return notify_error(PyErr_GetAsString());
}

extern "C"
{
    void init_hare(void);
}

HARE_IMPLEMENT_DYNAMIC_CLASS(PyGameApp, GameApp, 0)
{
    HARE_OBJ(mainScript, PyScriptRunner)
}

bool PyGameApp::go()
{
    Py_Initialize();

    init_hare();

    if (mainScript)
    {
        mainScript->loadScript();
    }
    else
    {
        mainScript = new PyScriptRunner();

        String game = CmdLineParser::getSingletonPtr()->getOptionValue("game");

        String script;

        if (game.empty()) 
            script = "/script.py";
        else
            script = "/" + game + "/script.py";

        mainScript->loadScript(script);
    }

    if (!mainScript->isLoaded())
    {
        notify_error();
        Py_Finalize();
        return false;
    }

    mainScript->notifyOwnerCreated();

    HareApp::getSingletonPtr()->hareRun();

    mainScript->notifyOwnerDestroyed();

    Py_Finalize();

    mainScript = 0;

    return true;
}

