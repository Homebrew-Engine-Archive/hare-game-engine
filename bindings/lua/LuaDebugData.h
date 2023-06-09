#ifndef LUA_DEBUG_DATA_H
#define LUA_DEBUG_DATA_H

#include "core/Core.h"
using namespace hare;

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
    LUA_DEBUGITEM_TABLEITEMS,
    LUA_DEBUGITEM_EXPRVALUE,

};

class LuaDebugItem : public Object
{
    HARE_DECLARE_DYNAMIC_CLASS(LuaDebugItem)

public:
    LuaDebugItem(const String& key, uint8 keyType, const String& value,
        uint8 valueType, const String& source, int32 idx, int32 flg)
        : itemKey(key), itemKeyType(keyType), itemValue(value), itemValueType(valueType),
          itemSource(source), index(idx), flag(flg) {}

    LuaDebugItem() : itemKeyType(LUA_TNONE), itemValueType(LUA_TNONE),
        index(0), flag(LUA_DEBUGITEM_NONE) {}

public:
    String  itemKey;
    uint8   itemKeyType;
    String  itemValue;
    uint8   itemValueType;
    String  itemSource;
    int32   index;
    int32   flag;
};

class LuaDebugData : public Object
{
    HARE_DECLARE_DYNAMIC_CLASS(LuaDebugData)

public:
    void enumerateStack(lua_State* L);
    void enumerateStackEntry(lua_State* L, int stackRef);
    void enumerateTable(lua_State* L, int stackRef, const String& table);
    void evaluateExpr(lua_State* L, int stackRef, const String& expr);

private:
    int getTypeValue(lua_State* L, int stack_idx, int* type, String& value);
    String getTableInfo(lua_State* L, int stack_idx);
    String getUserDataInfo(lua_State* L, int stack_idx);

public:
    LuaDebugItem::Array items;
};

#endif
