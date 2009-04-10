#include "LuaDebuggee.h"
#include "SocketHelper.h"

#define MAX_SOCKET_CONNECT_TIMEOUT  200

LuaDebuggee::LuaDebuggee(lua_State* L, const String& server, int port)
: state(L), serverName(server), portNumber(port), connected(false), debugThread(0),
  debugCondition(debugMutex), forceBreak(false), resetRequested(false), running(false),
  stopped(false), framesUntilBreak(0), runCondition(runMutex)
{
    lua_sethook(L, LuaDebuggee::luaDebugHook, LUA_MASKCALL | LUA_MASKLINE | LUA_MASKRET, 0);

    lua_pushcfunction(L, luaPrint);
    lua_setglobal(L, "print");

    enterLuaCriticalSection();
}

LuaDebuggee::~LuaDebuggee()
{
    leaveLuaCriticalSection();
}

void LuaDebuggee::luaDebugHook(lua_State *L, lua_Debug *debug)
{
    LuaDebuggee* debuggee = LuaDebuggee::getSingletonPtr();
    if (debuggee)
        debuggee->debugHook(debug->event);
}

int LuaDebuggee::luaPrint(lua_State *L)
{
    int idx;
    String stream;
    int n = lua_gettop(L);  /* number of arguments */
    lua_getglobal(L, "tostring");
    for (idx = 1; idx <= n;  idx++)
    {
        lua_pushvalue(L, -1);  /* function to be called */
        lua_pushvalue(L, idx);   /* value to print */
        lua_call(L, 1, 1);
        String s = lua_tostring(L, -1);  /* get result */
        if (s.empty())
            return luaL_error(L, "`tostring' must return a string to `print'");
        if (idx > 1)
            stream.append("\t");
        stream.append(s);
        lua_pop(L, 1);  /* pop result */
    }

    stream.append("\n");

    LuaDebuggee* debuggee = LuaDebuggee::getSingletonPtr();
    if (debuggee)
        debuggee->notifyPrint(stream);

    return 0;
}

bool LuaDebuggee::debugHook(int event)
{
    bool wait = false;
    stopped = true;

    int lineNumber = 0;
    String fileName;

    if (!(forceBreak && resetRequested))
    {
        lua_Debug luaDebug = INIT_LUA_DEBUG;
        lua_getstack(state, 0, &luaDebug);
        lua_getinfo(state, "Sln", &luaDebug);
        lineNumber = luaDebug.currentline - 1;
        fileName = luaDebug.source;
    }

    if (forceBreak)
    {
        if (resetRequested)
        {
            wait = true;
            exit(-1);
        }
        else
        {
            if (notifyBreak(fileName, lineNumber))
            {
                forceBreak = false;
                wait = true;
            }
        }
    }
    else
    {
        if (event == LUA_HOOKCALL)
        {
            framesUntilBreak++;
        }
        else if ((event == LUA_HOOKRET) || (event == LUA_HOOKTAILRET))
        {
            framesUntilBreak--;
        }
        else if (event == LUA_HOOKLINE)
        {
            if (hasBreakPoint(fileName, lineNumber) ||
                (nextOperation == DEBUG_STEP) ||
                (nextOperation == DEBUG_STEPOVER && framesUntilBreak <= 0) ||
                (nextOperation == DEBUG_STEPOUT && framesUntilBreak < 0))
            {
                if (notifyBreak(fileName, lineNumber))
                    wait = true;
            }
        }
    }

    if (wait)
    {
        leaveLuaCriticalSection();
        debugCondition.wait();
        enterLuaCriticalSection();
    }

    stopped = false;
    return wait;
}

bool LuaDebuggee::start()
{
    // create socket thread (connect to debug server and wait for debug command)

    if (debugThread)
        return true;

    debugThread = new LuaThread(this);

    if (debugThread->create() == THREAD_NO_ERROR &&
        debugThread->run() == THREAD_NO_ERROR)
    {
        if (!isConnected())
        {
            return false;
        }

        runCondition.wait();

        running = true;

        return true;
    }

    return false;
}

void LuaDebuggee::stop()
{
    // close socket and kill thread

    notifyExit();

    if (connected)
    {
        socket.shutdown(SD_BOTH);
        Thread::sleep(100);
        socket.close();
    }

    if (debugThread)
        debugThread->waitExit();
}

bool LuaDebuggee::isConnected(bool wait) const
{
    if (connected || !wait) return connected;

    for (int idx = 0; idx < MAX_SOCKET_CONNECT_TIMEOUT; ++idx)
    {
        if (connected)
            break;

        Thread::sleep(100);
    }
    return connected;
}
// -----------------------------------------------------------------
//  Socket & thread
// -----------------------------------------------------------------
void* LuaDebuggee::LuaThread::entry()
{
    debuggee->threadFunction();

    return 0;
}

