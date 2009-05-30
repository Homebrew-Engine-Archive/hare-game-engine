//***************************************************************
//  File:    PyEventHandler.h
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
#ifndef PYEVENTHANDLER_H
#define PYEVENTHANDLER_H

class PyEventCallbackData : public ReferenceCounted
{
    HARE_DECLARE_PTR(PyEventCallbackData)
public:
    PyEventCallbackData(PyObject* func)
    {
        this->func = func;
    }

    PyObject* func;

    virtual void _doRelease()
    {
        delete this;
    }
};

class PyEventHandler : public EventHandler
{
public:
    void connect(Window* window, int id, int lastId, EventType eventType, PyObject* func)
    {
        PyEventCallbackData* userData = new PyEventCallbackData(func);

        window->connect(id, lastId, eventType, EventFunction(&PyEventHandler::onPyEvent), userData, this);

        callbackData.push_back(userData);
    }

    void disconnect(Window* window, int id, int lastId, EventType eventType)
    {
        window->disconnect(id, lastId, eventType, EventFunction(&PyEventHandler::onPyEvent), NULL, this);
    }

protected:
    void onPyEvent(Event& event)
    {
        PyEventCallbackData* userData = (PyEventCallbackData*)event.userData;
        assert(userData);

        if (userData)
        {
            PyObject* s = dynamicCastObject(event.getEventSender(), 0);
            PyObject* e = dynamicCastObject(&event, 0);
            

        }

    }

    PyEventCallbackData::List callbackData;
};

PyEventHandler* getPyEventHandler()
{
    static PyEventHandler thePyEventHandler;
    return &thePyEventHandler;
}

void connectSelf(Window* window, EventType eventType, PyObject* func)
{
    getPyEventHandler()->connect(window, uiID_Any, uiID_Any, eventType, func);
}

void connectChild(Window* window, int id, EventType eventType, PyObject* func)
{
    getPyEventHandler()->connect(window, id, uiID_Any, eventType, func);
}

void connectRange(Window* window, int id, int lastId, EventType eventType, PyObject* func)
{
    getPyEventHandler()->connect(window, id, lastId, eventType, func);
}

void disconnectSelf(Window* window, EventType eventType)
{
    getPyEventHandler()->disconnect(window, uiID_Any, uiID_Any, eventType);
}

void disconnectChild(Window* window, int id, EventType eventType)
{
    getPyEventHandler()->disconnect(window, id, uiID_Any, eventType);
}

void disconnectRange(Window* window, int id, int lastId, EventType eventType)
{
    getPyEventHandler()->disconnect(window, id, lastId, eventType);
}


#endif