#ifndef LUA_DEBUG_DATA_H
#define LUA_DEBUG_DATA_H

#include "core/Core.h"
using namespace hare_core;

class LuaDebugItem : public Object
{
    HARE_DECLARE_DYNAMIC_CLASS(LuaDebugItem)

public:
    String   itemKey;
    u8       itemKeyType;

    String   itemValue;
    u8       itemValueType;

    String   itemSource;
    s32      luaRef;
    s32      index;
    s32      flag;
};

class LuaDebugData : public Object
{
public:
    int enumerateStack(lua_State* L);

private:
    LuaDebugItem::Array items;
};

#endif