void LuaDebuggee::threadFunction()
{
    bool threadRunning = false;

    if (socket.connect(serverName, portNumber))
    {
        connected = true;
        threadRunning = true;
    }
    else
    {
        printf("%s\n", socket.getLastErrorMsg().c_str());
        return;
    }

    while (threadRunning && !debugThread->testDestroy() && !resetRequested)
    {
        unsigned char debugCommand = 0;
        if (!SocketHelper::readCmd(&socket, debugCommand) ||
            !handleDebuggerCmd(debugCommand))
        {
            threadRunning = false;
        }
    }
}

bool LuaDebuggee::handleDebuggerCmd(int cmd)
{
    bool ret = false;

    switch (cmd)
    {
    case LUA_DEBUGGER_CMD_NONE:
        {
            ret = true;
            break;
        }
    case LUA_DEBUGGER_CMD_ADD_BREAKPOINT:
        {
            String file;
            int line = 0;

            if (SocketHelper::readString(&socket, file) &&
                SocketHelper::readInt(&socket, line))
            {
                ret = addBreakPoint(file, line);
            }
            break;
        }
    case LUA_DEBUGGER_CMD_REMOVE_BREAKPOINT:
        {
            String file;
            int line = 0;

            if (SocketHelper::readString(&socket, file) &&
                SocketHelper::readInt(&socket, line))
            {
                ret = removeBreakPoint(file, line);
            }
            break;
        }
    case LUA_DEBUGGER_CMD_CLEAR_ALL_BREAKPOINTS:
        {
            ret = clearAllBreakPoints();
            break;
        }
    case LUA_DEBUGGER_CMD_DEBUG_STEP:
        {
            ret = step();
            break;
        }
    case LUA_DEBUGGER_CMD_DEBUG_STEPOVER:
        {
            ret = stepOver();
            break;
        }
    case LUA_DEBUGGER_CMD_DEBUG_STEPOUT:
        {
            ret = stepOut();
            break;
        }
    case LUA_DEBUGGER_CMD_DEBUG_CONTINUE:
        {
            ret = continueExec();
            break;
        }
    case LUA_DEBUGGER_CMD_DEBUG_BREAK:
        {
            ret = breakExec();
            break;
        }
    case LUA_DEBUGGER_CMD_ENUMERATE_STACK:
        {
            ret = enumerateStack();
            break;
        }
    case LUA_DEBUGGER_CMD_ENUMERATE_STACK_ENTRY:
        {
            int stackRef = 0;

            if (SocketHelper::readInt(&socket, stackRef))
                ret = enumerateStackEntry(stackRef);

            break;
        }
    case LUA_DEBUGGER_CMD_ENUMERATE_TABLE:
        {
            int stackRef = 0;
            String table;

            if (SocketHelper::readInt(&socket, stackRef) &&
                SocketHelper::readString(&socket, table))
            {
                ret = enumerateTable(stackRef, table);
            }
            break;
        }
    case LUA_DEBUGGER_CMD_RESET:
        {
            ret = reset();
            break;
        }
    case LUA_DEBUGGER_CMD_EVALUATE_EXPR:
        {
            int stackRef = 0;
            String expr;
            if (SocketHelper::readInt(&socket, stackRef) &&
                SocketHelper::readString(&socket, expr))
            {
                ret = evaluateExpr(stackRef, expr);
            }
            break;
        }
    }

    return ret;
}
// -----------------------------------------------------------------

String LuaDebuggee::toBreakPoint(const String &fileName, int lineNumber) const
{
    return StringUtil::format("%d:%s", lineNumber, fileName.c_str());
}

bool LuaDebuggee::hasBreakPoint(const String &fileName, int lineNumber)
{
    CriticalSectionLocker locker(breakPointsCS);
    return breakPoints.find(toBreakPoint(fileName, lineNumber)) != breakPoints.end();
}

bool LuaDebuggee::addBreakPoint(const String &fileName, int lineNumber)
{
    CriticalSectionLocker locker(breakPointsCS);
    breakPoints.insert(toBreakPoint(fileName, lineNumber));
    return true;
}

bool LuaDebuggee::removeBreakPoint(const String &fileName, int lineNumber)
{
    CriticalSectionLocker locker(breakPointsCS);
    return breakPoints.erase(toBreakPoint(fileName, lineNumber)) > 0;
}

bool LuaDebuggee::clearAllBreakPoints()
{
    CriticalSectionLocker locker(breakPointsCS);
    breakPoints.clear();
    return true;
}

