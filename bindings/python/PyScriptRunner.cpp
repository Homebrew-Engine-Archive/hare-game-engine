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

    char* buffer = new char[size];
    fs->readFile(fh, buffer, size, 1);

    PyObject* code = Py_CompileString(buffer, scriptFile.c_str(), Py_file_input); 
    if (code)
    {
        module = PyImport_ExecCodeModule("hare_python_temp_module", code);
        Py_DECREF(code);
        if (module)
        {
            PyObject *obj = PyImport_GetModuleDict();
            PyDict_DelItemString(obj, "hare_python_temp_module");
        }
    }
    else
    {

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

    Py_XDECREF(ret);

    return (ret != NULL);
}

bool PyScriptRunner::notifyOwnerCreated()
{
    if (!callFunction("onCreate"))
    {
        return false;
    }

    return true;
}

bool PyScriptRunner::notifyOwnerDestroyed()
{
    if (!callFunction("onDestroy"))
    {
        return false;
    }

    return true;
}

void PyScriptRunner::postLoaded()
{
    if (!scriptFile.empty())
    {
        loadScript();
    }
}


