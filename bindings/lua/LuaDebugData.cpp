#include "LuaDebugData.h"

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
    int count = 0;

    while (lua_getstack(L, stackFrame, &luaDebug) != 0)
    {
        if (lua_getinfo(L, "Sln", &luaDebug))
        {
            int  currentLine = luaDebug.currentline;
            if ((count == 0) || (currentLine != -1))
            {
                String name;
                String source = luaDebug.source;

                if (currentLine == -1)
                    currentLine = 0;

                if (luaDebug.name != NULL)
                    name = StringUtil::format("function %s line %d", luaDebug.name, currentLine);
                else
                    name = StringUtil::format("line %d", currentLine);

                items.push_back(new LuaDebugItem(name, LUA_TNONE, "", LUA_TNONE, source, LUA_NOREF, stackFrame, LUA_DEBUGITEM_LOCALS));
                ++count;
            }
        }

        ++stackFrame;
    }

    return count;
}

int LuaDebugData::enumerateStackEntry(lua_State* L, int stackFrame, DebugReference& refs)
{
    lua_Debug luaDebug = INIT_LUA_DEBUG;
    int count = 0;

    if (lua_getstack(L, stackFrame, &luaDebug) != 0)
    {
        int stack_idx  = 1;
        String name = lua_getlocal(L, &luaDebug, stack_idx);
        while (!name.empty())
        {
            int valueType = LUA_TNONE;
            String value;
            String source = luaDebug.source;

            GetTypeValue(wxlState, -1, &wxl_valuetype, value);

            // FIXME! local tables get the right values for GetTypeValue(...)
            // but when you run wxluaR_ref() to store them they disappear
            // so that the next time wxluaR_ref() is run it reuses the same
            // index as your local table.
            // When using the stack dialog the next wxluaR_ref() is the
            // global table which is very confusing.

            int flag_type = 0;
            int nRef = LUA_NOREF; //RefTable(L, -1, &flag_type, references);

            lua_pop(L, 1); // remove variable value

            items.push_back(new LuaDebugItem(name, LUA_TNONE, value, valueType, source, nRef, 0, flag_type));
            ++count;

            name = lua_getlocal(L, &luaDebug, ++stack_idx);
        }
    }

    return count;
}