bool LuaDebuggee::step()
{
    nextOperation = DEBUG_STEP;

    if (!running)
        runCondition.signal();
    else if (stopped)
        debugCondition.signal();

    return true;

}
bool LuaDebuggee::stepOver()
{
    framesUntilBreak = 0;
    nextOperation = DEBUG_STEPOVER;

    if (!running)
        runCondition.signal();
    else if (stopped)
        debugCondition.signal();

    return true;
}

bool LuaDebuggee::stepOut()
{
    framesUntilBreak = 0;
    nextOperation = DEBUG_STEPOUT;

    if (!running)
        runCondition.signal();
    else if (stopped)
        debugCondition.signal();

    return true;
}

bool LuaDebuggee::continueExec()
{
    nextOperation = DEBUG_GO;

    if (!running)
        runCondition.signal();
    else if (stopped)
        debugCondition.signal();

    return true;
}

bool LuaDebuggee::breakExec()
{
    forceBreak = true;
    return true;

}

bool LuaDebuggee::reset()
{
    notifyExit();

    forceBreak = true;
    resetRequested = true;

    if (!running)
        runCondition.signal();
    else if (stopped)
        debugCondition.signal();

    return true;
}

bool LuaDebuggee::enumerateStack()
{
    LuaDebugData debugData;

    enterLuaCriticalSection();
    debugData.enumerateStack(state);
    leaveLuaCriticalSection();

    return notifyStackEnumeration(debugData);
}

bool LuaDebuggee::enumerateStackEntry(int stackRef)
{
    LuaDebugData debugData;

    enterLuaCriticalSection();
    debugData.enumerateStackEntry(state, stackRef);
    leaveLuaCriticalSection();

    return notifyStackEntryEnumeration(stackRef, debugData);
}

bool LuaDebuggee::enumerateTable(int stackRef, const String& table)
{
    LuaDebugData debugData;

    enterLuaCriticalSection();
    debugData.enumerateTable(state, stackRef, table);
    leaveLuaCriticalSection();

    return notifyTableEnumeration(debugData, table);
}

bool LuaDebuggee::evaluateExpr(int stackRef, const String& expr)
{
    LuaDebugData debugData;

    enterLuaCriticalSection();
    debugData.evaluateExpr(state, stackRef, expr);
    leaveLuaCriticalSection();

    return notifyEvaluateExpr(debugData);
}

// ------------------------------------------------------------------------
bool LuaDebuggee::notifyBreak(const String &fileName, int lineNumber)
{
    return isConnected() && !resetRequested &&
        SocketHelper::writeCmd(&socket, LUA_DEBUGGEE_EVENT_BREAK) &&
        SocketHelper::writeString(&socket, fileName) &&
        SocketHelper::writeInt(&socket, lineNumber);
}

bool LuaDebuggee::notifyPrint(const String &errorMsg)
{
    return isConnected() &&
        SocketHelper::writeCmd(&socket, LUA_DEBUGGEE_EVENT_PRINT) &&
        SocketHelper::writeString(&socket, errorMsg);
}

bool LuaDebuggee::notifyError(const String &errorMsg)
{
    return isConnected() &&
        SocketHelper::writeCmd(&socket, LUA_DEBUGGEE_EVENT_ERROR) &&
        SocketHelper::writeString(&socket, errorMsg);
}

bool LuaDebuggee::notifyExit()
{
    return isConnected() &&
        SocketHelper::writeCmd(&socket, LUA_DEBUGGEE_EVENT_EXIT);
}

bool LuaDebuggee::notifyStackEnumeration(LuaDebugData& debugData)
{
    return isConnected() &&
        SocketHelper::writeCmd(&socket, LUA_DEBUGGEE_EVENT_STACK_ENUM) &&
        SocketHelper::writeObject(&socket, &debugData);
}

bool LuaDebuggee::notifyStackEntryEnumeration(int stackRef, LuaDebugData& debugData)
{
    return isConnected() &&
        SocketHelper::writeCmd(&socket, LUA_DEBUGGEE_EVENT_STACK_ENTRY_ENUM) &&
        SocketHelper::writeInt(&socket, stackRef) &&
        SocketHelper::writeObject(&socket, &debugData);
}

bool LuaDebuggee::notifyTableEnumeration(LuaDebugData& debugData, const String& table)
{
    return isConnected() &&
        SocketHelper::writeCmd(&socket, LUA_DEBUGGEE_EVENT_TABLE_ENUM) &&
        SocketHelper::writeString(&socket, table) &&
        SocketHelper::writeObject(&socket, &debugData);
}

bool LuaDebuggee::notifyEvaluateExpr(LuaDebugData& debugData)
{
    return isConnected() &&
        SocketHelper::writeCmd(&socket, LUA_DEBUGGEE_EVENT_EVALUATE_EXPR) &&
        SocketHelper::writeObject(&socket, &debugData);
}
