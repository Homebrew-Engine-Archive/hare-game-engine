#ifndef LUA_DEBUG_DATA_H
#define LUA_DEBUG_DATA_H

#include "core/Core.h"
using namespace hare_core;

extern "C" 
{
    #include <lua.h>
    #include <lauxlib.h>
    #include <lualib.h>
}

enum LuaDebugDataItem
{
    LUA_DEBUGITEM_NONE = 0,
    LUA_DEBUGITEM_CALLSTACK,
    LUA_DEBUGITEM_STACKFRAME,
};

class LuaDebugItem : public Object
{
    HARE_DECLARE_DYNAMIC_CLASS(LuaDebugItem)

public:
    LuaDebugItem(const String& key, u8 keyType, const String& value,
        u8 valueType, const String& source, s32 ref, s32 idx, s32 flg)
        : itemKey(key), itemKeyType(keyType), itemValue(value), 
          itemValueType(valueType), itemSource(source), luaRef(ref),
          index(idx), flag(flg) {}

    LuaDebugItem() : itemKeyType(LUA_TNONE), itemValueType(LUA_TNONE), 
        luaRef(LUA_NOREF), index(0), flag(LUA_DEBUGITEM_NONE) {}

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
    HARE_DECLARE_DYNAMIC_CLASS(LuaDebugData)

public:
    void enumerateStack(lua_State* L);
    void enumerateStackEntry(lua_State* L, int stackFrame);
    void enumerateTable(lua_State* L, int tableRef, int index);

private:
    int getTypeValue(lua_State* L, int stack_idx, int* type, String& value);
    String getTableInfo(lua_State* L, int stack_idx);
    String getUserDataInfo(lua_State* L, int stack_idx);

public:
    LuaDebugItem::Array items;
};

#endif