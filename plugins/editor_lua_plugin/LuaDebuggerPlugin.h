//***************************************************************
//  File:    LuaDebugger.h
//  Data:    01/03/2009
//  Author:  littlesome (littlesome@live.cn)
//-------------------------------------------------------------
//  
//-------------------------------------------------------------
//  This file is part of Hare2D Game Engine.
//  Copyright (C) All Rights Reserved
//***************************************************************
// 
//***************************************************************
#ifndef _LUADEBUGGERPLUGIN_H_
#define _LUADEBUGGERPLUGIN_H_

#include <wx/process.h>

class LuaDebuggerProcess;

enum
{
    ID_LUA_DEBUGGEE_PROCESS = 1500 // id of the spawned debuggee wxProcess
};

class LuaDebugger : public DebuggerPlugin
{
public:
    virtual bool buildMenuBar(wxMenuBar* menuBar);
    virtual bool buildToolBar(wxAuiToolBar* toolBar);

protected:
    // ----------------------------------------------------------------------------
    // LuaThread - a wxThread for the LuaDebugger
    // ----------------------------------------------------------------------------
    class LuaThread : public wxThread
    {
    public:
        LuaThread(LuaDebugger *dbg) : wxThread(wxTHREAD_JOINABLE),
            debugger(dbg) {}

        virtual void* Entry()
        {
            debugger->threadFunction();
            return 0;
        }

        LuaDebugger *debugger;
    };

public:
    LuaDebugger();
    virtual ~LuaDebugger();

    // Start the debugger server to listen for a debuggee. After creation
    //  you must call startServer to actually start the server. returns success
    virtual bool startDebugger();
    // Stop the debugger server, returns success
    virtual bool stopDebugger();
    // Start a debuggee client to be debugged by this, returns process ID
    //   is > 0 on success.
    virtual long startDebuggee();

    bool killDebuggee();

    virtual Socket* getSocket() 
    { 
        return acceptedSocket; 
    }

    static void setDebugCmd(const wxString& cmd) 
    { 
        debugCmd = cmd; 
    }
    
    static wxString getDebugCmd() 
    { 
        return debugCmd; 
    }

    static void setHostName(const wxString& name) 
    { 
        hostName = name; 
    }
    static wxString getHostName()
    { 
        return hostName; 
    }

    bool step();
    bool stepOver();
    bool stepOut();
    bool continueExec();
    bool breakExec();
    bool reset();
    bool enumerateStack();
    bool enumerateStackEntry(int stackEntry);
    bool enumerateTable(int tableRef, int index, long itemNode);
    bool clearDebugReferences();
    bool evaluateExpr(int exprRef, const String &strExpression);

    virtual wxString getSocketErrorMsg();

    void threadFunction();

    int handleDebuggeeEvent(int event_type);

    void OnEndDebugeeProcess(wxProcessEvent& event);
    
    int portNumber;  
    Socket* serverSocket;
    Socket* acceptedSocket;
    LuaThread* thread;
    bool shutdown;

    wxCriticalSection acceptSockCS; // for deleting accepted socket
    wxCriticalSection processCS;    // for deleting the client process

    static wxString debugCmd;
    static wxString hostName;

    LuaDebuggerProcess* debuggeeProcess;   // wxProcess of the debuggee
    long debuggeePID;

private:
    bool checkSocketConnected(bool sendEvent, const wxString& msg);
    bool checkSocketRead(bool isOk, const wxString& msg);
    bool checkSocketWrite(bool isOk, const wxString& msg);
};

class LuaDebuggerProcess : public wxProcess
{
public:
    // Don't use the debugger as the event handler since we don't want
    //   problems when this may exist when the debugger is being deleted.
    LuaDebuggerProcess(LuaDebugger* dbg, wxWindowID id)
        : wxProcess(NULL, id), debugger(dbg) {}

    // don't send event, but delete this and NULL debugger's pointer to this
    virtual void OnTerminate(int pid, int status);

    LuaDebugger* debugger;
};

#endif