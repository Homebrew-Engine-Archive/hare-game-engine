#include "LuaDebugData.h"
#include "LuaDebugDefines.h"

HARE_IMPLEMENT_DYNAMIC_CLASS(LuaDebugItem, Object, 0)
{
    HARE_META(itemKey, String)
    HARE_META(itemKeyType, u8)
    HARE_META(itemValue, String)
    HARE_META(itemValueType, u8)
    HARE_META(itemSource, String)
    HARE_META(luaRef, s32)
    HARE_META(index, s32)
    HARE_META(flag, s32)
}

HARE_IMPLEMENT_DYNAMIC_CLASS(LuaDebugData, Object, 0)
{
    HARE_OBJ_ARRAY(items, LuaDebugItem)
}

void LuaDebugData::enumerateStack(lua_State* L)
{
    lua_Debug luaDebug = INIT_LUA_DEBUG;
    int stackFrame = 0;

    while (lua_getstack(L, stackFrame, &luaDebug) != 0)
    {
        if (lua_getinfo(L, "Sln", &luaDebug))
        {
            int currentLine = luaDebug.currentline;
            if ((stackFrame == 0) || (currentLine != -1))
            {
                String name;
                String source = luaDebug.source;

                if (currentLine == -1)
                    currentLine = 0;

                if (luaDebug.name != NULL)
                    name = StringUtil::format("function %s line %d", luaDebug.name, currentLine);
                else
                    name = StringUtil::format("line %d", currentLine);

                String lineStr = StringConverter::toString(currentLine - 1);

                items.push_back(new LuaDebugItem(name, LUA_TNONE, lineStr, 
                    LUA_TNONE, source, LUA_NOREF, stackFrame, LUA_DEBUGITEM_CALLSTACK));
            }
        }

        ++stackFrame;
    }
}

void LuaDebugData::enumerateStackEntry(lua_State* L, int stackFrame)
{
    lua_Debug luaDebug = INIT_LUA_DEBUG;

    if (lua_getstack(L, stackFrame, &luaDebug) != 0)
    {
        int stack_idx  = 1;
        String name = lua_getlocal(L, &luaDebug, stack_idx);
        while (!name.empty())
        {
            String source = luaDebug.source;
            String value;
            int valueType = LUA_TNONE;

            getTypeValue(L, -1, &valueType, value);

            lua_pop(L, 1); // remove variable value

            items.push_back(new LuaDebugItem(name, LUA_TNONE, value, valueType, 
                source, LUA_NOREF, 0, LUA_DEBUGITEM_STACKFRAME));

            name = lua_getlocal(L, &luaDebug, ++stack_idx);
        }
    }
}

void LuaDebugData::enumerateTable(lua_State* L, int tableRef, int index)
{

}


static void coverGlobals(lua_State* L, int stackRef)
{
    lua_newtable(L);  // save there globals covered by locals

    lua_Debug luaDebug = INIT_LUA_DEBUG;
    if (lua_getstack (L, stackRef, &luaDebug))
    {
        int i = 1;
        const char *name = 0;
        while ((name = lua_getlocal(L, &luaDebug, i++)) != NULL) { /* SAVE lvalue */
            lua_pushstring(L, name);	/* SAVE lvalue name */						
            lua_pushvalue(L, -1);	/* SAVE lvalue name name */
            lua_pushvalue(L, -1);	/* SAVE lvalue name name name */
            lua_insert(L, -4);		/* SAVE name lvalue name name */
            lua_rawget(L, LUA_GLOBALSINDEX);	/* SAVE name lvalue name gvalue */
            lua_rawset(L, -5);	// save global value in local table
            /* SAVE name lvalue */
            lua_rawset(L, LUA_GLOBALSINDEX); /* SAVE */
        }
    }
}

