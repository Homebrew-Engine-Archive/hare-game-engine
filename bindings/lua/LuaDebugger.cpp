#include "LuaDebugger.h"
#include "SocketHelper.h"

#define MAX_SOCKET_CONNECT_TIMEOUT  200

LuaDebugger::LuaDebugger(lua_State* L, const String& server, int port)
: state(L), serverName(server), portNumber(port), connected(false), debugThread(0),
  debugCondition(debugMutex), forceBreak(false), resetRequested(false), running(false),
  stopped(false), framesUntilBreak(0), runCondition(runMutex)
{
    lua_sethook(L, LuaDebugger::luaDebugHook, LUA_MASKCALL | LUA_MASKLINE | LUA_MASKRET, 0);

    enterLuaCriticalSection();
}

LuaDebugger::~LuaDebugger()
{
    leaveLuaCriticalSection();
}

void LuaDebugger::luaDebugHook(lua_State *L, lua_Debug *debug)
{
    LuaDebugger* debugger = LuaDebugger::getSingletonPtr();
    if (debugger)
        debugger->debugHook(debug->event);
}

bool LuaDebugger::debugHook(int event)
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
                wait = true;
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
            if (framesUntilBreak > 0)
                framesUntilBreak--;
        }
        else if (event == LUA_HOOKLINE)
        {
            switch (nextOperation)
            {
            case DEBUG_STEP:
                {
                    if (notifyBreak(fileName, lineNumber))
                        wait = true;

                    break;
                }
            case DEBUG_STEPOVER:
                {
                    if ((framesUntilBreak == 0) && notifyBreak(fileName, lineNumber))
                        wait = true;

                    break;
                }
            case DEBUG_GO:
            default:
                {
                    if (hasBreakPoint(fileName, lineNumber) && notifyBreak(fileName, lineNumber))
                        wait = true;

                    break;
                }
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

bool LuaDebugger::start()
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
        return true;
    }

    return false;
}

void LuaDebugger::stop()
{
    // close socket and kill thread

    notifyExit();

    if (connected)
    {
        socket.shutdown(SD_BOTH);
        Sleep(100);
        socket.close();
    }

    if (debugThread)
        debugThread->waitExit();
}

bool LuaDebugger::isConnected(bool wait) const
{
    if (connected || !wait) return connected;

    for (int idx = 0; idx < MAX_SOCKET_CONNECT_TIMEOUT; ++idx)
    {
        if (connected)
            break;

        Sleep(100);
    }
    return connected;
}
// -----------------------------------------------------------------
//  Socket & thread
// -----------------------------------------------------------------
void* LuaDebugger::LuaThread::entry()
{
    debugger->threadFunction();

    return 0;
}

void LuaDebugger::threadFunction()
{
    bool threadRunning = false;

    if (socket.connect(serverName, portNumber))
    {
        connected = true;
        threadRunning = true;
    }
    else
    {
        return;
    }

    while (threadRunning)
    {
        unsigned char debugCommand = 0;
        if (!SocketHelper::readCmd(&socket, debugCommand) ||
            !handleDebuggerCmd(debugCommand))
        {
            threadRunning = false;
        }
    }
}

bool LuaDebugger::handleDebuggerCmd(int cmd)
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
    case LUA_DEBUGGER_CMD_ENUMERATE_TABLE_REF:
        {
            int tableRef = 0;
            int index = 0;
            long itemNode = 0;

            if (SocketHelper::readInt(&socket, tableRef) &&
                SocketHelper::readInt(&socket, index) &&
                SocketHelper::readLong(&socket, itemNode))
            {
                ret = enumerateTable(tableRef, index, itemNode);
            }
            break;
        }
    case LUA_DEBUGGER_CMD_RESET:
        {
            ret = reset();
            break;
        }
    }

    return ret;
}
// -----------------------------------------------------------------

String LuaDebugger::toBreakPoint(const String &fileName, int lineNumber) const
{
    return StringUtil::format("%d:%s", lineNumber, fileName.c_str());
}

bool LuaDebugger::hasBreakPoint(const String &fileName, int lineNumber)
{
    CriticalSectionLocker locker(breakPointsCS);
    return breakPoints.find(toBreakPoint(fileName, lineNumber)) != breakPoints.end();
}

bool LuaDebugger::addBreakPoint(const String &fileName, int lineNumber)
{
    CriticalSectionLocker locker(breakPointsCS);
    breakPoints.insert(toBreakPoint(fileName, lineNumber));
}

