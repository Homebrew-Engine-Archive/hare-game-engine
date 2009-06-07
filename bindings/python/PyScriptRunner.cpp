//***************************************************************
//  File:    PyScriptRunner.cpp
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
#include "PCH.h"
#include "PyScriptRunner.h"

PyObject* dynamicCastObject(Object* object, int owner);
String PyErr_GetAsString();
bool notify_error(const String& err);

HARE_IMPLEMENT_DYNAMIC_CLASS(PyScriptRunner, ScriptRunner, 0)
{
}

PyScriptRunner::PyScriptRunner()
{
    module = NULL;
}

bool PyScriptRunner::loadScript(const String& fileName)
{
    if (!fileName.empty())
    {
        scriptFile = fileName;
    }

    if (!Py_IsInitialized())
    {
        error = "PyScriptRunner::loadScript() Py_Initialized() MUST be called first.";
        return false;
    }

    if (!StringUtil::startsWith(scriptFile, "/"))
    {
        error = "PyScriptRunner::loadScript() fileName MUST start with '/'";
        return false;
    }

    FileSystem* fs = FileSystem::getSingletonPtr();

    FileHandle fh = fs->openFile(scriptFile, FM_Read);
    if (!fh)
    {
        error = "PyScriptRunner::loadScript() can not open file : " + scriptFile;
        return false;
    }

    int size = fs->size(fh);
    if (size <= 0)
    {
        fs->closeFile(fh);
        error = "PyScriptRunner::loadScript() empty file : " + scriptFile;
        return false;
    }

    char* buffer = new char[size + 1];
    fs->readFile(fh, buffer, size, 1);
    buffer[size] = 0;

    /* Python's Doc says : When embedding Python, source code strings should be passed to 
     *   Python APIs using the standard C conventions for newline characters 
     *   (the \n character, representing ASCII LF, is the line terminator).
     */
    String buff = StringUtil::replace(buffer, "\r", "");

    PyObject* code = Py_CompileString(buff.c_str(), scriptFile.c_str(), Py_file_input); 
    if (code)
    {
        module = PyImport_ExecCodeModule("hare_python_temp_module", code);
        Py_DECREF(code);
        if (module)
        {
            PyObject *obj = PyImport_GetModuleDict();
            PyDict_DelItemString(obj, "hare_python_temp_module");
        }
        else
        {
            error = "PyScriptRunner::loadScript() PyImport_ExecCodeModule failed : \n" + PyErr_GetAsString();
        }
    }
    else
    {
        error = "PyScriptRunner::loadScript() Py_CompileString failed : \n" + PyErr_GetAsString();
    }
    delete [] buffer;

    loaded = (module != NULL);

    return loaded;
}

bool PyScriptRunner::callFunction(const String& name)
{
    if (!loaded)
    {
        error = "PyScriptRunner::callFunction() script is not loaded.";
        return false;
    }

    PyObject* object = dynamicCastObject(owner, 0);

    PyObject* ret = PyObject_CallMethod(module, (char*)name.c_str(), "O", object);

    if (!ret)
    {
        error = "PyScriptRunner::callFunction() PyObject_CallMethod failed : \n" + PyErr_GetAsString();
    }

    Py_XDECREF(ret);

    return (ret != NULL);
}

bool PyScriptRunner::notifyOwnerCreated()
{
    if (!callFunction("onCreate"))
    {
        notify_error(getLastError());
        return false;
    }

    return true;
}

bool PyScriptRunner::notifyOwnerDestroyed()
{
    if (!callFunction("onDestroy"))
    {
        notify_error(getLastError());
        return false;
    }

    return true;
}

void PyScriptRunner::postLoaded()
{
    if (Py_IsInitialized() && !scriptFile.empty())
    {
        loadScript();
    }
}