static void restoreGlobals(lua_State* L)
{
    // there is table of covered globals on top

    lua_pushnil(L);  /* first key */
    /* SAVE nil */
    while (lua_next(L, -2))		/* SAVE key value */
    {
        lua_pushvalue(L, -2);   /* SAVE key value key */
        lua_insert(L, -2);      /* SAVE key key value */

        lua_rawset(L, LUA_GLOBALSINDEX);	// restore global
        /* SAVE key */
    }

    lua_pop(L, 1); // pop table of covered globals;
}

void LuaDebugData::evaluateExpr(lua_State* L, int stackRef, const String& expr)
{
    String exprExec = "return " + expr;
    String value;
    int valueType = LUA_TNONE;

    coverGlobals(L, stackRef);

    int top = lua_gettop(L);

    const char* code = exprExec.c_str();

    int status = luaL_loadbuffer(L, code, strlen(code), code);
    if (status)
    {
        const char* szErr = luaL_checkstring(L, -1);
        const char* szErr2 = strstr(szErr, ": ");
        value = szErr2 ? (szErr2 + 2) : szErr;
    }
    else
    {
        status = lua_pcall(L, 0, LUA_MULTRET, 0);  /* call main */
        if (status)
        {
            const char* szErr = luaL_checkstring(L, -1);
            const char* szErr2 = strstr(szErr, ": ");
            value = szErr2 ? (szErr2 + 2) : szErr;
        }
        else
            getTypeValue(L, -1, &valueType, value);
    }

    items.push_back(new LuaDebugItem(expr, LUA_TNONE, 
        value, valueType, StringUtil::EMPTY, LUA_NOREF, 0, LUA_DEBUGITEM_NONE));

    lua_settop(L, top);

    restoreGlobals(L);
}

int LuaDebugData::getTypeValue(lua_State* L, int stack_idx, int* type, String& value)
{
    int l_type = lua_type(L, stack_idx);

    if (type) *type = l_type;

    switch (l_type)
    {
    case LUA_TNONE:
        {
            value = StringUtil::EMPTY;
            break;
        }
    case LUA_TNIL:
        {
            value = "nil";
            break;
        }
    case LUA_TBOOLEAN:
        {
            value = (lua_toboolean(L, stack_idx) != 0) ? "true" : "false";
            break;
        }
    case LUA_TLIGHTUSERDATA:
        {
            value = getUserDataInfo(L, stack_idx);
            break;
        }
    case LUA_TNUMBER:
        {
            double num = lua_tonumber(L, stack_idx);

            if ((long)num == num)
                value = StringUtil::format("%ld (0x%lx)", (long)num, (unsigned long)num);
            else
                value = StringUtil::format("%g", num);

            break;
        }
    case LUA_TSTRING:
        {
            value = lua_tostring(L, stack_idx);
            break;
        }
    case LUA_TTABLE:
        {
            value = getTableInfo(L, stack_idx);
            break;
        }
    case LUA_TFUNCTION:
        {
            value = StringUtil::format("%p", lua_topointer(L, stack_idx));
            break;
        }
    case LUA_TUSERDATA:
        {
            value = getUserDataInfo(L, stack_idx);
            break;
        }
    case LUA_TTHREAD:
        {
            value = StringUtil::format("%p", lua_topointer(L, stack_idx));
            break;
        }
    default :
        {
            value = StringUtil::EMPTY;
            break;
        }
    }

    return l_type;
}


String LuaDebugData::getTableInfo(lua_State* L, int stack_idx)
{
    int num = luaL_getn(L, stack_idx);
    const void *ptr = lua_topointer(L, stack_idx);

    String s(StringUtil::format("%p", ptr));

    if (num > 0)
        s += StringUtil::format(" (%d items)", num);

    return s;
}

String LuaDebugData::getUserDataInfo(lua_State* L, int stack_idx)
{
    void* udata = lua_touserdata(L, stack_idx);

    String s(StringUtil::format("%p", udata));

    // what about SWIG_Lua_typename ?

    return s;
}