bool LuaDebugger::removeBreakPoint(const String &fileName, int lineNumber)
{
    CriticalSectionLocker locker(breakPointsCS);
    return breakPoints.erase(toBreakPoint(fileName, lineNumber)) > 0;    
}

bool LuaDebugger::clearAllBreakPoints()
{
    CriticalSectionLocker locker(breakPointsCS);
    breakPoints.clear();
    return true;
}

bool LuaDebugger::step()
{
    nextOperation = DEBUG_STEP;
    
    if (!running)
        runCondition.signal();
    else if (stopped)
        debugCondition.signal();

    return true;

}
bool LuaDebugger::stepOver()
{
    framesUntilBreak = 0;
    nextOperation = DEBUG_STEPOVER;

    if (!running)
        runCondition.signal();
    else if (stopped)
        debugCondition.signal();

    return true;
}

bool LuaDebugger::stepOut()
{
    framesUntilBreak = 1;
    nextOperation = DEBUG_STEPOVER;

    if (!running)
        runCondition.signal();
    else if (stopped)
        debugCondition.signal();

    return true;
}

bool LuaDebugger::continueExec()
{
    nextOperation = DEBUG_GO;

    if (!running)
        runCondition.signal();
    else if (stopped)
        debugCondition.signal();

    return true;
}

bool LuaDebugger::breakExec()
{
    forceBreak = true;
    return true;

}

bool LuaDebugger::reset()
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

bool LuaDebugger::enumerateStack()
{
    LuaDebugData debugData;

    enterLuaCriticalSection();
    debugData.enumerateStack(state);
    leaveLuaCriticalSection();

    return notifyStackEnumeration(debugData);
}

bool LuaDebugger::enumerateStackEntry(int stackRef)
{
    LuaDebugData debugData;

    enterLuaCriticalSection();
    debugData.enumerateStackEntry(state, stackRef, debugRefs);
    leaveLuaCriticalSection();

    return notifyStackEntryEnumeration(stackRef, debugData);
}

bool LuaDebugger::enumerateTable(int tableRef, int index, long itemNode)
{
    LuaDebugData debugData;

    enterLuaCriticalSection();
    debugData.enumerateTable(state, tableRef, index, debugRefs);
    leaveLuaCriticalSection();

    return notifyTableEnumeration(itemNode, debugData);
}

// ------------------------------------------------------------------------
bool LuaDebugger::notifyBreak(const String &fileName, int lineNumber)
{
    return isConnected() && !resetRequested &&
        SocketHelper::writeCmd(&socket, LUA_DEBUGGEE_EVENT_BREAK) &&
        SocketHelper::writeString(&socket, fileName) &&
        SocketHelper::writeInt(&socket, lineNumber);
}

bool LuaDebugger::notifyPrint(const String &errorMsg)
{
    return isConnected() && 
        SocketHelper::writeCmd(&socket, LUA_DEBUGGEE_EVENT_PRINT) &&
        SocketHelper::writeString(&socket, errorMsg);
}

bool LuaDebugger::notifyError(const String &errorMsg)
{
    return isConnected() && 
        SocketHelper::writeCmd(&socket, LUA_DEBUGGEE_EVENT_ERROR) &&
        SocketHelper::writeString(&socket, errorMsg);
}

bool LuaDebugger::notifyExit()
{
    return isConnected() &&
        SocketHelper::writeCmd(&socket, LUA_DEBUGGEE_EVENT_EXIT);
}

bool LuaDebugger::notifyStackEnumeration(LuaDebugData& debugData)
{
    return isConnected() &&
        SocketHelper::writeCmd(&socket, LUA_DEBUGGEE_EVENT_STACK_ENUM) &&
        SocketHelper::writeObject(&socket, &debugData);
}

bool LuaDebugger::notifyStackEntryEnumeration(int stackRef, LuaDebugData& debugData)
{
    return isConnected() &&
        SocketHelper::writeCmd(&socket, LUA_DEBUGGEE_EVENT_STACK_ENTRY_ENUM) &&
        SocketHelper::writeInt(&socket, stackRef) &&
        SocketHelper::writeObject(&socket, &debugData);
}

bool LuaDebugger::notifyTableEnumeration(long itemNode, LuaDebugData& debugData)
{
    return isConnected() &&
        SocketHelper::writeCmd(&socket, LUA_DEBUGGEE_EVENT_TABLE_ENUM) &&
        SocketHelper::writeLong(&socket, itemNode) &&
        SocketHelper::writeObject(&socket, &debugData);
}