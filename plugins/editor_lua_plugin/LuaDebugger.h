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
#ifndef _LUADEBUGGER_H_
#define _LUADEBUGGER_H_

#include <wx/process.h>
#include "CallStackWindow.h"
#include "WatchWindow.h"

class LuaDebuggerProcess;
class LuaDebuggerEvent;

class LuaDebuggerConfig : public Object
{
    HARE_DECLARE_DYNAMIC_CLASS(LuaDebuggerConfig)
private:
    bool remoteDebug;
    String hostName;
    int portNumber;
    String appPath;
    String layout;

public:
    LuaDebuggerConfig();

    wxString getLayout() const;
    void setLayout(const wxString& val);

    bool isRemoteDebug() const;
    void setRemoteDebug(bool val);

    wxString getHostName() const;
    void setHostName(const wxString& val);

    int getPortNumber() const;
    void setPortNumber(int val);

    wxString getAppPath() const;
    void setAppPath(const wxString& val);
};

enum LuaDebuggerState
{
    Debugger_Stoped,
    Debugger_Breaked,
    Debugger_Wait,
    Debugger_Running,
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

public:
    virtual bool start();
    virtual bool addBreakPoint(const String &fileName, int lineNumber);
    virtual bool removeBreakPoint(const String &fileName, int lineNumber);

protected:
    virtual void onAttach();
    virtual void onDetach(bool isShutDown);

    void switchToDebugLayout();
    void restoreLayout();

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

    bool step();
    bool stepOver();
    bool stepOut();
    bool continueExec();
    bool breakExec();
    bool reset();
    bool clearAllBreakPoints();

    bool enumerateStack();
    bool enumerateStackEntry(int stackEntry);
    bool enumerateTable(int tableRef, int index, long itemNode);
    bool evaluateExpr(int stackRef, const String &strExpression);

    virtual wxString getSocketErrorMsg();

    void threadFunction();

    int handleDebuggeeEvent(int event_type);

    void OnEndDebugeeProcess(wxProcessEvent& event);
    
    Socket* serverSocket;
    Socket* acceptedSocket;
    LuaThread* thread;
    bool shutdown;

    wxCriticalSection acceptSockCS; // for deleting accepted socket
    wxCriticalSection processCS;    // for deleting the client process

    LuaDebuggerProcess* debuggeeProcess;   // wxProcess of the debuggee
    long debuggeePID;

    LuaCallStackWindow* callStackWindow;
    LuaWatchWindow* watchWindow;
    LuaDebuggerConfig::Ptr config;
    wxString previousLayout;

    int currStackLevel;

    enum StartCommand
    {
        StartCmd_Go,
        StartCmd_StepIn,
        StartCmd_StepOver,
    };

    LuaDebuggerState state;
    StartCommand startCmd;

private:
    bool checkSocketConnected(bool sendEvent, const wxString& msg);
    bool checkSocketRead(bool isOk, const wxString& msg);
    bool checkSocketWrite(bool isOk, const wxString& msg);

public:
    void syncEditor(const String& fileName, int lineNumber);
    void clearEditorDebugMark();

    void setCurrStackLevel(int stackLevel);
    int getCurrStackLevel();

private:
    void onMenuUpdateUI(wxUpdateUIEvent& event);

    void onLuaDebugStart(wxCommandEvent& event);
    void onLuaDebugPause(wxCommandEvent& event);
    void onLuaDebugStop(wxCommandEvent& event);
    void onLuaDebugStepIn(wxCommandEvent& event);
    void onLuaDebugStepOver(wxCommandEvent& event);
    void onLuaDebugStepOut(wxCommandEvent& event);

    void onLuaDebugDebuggeeConnected(LuaDebuggerEvent& event);
    void onLuaDebugDebuggeeDisconnected(LuaDebuggerEvent& event);
    void onLuaDebugBreak(LuaDebuggerEvent& event);
    void onLuaDebugStackEnum(LuaDebuggerEvent& event);
    void onLuaDebugEvaluateExpr(LuaDebuggerEvent& event);
    void onLuaDebugExit(LuaDebuggerEvent& event);
    
    void onAppBeforeShutdown(EditorEvent& event);

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

#define wxLuaDebuggerEventHandler(func) \
    (wxObjectEventFunction)(wxEventFunction)wxStaticCastEvent(wxLuaDebuggerEventFunction, &func)

#define EVT_LUA_DEBUGGER_DEBUGGEE_CONNECTED(id, fn) \
    DECLARE_EVENT_TABLE_ENTRY(wxEVT_LUA_DEBUGGER_DEBUGGEE_CONNECTED, id, -1, wxLuaDebuggerEventHandler(fn), (wxObject*)NULL),
#define EVT_LUA_DEBUGGER_DEBUGGEE_DISCONNECTED(id, fn) \
    DECLARE_EVENT_TABLE_ENTRY(wxEVT_LUA_DEBUGGER_DEBUGGEE_DISCONNECTED, id, -1, wxLuaDebuggerEventHandler(fn), (wxObject*)NULL),
#define EVT_LUA_DEBUGGER_BREAK(id, fn) \
    DECLARE_EVENT_TABLE_ENTRY(wxEVT_LUA_DEBUGGER_BREAK, id, -1, wxLuaDebuggerEventHandler(fn), (wxObject*)NULL),
#define EVT_LUA_DEBUGGER_PRINT(id, fn) \
    DECLARE_EVENT_TABLE_ENTRY(wxEVT_LUA_DEBUGGER_PRINT, id, -1, wxLuaDebuggerEventHandler(fn), (wxObject*)NULL),
#define EVT_LUA_DEBUGGER_ERROR(id, fn) \
    DECLARE_EVENT_TABLE_ENTRY(wxEVT_LUA_DEBUGGER_ERROR, id, -1, wxLuaDebuggerEventHandler(fn), (wxObject*)NULL),
#define EVT_LUA_DEBUGGER_EXIT(id, fn) \
    DECLARE_EVENT_TABLE_ENTRY(wxEVT_LUA_DEBUGGER_EXIT, id, -1, wxLuaDebuggerEventHandler(fn), (wxObject*)NULL),
#define EVT_LUA_DEBUGGER_STACK_ENUM(id, fn) \
    DECLARE_EVENT_TABLE_ENTRY(wxEVT_LUA_DEBUGGER_STACK_ENUM, id, -1, wxLuaDebuggerEventHandler(fn), (wxObject*)NULL),
#define EVT_LUA_DEBUGGER_STACK_ENTRY_ENUM(id, fn) \
    DECLARE_EVENT_TABLE_ENTRY(wxEVT_LUA_DEBUGGER_STACK_ENTRY_ENUM, id, -1, wxLuaDebuggerEventHandler(fn), (wxObject*)NULL),
#define EVT_LUA_DEBUGGER_TABLE_ENUM(id, fn) \
    DECLARE_EVENT_TABLE_ENTRY(wxEVT_LUA_DEBUGGER_TABLE_ENUM, id, -1, wxLuaDebuggerEventHandler(fn), (wxObject*)NULL),
#define EVT_LUA_DEBUGGER_EVALUATE_EXPR(id, fn) \
    DECLARE_EVENT_TABLE_ENTRY(wxEVT_LUA_DEBUGGER_EVALUATE_EXPR, id, -1, wxLuaDebuggerEventHandler(fn), (wxObject*)NULL),

#endif