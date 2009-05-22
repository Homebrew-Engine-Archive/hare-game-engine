
#ifndef LUA_DEBUGGEE_H
#define LUA_DEBUGGEE_H

#include "LuaPrerequisites.h"
#include "LuaDebugDefines.h"
#include "LuaDebugData.h"

class LUA_BINDING_API LuaDebuggee : public Singleton<LuaDebuggee>
{
public:
    LuaDebuggee(lua_State* L, const String& server, int port);
   ~LuaDebuggee();

    bool start();
    void stop();

public:
    class LuaThread : public Thread
    {
    public:
        LuaThread(LuaDebuggee *dbg) : debuggee(dbg) {}
    protected:
        virtual void* entry();

        LuaDebuggee *debuggee;
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

protected:
    bool handleDebuggerCmd(int cmd);

    bool debugHook(int event);

    static void luaDebugHook(lua_State *L, lua_Debug *debug);
    static int luaPrint(lua_State *L);

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
    bool enumerateTable(int stackRef, const String& table);
    bool evaluateExpr(int stackRef, const String& expr);

public:
    bool notifyBreak(const String &fileName, int lineNumber);
    bool notifyPrint(const String &errorMsg);
    bool notifyError(const String &errorMsg);
    bool notifyExit();
    bool notifyStackEnumeration(LuaDebugData& debugData);
    bool notifyStackEntryEnumeration(int stackRef, LuaDebugData& debugData);
    bool notifyTableEnumeration(LuaDebugData& debugData, const String& table);
    bool notifyEvaluateExpr(LuaDebugData& debugData);
};

#endif
