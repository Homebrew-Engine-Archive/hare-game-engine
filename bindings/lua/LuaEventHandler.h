//***************************************************************
//  File:    LuaEventHandler.h
//  Data:    05/25/2009
//  Author:  littlesome (littlesome@live.cn)
//-------------------------------------------------------------
//  
//-------------------------------------------------------------
//  This file is part of Hare2D Game Engine.
//  Copyright (C) All Rights Reserved
//***************************************************************
// 
//***************************************************************
#ifndef LUAUIEVENTHANDLER_H
#define LUAUIEVENTHANDLER_H

class LuaEventCallbackData : public ReferenceCounted
{
    HARE_DECLARE_PTR(LuaEventCallbackData)
public:
    LuaEventCallbackData(const SWIGLUA_REF &func)
    {
        this->func = func;
    }

    SWIGLUA_REF func;

    virtual void _doRelease()
    {
        delete this;
    }
};

class LuaEventHandler : public EventHandler
{
public:

    void connect(Window* window, int id, int lastId, EventType eventType, SWIGLUA_REF func)
    {
        LuaEventCallbackData* userData = new LuaEventCallbackData(func);

        window->connect(id, lastId, eventType, EventFunction(&LuaEventHandler::onLuaEvent), userData, this);

        callbackData.push_back(userData);
    }

    void disconnect(Window* window, int id, int lastId, EventType eventType)
    {
        window->disconnect(id, lastId, eventType, EventFunction(&LuaEventHandler::onLuaEvent), NULL, this);

        // TODO : Free LuaEventCallbackData right now?
    }

protected:
    void onLuaEvent(Event& event)
    {
        LuaEventCallbackData* userData = (LuaEventCallbackData*)event.userData;
        assert(userData);

        if (userData)
        {
            swiglua_ref_get(&userData->func);

            dynamicCastObject(userData->func.L, event.getEventSender(), 0);
            dynamicCastObject(userData->func.L, &event, 0);

            if (lua_pcall(userData->func.L, 2, LUA_MULTRET, 0))
                notify_error(userData->func.L);
        }

    }

    LuaEventCallbackData::List callbackData;
};

LuaEventHandler* getLuaEventHandler()
{
    static LuaEventHandler theLuaEventHandler;
    return &theLuaEventHandler;
}

void connectSelf(Window* window, EventType eventType, SWIGLUA_REF func)
{
    getLuaEventHandler()->connect(window, uiID_Any, uiID_Any, eventType, func);
}

void connectChild(Window* window, int id, EventType eventType, SWIGLUA_REF func)
{
    getLuaEventHandler()->connect(window, id, uiID_Any, eventType, func);
}

void connectRange(Window* window, int id, int lastId, EventType eventType, SWIGLUA_REF func)
{
    getLuaEventHandler()->connect(window, id, lastId, eventType, func);
}

void disconnectSelf(Window* window, EventType eventType)
{
    getLuaEventHandler()->disconnect(window, uiID_Any, uiID_Any, eventType);
}

void disconnectChild(Window* window, int id, EventType eventType)
{
    getLuaEventHandler()->disconnect(window, id, uiID_Any, eventType);
}

void disconnectRange(Window* window, int id, int lastId, EventType eventType)
{
    getLuaEventHandler()->disconnect(window, id, lastId, eventType);
}


#endif