int wxLuaDebugData::EnumerateTable(const wxLuaState& wxlState_, int tableRef, int nIndex, wxArrayInt& references)
{
    wxCHECK_MSG(wxlState_.Ok(), 0, wxT("Invalid wxLuaState"));
    wxCHECK_MSG(M_DEBUGREFDATA != NULL, 0, wxT("Invalid ref data"));

    wxLuaState wxlState(wxlState_); // unconst the state
    lua_State* L = wxlState.GetLuaState();
    int count = 0;

    int wxl_keytype   = WXLUA_TNONE;
    int wxl_valuetype = WXLUA_TNONE;
    wxString value;
    wxString name;

    if ((tableRef == -1) || (tableRef == LUA_GLOBALSINDEX))
    {
        lua_pushvalue(L, LUA_GLOBALSINDEX);
        GetTypeValue(wxlState, -1, &wxl_valuetype, value);

        int flag_type = 0;
        int val_ref = RefTable(L, -1, &flag_type, WXLUA_DEBUGITEM_VALUE_REF, references);
        lua_pop(L, 1); // pop globals table

        Add(new wxLuaDebugItem(wxT("Globals"), WXLUA_TNONE, value, WXLUA_TTABLE, wxT(""), val_ref, 0, flag_type));
    }
    else if (tableRef == LUA_REGISTRYINDEX)
    {
        lua_pushvalue(L, LUA_REGISTRYINDEX);
        GetTypeValue(wxlState, -1, &wxl_valuetype, value);

        int flag_type = 0;
        int val_ref = RefTable(L, -1, &flag_type, WXLUA_DEBUGITEM_VALUE_REF, references);
        lua_pop(L, 1); // pop registry table

        Add(new wxLuaDebugItem(wxT("Registry"), WXLUA_TNONE, value, WXLUA_TTABLE, wxT(""), val_ref, 0, flag_type));
    }
    else
    {
        // push the table onto the stack to iterate through
        if (wxluaR_getref(L, tableRef, &wxlua_lreg_debug_refs_key))
        {
            if (lua_isnil(L, -1))
            {
                // assert so we don't crash mysteriously inside Lua on nil
                lua_pop(L, 1); // pop nil
                wxFAIL_MSG(wxT("Invalid wxLua debug reference"));
                return count;
            }

            // Check to see if this is a wxLua LUA_REGISTRYINDEX table
            void *lightuserdata_reg_key = NULL;
            lua_pushlightuserdata(L, &wxlua_lreg_regtable_key); // push key
            lua_rawget(L, LUA_REGISTRYINDEX);
            lua_pushvalue(L, -2); // push value (table we're iterating)
            lua_rawget(L, -2);
            lightuserdata_reg_key = lua_touserdata(L, -1);
            lua_pop(L, 2); // pop wxlua_lreg_regtable_key table and (nil or lightuserdata)

            // Check if the table/userdata has a metatable
            if (lua_getmetatable(L, -1)) // if no metatable then nothing is pushed
            {
                // get the type and value
                GetTypeValue(wxlState, -1, &wxl_valuetype, value);

                int flag_type = 0;
                int val_ref = RefTable(L, -1, &flag_type, WXLUA_DEBUGITEM_VALUE_REF, references);

                // leading space so it's first when sorted
                Add(new wxLuaDebugItem(wxT(" __metatable"), WXLUA_TTABLE, value, wxl_valuetype, wxT(""), val_ref, nIndex, flag_type));
                ++count;

                lua_pop(L, 1); // pop metatable
            }

            // start iterating
            if (lua_istable(L, -1))
            {
                lua_pushnil(L);
                while (lua_next(L, -2) != 0)
                {
                    // value at -1, key at -2, table at -3

                    // get the key type and value
                    int lua_key_type = GetTypeValue(wxlState, -2, &wxl_keytype, name);
                    // get the value type and value
                    int lua_value_type = GetTypeValue(wxlState, -1, &wxl_valuetype, value);

                    // Handle wxLua LUA_REGISTRYINDEX tables to give more information
                    if (lightuserdata_reg_key != NULL)
                    {
                        if (lightuserdata_reg_key == &wxlua_lreg_types_key)
                        {
                            value += wxT(" (") + wxluaT_typename(L, (int)lua_tonumber(L, -2)) + wxT(")");
                        }
                        else if (lightuserdata_reg_key == &wxlua_lreg_classes_key)
                        {
                            wxLuaBindClass* wxlClass = (wxLuaBindClass*)lua_touserdata(L, -1);
                            value += wxLuaBindClassString(wxlClass);
                        }
                        else if (lightuserdata_reg_key == &wxlua_lreg_wxluabindings_key)
                        {
                            wxLuaBinding* binding = (wxLuaBinding*)lua_touserdata(L, -2);
                            name = wxT("wxLuaBinding(")+name+wxT(") -> ")+binding->GetBindingName();
                            value += wxT(" = ") + binding->GetLuaNamespace();
                        }
                        else if (lightuserdata_reg_key == &wxlua_lreg_evtcallbacks_key)
                        {
                            wxLuaEventCallback* wxlCallback = (wxLuaEventCallback*)lua_touserdata(L, -2);
                            wxCHECK_MSG(wxlCallback, count, wxT("Invalid wxLuaEventCallback"));

                            wxString s(wxlCallback->GetInfo());
                            name  = s.BeforeFirst(wxT('|'));
                            value = s.AfterFirst(wxT('|'));
                        }
                        else if (lightuserdata_reg_key == &wxlua_lreg_windestroycallbacks_key)
                        {
                            // only handle t[wxWindow*] = wxLuaWinDestroyCallback*
                            wxLuaWinDestroyCallback* wxlDestroyCallBack = (wxLuaWinDestroyCallback*)lua_touserdata(L, -1);
                            wxCHECK_MSG(wxlDestroyCallBack, count, wxT("Invalid wxLuaWinDestroyCallback"));

                            wxString s(wxlDestroyCallBack->GetInfo());
                            name  = s.BeforeFirst(wxT('|'));
                            value = s.AfterFirst(wxT('|'));
                        }
                        else if (lightuserdata_reg_key == &wxlua_lreg_topwindows_key)
                        {
                            wxWindow* win = (wxWindow*)lua_touserdata(L, -2);
                            name += wxT(" ") + wxString(win->GetClassInfo()->GetClassName());
                        }
                        else if (lightuserdata_reg_key == &wxlua_lreg_gcobjects_key)
                        {
                            wxObject* obj = (wxObject*)lua_touserdata(L, -1);
                            name = wxString(obj->GetClassInfo()->GetClassName()) + wxT("(") + name + wxT(")");
                        }
                    }

                    // For these keys we know what is in the value to give more information
                    if (lua_key_type == LUA_TLIGHTUSERDATA)
                    {
                        void* key = lua_touserdata(L, -2);

                        if (key == &wxlua_metatable_type_key)
                        {
                            value += wxT(" (") + wxluaT_typename(L, (int)lua_tonumber(L, -1)) + wxT(")");
                        }
                        else if (key == &wxlua_metatable_wxluabindclass_key)
                        {
                            wxLuaBindClass* wxlClass = (wxLuaBindClass*)lua_touserdata(L, -1);
                            value += wxLuaBindClassString(wxlClass);
                        }
                        else if (key == &wxlua_lreg_debug_refs_key)
                        {
                            value += wxT(" Note: You cannot traverse refed tables");
                        }
                    }

                    // ----------------------------------------------------------
                    // Handle the key

                    int key_flag_type = 0;
                    int key_ref = LUA_NOREF;

                    // don't ref anything in this table since it's already refed
                    if ((lua_key_type == LUA_TTABLE) && (lightuserdata_reg_key != &wxlua_lreg_debug_refs_key))
                        key_ref = RefTable(L, -2, &key_flag_type, WXLUA_DEBUGITEM_KEY_REF, references);
                    else if (lua_key_type == LUA_TUSERDATA)
                    {
                        if (lua_getmetatable(L, -2)) // doesn't push anything if nil
                        {
                            key_ref = RefTable(L, -2, &key_flag_type, WXLUA_DEBUGITEM_KEY_REF, references);
                            lua_pop(L, 1);
                        }
                    }

                    // only add the key if we refed it so it can be viewed in the stack dialog
                    if (key_flag_type != 0)
                    {
                        Add(new wxLuaDebugItem(name, wxl_keytype, value, wxl_valuetype, wxT(""), key_ref, nIndex, key_flag_type));
                        ++count;
                    }

                    // ----------------------------------------------------------
                    // Handle the value

                    int val_flag_type = 0;
                    int val_ref = LUA_NOREF;

                    // don't ref anything in this table since it's already refed
                    if ((lua_value_type == LUA_TTABLE) && (lightuserdata_reg_key != &wxlua_lreg_debug_refs_key))
                        val_ref = RefTable(L, -1, &val_flag_type, WXLUA_DEBUGITEM_VALUE_REF, references);
                    else if (lua_value_type == LUA_TUSERDATA)
                    {
                        if (lua_getmetatable(L, -1)) // doesn't push anything if nil
                        {
                            val_ref = RefTable(L, -1, &val_flag_type, WXLUA_DEBUGITEM_VALUE_REF, references);
                            lua_pop(L, 1);
                        }
                    }

                    if ((key_flag_type == 0) || ((key_flag_type != 0) && (val_flag_type != 0)))
                    {
                        Add(new wxLuaDebugItem(name, wxl_keytype, value, wxl_valuetype, wxT(""), val_ref, nIndex, val_flag_type));
                        ++count;
                    }

                    lua_pop(L, 1); // pop value, leave key
                }
            }

            lua_pop(L, 1); // remove reference
        }
    }

    return count;
}

