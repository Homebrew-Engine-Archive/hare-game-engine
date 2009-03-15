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
#include "CallStackWindow.h"
#include "WatchWindow.h"

class LuaDebuggerProcess;
class LuaDebuggerEvent;

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

public:
    virtual bool start();

protected:
    virtual void onAttach();
    virtual void onDetach(bool isShutDown);

public:
    // Start the debugger server to listen for a debuggee. After creation
    //  you must call startServer to actually start the server. returns success
    bool startDebugger();
    // Stop the debugger server, returns success
    bool stopDebugger();
    // Start a debuggee client to be debugged by this, returns process ID
    //   is > 0 on success.
    virtual long startDebuggee();

    bool killDebuggee();

    Socket* getSocket() 
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

    LuaCallStackWindow* callStackWindow;
    LuaWatchWindow* watchWindow;

private:
    bool checkSocketConnected(bool sendEvent, const wxString& msg);
    bool checkSocketRead(bool isOk, const wxString& msg);
    bool checkSocketWrite(bool isOk, const wxString& msg);

public:
    void syncEditor(const String& fileName, int lineNumber);

private:
    void onMenuUpdateUI(wxUpdateUIEvent& event);

    void onLuaDebugStart(wxCommandEvent& event);
    void onLuaDebugPause(wxCommandEvent& event);
    void onLuaDebugStop(wxCommandEvent& event);
    void onLuaDebugStepIn(wxCommandEvent& event);
    void onLuaDebugStepOver(wxCommandEvent& event);
    void onLuaDebugStepOut(wxCommandEvent& event);

    void onLuaDebugBreak(LuaDebuggerEvent& event);
    void onLuaDebugStackEnum(LuaDebuggerEvent& event);
    
private:
    DECLARE_EVENT_TABLE()
};

class LuaDebuggerProcess : public wxProcess
{
public:
    // Don't use the debugger as the event handler since we don't want
    //   problems when this may exist when the debugger is being deleted.
    LuaDebuggerProcess(LuaDebugger* dbg)
        : wxProcess(NULL), debugger(dbg) {}

    // don't send event, but delete this and NULL debugger's pointer to this
    virtual void OnTerminate(int pid, int status);

    LuaDebugger* debugger;
};

class LuaDebuggerEvent : public wxEvent
{
public:
    LuaDebuggerEvent(const LuaDebuggerEvent& event);
    LuaDebuggerEvent(wxEventType eventType = wxEVT_NULL,
        wxObject* eventObject = NULL,
        int line = 0,
        const String &file = StringUtil::EMPTY);

protected:
    virtual wxEvent* Clone() const { return new LuaDebuggerEvent(*this); }

public:
    int lineNumber;
    String fileName;
    String strMessage;
    LuaDebugData::Ptr debugData;

private:
    DECLARE_DYNAMIC_CLASS(LuaDebuggerEvent)
};

typedef void (wxEvtHandler::*wxLuaDebuggerEventFunction)(LuaDebuggerEvent&);

#endif