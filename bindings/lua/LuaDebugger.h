
#ifndef LUA_DEBUGGER_H
#define LUA_DEBUGGER_H

#include "core/Core.h"
using namespace hare_core;

extern "C" 
{
    #include <lua.h>
    #include <lauxlib.h>
    #include <lualib.h>
}

#include "LuaDebugDefines.h"

class LuaDebugger : public Singleton<LuaDebugger>
{
public:
    LuaDebugger(lua_State* L, const String& server, int port);
   ~LuaDebugger();

    bool start();
    void stop();

public:
    class LuaThread : public Thread
    {
    public:
        LuaThread(LuaDebugger *dbg) : debugger(dbg) {}
    protected:
        virtual void* entry();

        LuaDebugger *debugger;
    };

    void threadFunction();

protected:

    lua_State* state;

    LuaThread* debugThread;
    Socket socket;
    String serverName;
    int portNumber;
    bool connected;

    bool running;
    Mutex runMutex;
    Condition runCondition;

    bool stopped;
    Mutex debugMutex;
    Condition debugCondition;

    int framesUntilBreak;

    bool forceBreak;
    bool resetRequested;

    CriticalSection luaCS;

    enum DebugOperations
    {
        DEBUG_STEP,
        DEBUG_STEPOVER,
        DEBUG_STEPOUT,
        DEBUG_GO
    };

    DebugOperations nextOperation; 

    typedef std::set<String> BreakPointSet;

    BreakPointSet breakPoints;
    CriticalSection breakPointsCS;

    DebugReference debugRefs;

protected:
    bool handleDebuggerCmd(int cmd);

    bool debugHook(int event);

    static void luaDebugHook(lua_State *L, lua_Debug *debug);

    bool isConnected(bool wait = true) const;

    void enterLuaCriticalSection() { luaCS.enter(); }
    void leaveLuaCriticalSection() { luaCS.leave(); }

    String toBreakPoint(const String &fileName, int lineNumber) const;

    bool hasBreakPoint(const String &fileName, int lineNumber);
    
    bool addBreakPoint(const String &fileName, int lineNumber);
    
    bool removeBreakPoint(const String &fileName, int lineNumber);
    
    bool clearAllBreakPoints();
    
    bool step();
    bool stepOver();
    bool stepOut();
    bool continueExec();
    bool breakExec();
    bool reset();
    
    bool enumerateStack();
    bool enumerateStackEntry(int stackRef);
    bool enumerateTable(int tableRef, int index, long itemNode);

    bool notifyBreak(const String &fileName, int lineNumber);
    bool notifyPrint(const String &errorMsg);
    bool notifyError(const String &errorMsg);
    bool notifyExit();
    bool notifyStackEnumeration(LuaDebugData& debugData);
    bool notifyStackEntryEnumeration(int stackRef, LuaDebugData& debugData);
    bool notifyTableEnumeration(long itemNode, LuaDebugData& debugData);
};

#endif