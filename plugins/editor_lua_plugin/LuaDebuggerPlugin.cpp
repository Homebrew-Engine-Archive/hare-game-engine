//***************************************************************
//  File:    LuaDebugger.cpp
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
#include "PCH.h"
#include "LuaDebuggerPlugin.h"
#include "../../bindings/lua/LuaDebugDefines.h"
#include "../../bindings/lua/SocketHelper.h"
#include <wx/xrc/xmlres.h>
#include <wx/aui/aui.h>

int idLuaDebugStart = XRCID("idLuaDebugStart");
int idLuaDebugPause = XRCID("idLuaDebugPause");
int idLuaDebugStop = XRCID("idLuaDebugStop");
int idLuaDebugStepIn = XRCID("idLuaDebugStepIn");
int idLuaDebugStepOver = XRCID("idLuaDebugStepOver");
int idLuaDebugStepOut = XRCID("idLuaDebugStepOut");

// ------------------------------------------------------------------------
//   LuaDebuggerProcess
// ------------------------------------------------------------------------
void LuaDebuggerProcess::OnTerminate(int pid, int status)
{
    // If this is being deleted from the destructor of LuaDebugger
    //   it has already been NULLed so don't send event.
    if (debugger && debugger->debuggeeProcess)
    {
        // we don't use the event handler, but this is good enough.
        wxProcessEvent event(m_id, pid, status);
        debugger->OnEndDebugeeProcess(event);

        debugger->debuggeeProcess = NULL;
        debugger->debuggeePID = -1;
    }

    delete this;
}
// ------------------------------------------------------------------------
//   LuaDebugger
// ------------------------------------------------------------------------
wxString LuaDebugger::debugCmd;
wxString LuaDebugger::hostName;

LuaDebugger::LuaDebugger()
 : portNumber(9981), debuggeeProcess(NULL), debuggeePID(-1)
{
    if (hostName.IsEmpty())
    {
        hostName = wxT("localhost");
    }
}

LuaDebugger::~LuaDebugger()
{
    // we don't delete the the process, we kill it and its OnTerminate deletes it
    if ((debuggeeProcess != NULL) && (debuggeePID > 0) &&
        wxProcess::Exists(debuggeePID))
    {
        debuggeeProcess->debugger = NULL;
        debuggeeProcess = NULL;
        wxProcess::Kill(debuggeePID, wxSIGKILL, wxKILL_CHILDREN);
    }
}

bool LuaDebugger::buildMenuBar(wxMenuBar* menuBar)
{
    wxMenu* menu = wxXmlResource::Get()->LoadMenu(wxT("lua_debugger_menu"));
    menuBar->Insert(3, menu, _("Lua&Debug"));

    return true;
}

bool LuaDebugger::buildToolBar(wxAuiToolBar* toolBar)
{
    wxString fullPath = Manager::getInstancePtr()->getAppDir() + wxT("/editor_data/resources/");
    wxBitmap bmp;

    bmp.LoadFile(fullPath + wxT("start.png"), wxBITMAP_TYPE_PNG);
    toolBar->AddTool(idLuaDebugStart, _("Start"), bmp, wxT("Start"));
    bmp.LoadFile(fullPath + wxT("pause.png"), wxBITMAP_TYPE_PNG);
    toolBar->AddTool(idLuaDebugPause, _("Pause"), bmp, wxT("Pause"));
    bmp.LoadFile(fullPath + wxT("stop.png"), wxBITMAP_TYPE_PNG);
    toolBar->AddTool(idLuaDebugStop, _("Stop"), bmp, wxT("Stop"));
    toolBar->AddSeparator();
    bmp.LoadFile(fullPath + wxT("step_in.png"), wxBITMAP_TYPE_PNG);
    toolBar->AddTool(idLuaDebugStepIn, _("StepIn"), bmp, wxT("StepIn"));
    bmp.LoadFile(fullPath + wxT("step_over.png"), wxBITMAP_TYPE_PNG);
    toolBar->AddTool(idLuaDebugStepOver, _("StepOver"), bmp, wxT("StepOver"));
    bmp.LoadFile(fullPath + wxT("step_out.png"), wxBITMAP_TYPE_PNG);
    toolBar->AddTool(idLuaDebugStepOut, _("StepOut"), bmp, wxT("StepOut"));
    return true;
}

void LuaDebugger::OnEndDebugeeProcess(wxProcessEvent& event)
{
    // The process's OnTerminate will null debuggeeProcess,
    // but if in destructor it's already NULL and don't send event.
    if (debuggeeProcess != NULL)
    {
        //wxLuaDebuggerEvent debugEvent(wxEVT_WXLUA_DEBUGGER_EXIT, this);
        //debugEvent.SetMessage(wxString::Format(wxT("Process (%d) ended with exit code : %d"), event.GetPid(), event.GetExitCode()));
        //wxPostEvent(this, debugEvent);
    }

    event.Skip();
}

bool LuaDebugger::step()
{
   return checkSocketConnected(true, wxT("Debugger Step")) && checkSocketWrite(
        SocketHelper::writeCmd(getSocket(), LUA_DEBUGGER_CMD_DEBUG_STEP),
        wxT("Debugger Step"));
}

bool LuaDebugger::stepOver()
{
    return checkSocketConnected(true, wxT("Debugger StepOver")) && checkSocketWrite(
        SocketHelper::writeCmd(getSocket(), LUA_DEBUGGER_CMD_DEBUG_STEPOVER),
        wxT("Debugger StepOver"));
}

bool LuaDebugger::stepOut()
{
    return checkSocketConnected(true, wxT("Debugger StepOut")) && checkSocketWrite(
        SocketHelper::writeCmd(getSocket(), LUA_DEBUGGER_CMD_DEBUG_STEPOUT),
        wxT("Debugger StepOut"));
}

bool LuaDebugger::continueExec()
{
    return checkSocketConnected(true, wxT("Debugger Continue")) && checkSocketWrite(
        SocketHelper::writeCmd(getSocket(), LUA_DEBUGGER_CMD_DEBUG_CONTINUE),
        wxT("Debugger Continue"));
}

bool LuaDebugger::breakExec()
{
    return checkSocketConnected(true, wxT("Debugger Break")) && checkSocketWrite(
        SocketHelper::writeCmd(getSocket(), LUA_DEBUGGER_CMD_DEBUG_BREAK),
        wxT("Debugger Break"));
}

bool LuaDebugger::reset()
{
    return checkSocketConnected(true, wxT("Debugger Reset")) && checkSocketWrite(
        SocketHelper::writeCmd(getSocket(), LUA_DEBUGGER_CMD_RESET),
        wxT("Debugger Reset"));
}

bool LuaDebugger::enumerateStack()
{
    return checkSocketConnected(true, wxT("Debugger EnumerateStack")) && checkSocketWrite(
        SocketHelper::writeCmd(getSocket(), LUA_DEBUGGER_CMD_ENUMERATE_STACK),
        wxT("Debugger EnumerateStack"));
}

bool LuaDebugger::enumerateStackEntry(int stackEntry)
{
    return checkSocketConnected(true, wxT("Debugger EnumerateStackEntry")) && checkSocketWrite(
        SocketHelper::writeCmd(getSocket(), LUA_DEBUGGER_CMD_ENUMERATE_STACK_ENTRY) &&
        SocketHelper::writeInt(getSocket(), stackEntry),
        wxT("Debugger EnumerateStackEntry"));
}

bool LuaDebugger::enumerateTable(int tableRef, int index, long itemNode)
{
    return checkSocketConnected(true, wxT("Debugger EnumerateTable")) && checkSocketWrite(
        SocketHelper::writeCmd(getSocket(), LUA_DEBUGGER_CMD_ENUMERATE_TABLE_REF) &&
        SocketHelper::writeInt(getSocket(), tableRef) &&
        SocketHelper::writeInt(getSocket(), index) &&
        SocketHelper::writeLong(getSocket(), itemNode),
        wxT("Debugger EnumerateTable"));
}

bool LuaDebugger::clearDebugReferences()
{
    return checkSocketConnected(true, wxT("Debugger ClearDebugReferences")) && checkSocketWrite(
        SocketHelper::writeCmd(getSocket(), LUA_DEBUGGER_CMD_CLEAR_DEBUG_REFERENCES),
        wxT("Debugger ClearDebugReferences"));
}

bool LuaDebugger::evaluateExpr(int exprRef, const String &strExpression)
{
    return checkSocketConnected(true, wxT("Debugger EvaluateExpr")) && checkSocketWrite(
        SocketHelper::writeCmd(getSocket(), LUA_DEBUGGER_CMD_EVALUATE_EXPR) &&
        SocketHelper::writeInt(getSocket(), exprRef) &&
        SocketHelper::writeString(getSocket(), strExpression),
        wxT("Debugger EvaluateExpr"));
}

long LuaDebugger::startDebuggee()
{
    if (debuggeeProcess == NULL)
    {
        debuggeeProcess = new LuaDebuggerProcess(this, ID_LUA_DEBUGGEE_PROCESS);
        wxString command = wxString::Format(wxT("%s -debug %s:%u"),
            getDebugCmd().c_str(),
            getHostName().c_str(),
            portNumber);

        debuggeePID = wxExecute(command, wxEXEC_ASYNC | wxEXEC_MAKE_GROUP_LEADER, debuggeeProcess);

        if (debuggeePID < 1)
            killDebuggee();
    }

    return debuggeePID;
}

bool LuaDebugger::startDebugger()
{
    wxCHECK_MSG(serverSocket == NULL, false, wxT("Debugger server socket already created"));

    shutdown = false;
    serverSocket = new Socket();

    if (serverSocket->listen(portNumber))
    {
        wxCHECK_MSG(thread == NULL, false, wxT("Debugger server thread already created"));

        if (!shutdown)
        {
            thread = new LuaThread(this);

            return ((thread != NULL) && (thread->Create() == wxTHREAD_NO_ERROR) &&
                (thread->Run() == wxTHREAD_NO_ERROR));
        }
    }
    else
    {
        //wxLuaDebuggerEvent debugEvent(wxEVT_WXLUA_DEBUGGER_ERROR, this);
        //debugEvent.SetMessage(m_serverSocket->GetErrorMsg(true));
        //AddPendingEvent(debugEvent);

        delete serverSocket;
        serverSocket = NULL;
        shutdown = true;
    }

    return false;
}

bool LuaDebugger::stopDebugger()
{
    shutdown = true;

    // try to nicely stop the socket if it exists
    if (acceptedSocket)
    {
        reset();
        wxMilliSleep(500);
    }

    Socket *accSocket = acceptedSocket;

    if (accSocket != NULL)
    {
        if (!accSocket->shutdown(SD_BOTH))
        {
            //wxLuaDebuggerEvent debugEvent(wxEVT_WXLUA_DEBUGGER_ERROR, this);
            //debugEvent.SetMessage(acceptedSocket->GetErrorMsg(true));
            //AddPendingEvent(debugEvent);
        }

        wxMilliSleep(500);
    }

    // close the server socket, if accepted socket created it will already
    // have been deleted
    if (serverSocket != NULL)
    {
        Socket *svrSocket = serverSocket;
        serverSocket = NULL;

        // close the server socket by connecting to the socket, thus
        // completing the 'accept'. If a client has not connected, this
        // code will satisfy the accept the m_shutdown flag will be set
        // so the thread will not loop and instead will just destroy the
        // session socket object and return.
        Socket closeSocket;

        if (!closeSocket.connect(getHostName().ToUTF8().data(), portNumber) ||
            !closeSocket.shutdown(SD_BOTH))
        {
            //wxLuaDebuggerEvent debugEvent(wxEVT_WXLUA_DEBUGGER_ERROR, this);
            //debugEvent.SetMessage(serverSocket->GetErrorMsg(true));
            //AddPendingEvent(debugEvent);
        }

        wxMilliSleep(100);

        delete svrSocket;
    }

    // One of the above two operations terminates the thread. Wait for it to stop.
    if ((thread != NULL) && thread->IsRunning())
        thread->Wait();

    delete thread;
    thread = NULL;

    return true;
}

bool LuaDebugger::killDebuggee()
{
    if ((debuggeeProcess != NULL) && (debuggeePID > 0))
    {
        debuggeeProcess->debugger = NULL;
        debuggeeProcess = NULL;

        wxProcess::Kill(debuggeePID, wxSIGKILL, wxKILL_CHILDREN);
    }
    else if (debuggeeProcess != NULL) // error starting process?
    {
        LuaDebuggerProcess* p = debuggeeProcess;
        debuggeeProcess->debugger = NULL;
        debuggeeProcess = NULL;
        delete p;
    }

    debuggeePID = -1;

    return true;
}

void LuaDebugger::threadFunction()
{
    wxCHECK_RET(serverSocket, wxT("Invalid server socket"));
    wxCHECK_RET(acceptedSocket == NULL, wxT("The debugger server has already accepted a socket connection"));

    acceptedSocket = serverSocket->accept();
    if (!acceptedSocket)
    {
        //wxLuaDebuggerEvent debugEvent(wxEVT_WXLUA_DEBUGGER_ERROR, this);
        //debugEvent.SetMessage(m_serverSocket->GetErrorMsg(true));
        //AddPendingEvent(debugEvent);
    }
    else
    {
        Socket *svrSocket = serverSocket;
        serverSocket = NULL;
        delete svrSocket;

        wxThread::Sleep(500);

        // Notify that a client has connected and we are ready to debug
        //wxLuaDebuggerEvent debugEvent(wxEVT_WXLUA_DEBUGGER_DEBUGGEE_CONNECTED, this);
        //AddPendingEvent(debugEvent);

        unsigned char debug_event = 0; // wxLuaSocketDebuggeeEvents_Type

        // Enter the debug loop
        while (!thread->TestDestroy() && !shutdown && acceptedSocket)
        {
            debug_event = LUA_DEBUGGEE_EVENT_EXIT;

            {
                // lock the critical section while we access it
                wxCriticalSectionLocker locker(acceptSockCS);
                if (shutdown || (acceptedSocket == NULL) || !SocketHelper::readCmd(acceptedSocket, debug_event))
                {
                    shutdown = true;
                    break;
                }
            }

            if((debug_event == LUA_DEBUGGEE_EVENT_EXIT) ||
                (handleDebuggeeEvent(debug_event) != -1))
            {
                // don't send exit event until we've closed the socket
                if (debug_event == LUA_DEBUGGEE_EVENT_EXIT)
                {
                    shutdown = true;
                    break;
                }
            }
        }

        wxCriticalSectionLocker locker(acceptSockCS);
        // delete the accepted socket
        if (acceptedSocket != NULL)
        {
            Socket *accSocket = acceptedSocket;
            acceptedSocket = NULL;
            delete accSocket;
        }
    }

    // Send the exit event, now that everything is shut down
    //if (debug_event == LUA_DEBUGGEE_EVENT_EXIT)
    {
        //wxLuaDebuggerEvent debugEvent(wxEVT_WXLUA_DEBUGGER_EXIT, this);
        //wxPostEvent(this, debugEvent);
    }
}

int LuaDebugger::handleDebuggeeEvent(int event_type)
{
    wxCHECK_MSG(getSocket(), event_type, wxT("Invalid socket"));

    switch (event_type)
    {
    case LUA_DEBUGGEE_EVENT_BREAK:
        {
            String fileName;
            int lineNumber = 0;

            if (checkSocketRead(
                SocketHelper::readString(getSocket(), fileName) &&
                SocketHelper::readInt(getSocket(), lineNumber),
                wxT("Debugger LUA_DEBUGGEE_EVENT_BREAK")))
            {
                //wxLuaDebuggerEvent debugEvent(wxEVT_WXLUA_DEBUGGER_BREAK, this, lineNumber, fileName);
                //SendEvent(debugEvent);
            }
            else 
                return -1;

            break;
        }
    case LUA_DEBUGGEE_EVENT_PRINT:
        {
            String strMessage;

            if (checkSocketRead(
                SocketHelper::readString(getSocket(), strMessage),
                wxT("Debugger LUADEBUGGEE_EVENT_PRINT")))
            {
                //wxLuaDebuggerEvent debugEvent(wxEVT_WXLUA_DEBUGGER_PRINT, this);
                //debugEvent.SetMessage(strMessage);
                //SendEvent(debugEvent);
            }
            else return -1;

            break;
        }
    case LUA_DEBUGGEE_EVENT_ERROR:
        {
            String strMessage;

            if (checkSocketRead(
                SocketHelper::readString(getSocket(), strMessage),
                wxT("Debugger LUA_DEBUGGEE_EVENT_ERROR")))
            {
                //wxLuaDebuggerEvent debugEvent(wxEVT_WXLUA_DEBUGGER_ERROR, this);
                //debugEvent.SetMessage(strMessage);
                //SendEvent(debugEvent);
            }
            else return -1;

            break;
        }
    case LUA_DEBUGGEE_EVENT_EXIT:
        {
            //wxLuaDebuggerEvent debugEvent(wxEVT_WXLUA_DEBUGGER_EXIT, this);
            //wxPostEvent(this, debugEvent);
            break;
        }
    case LUA_DEBUGGEE_EVENT_STACK_ENUM:
        {
            /*wxLuaDebugData debugData(true);

            if (CheckSocketRead(
                GetSocketBase()->ReadDebugData(debugData),
                wxT("Debugger LUA_DEBUGGEE_EVENT_STACK_ENUM")))
            {
                wxLuaDebuggerEvent debugEvent(wxEVT_WXLUA_DEBUGGER_STACK_ENUM, this);
                debugEvent.SetDebugData(-1, debugData);
                SendEvent(debugEvent);
            }
            else return -1;*/

            break;
        }
    case LUA_DEBUGGEE_EVENT_STACK_ENTRY_ENUM:
        {
            /*wxInt32 stackRef = 0;
            wxLuaDebugData debugData(true);

            if (CheckSocketRead(
                GetSocketBase()->ReadInt32(stackRef) &&
                GetSocketBase()->ReadDebugData(debugData),
                wxT("Debugger LUA_DEBUGGEE_EVENT_STACK_ENTRY_ENUM")))
            {
                wxLuaDebuggerEvent debugEvent(wxEVT_WXLUA_DEBUGGER_STACK_ENTRY_ENUM, this);
                debugEvent.SetDebugData(stackRef, debugData);
                SendEvent(debugEvent);
            }
            else return -1;*/

            break;
        }
    case LUA_DEBUGGEE_EVENT_TABLE_ENUM:
        {
            /*long itemNode = 0;
            wxLuaDebugData debugData(true);

            if (CheckSocketRead(
                GetSocketBase()->ReadLong(itemNode) &&
                GetSocketBase()->ReadDebugData(debugData),
                wxT("Debugger LUA_DEBUGGEE_EVENT_TABLE_ENUM")))
            {
                wxLuaDebuggerEvent debugEvent(wxEVT_WXLUA_DEBUGGER_TABLE_ENUM, this);
                debugEvent.SetDebugData(itemNode, debugData);
                SendEvent(debugEvent);
            }
            else return -1;*/

            break;
        }
    case LUA_DEBUGGEE_EVENT_EVALUATE_EXPR:
        {
            int exprRef = 0;
            String strResult;

            if (checkSocketRead(
                SocketHelper::readInt(getSocket(), exprRef) &&
                SocketHelper::readString(getSocket(), strResult),
                wxT("Debugger LUA_DEBUGGEE_EVENT_EVALUATE_EXPR")))
            {
                //wxLuaDebuggerEvent debugEvent(wxEVT_WXLUA_DEBUGGER_EVALUATE_EXPR, this);
                //debugEvent.SetMessage(strResult);
                //debugEvent.SetDebugData(exprRef);
                //SendEvent(debugEvent);
            }
            else 
                return -1;

            break;
        }
    default : 
        return -1; // don't know this event?
    }

    return event_type;
}


//bool LuaDebugger::AddBreakPoint(const wxString &fileName, int lineNumber)
//{
//    return CheckSocketConnected(true, wxT("Debugger AddBreakPoint")) && CheckSocketWrite(
//        GetSocketBase()->WriteCmd(LUA_DEBUGGER_CMD_ADD_BREAKPOINT) &&
//        GetSocketBase()->WriteString(fileName) &&
//        GetSocketBase()->WriteInt32(lineNumber),
//        wxT("Debugger AddBreakPoint"));
//}
//
//bool wxLuaDebuggerBase::RemoveBreakPoint(const wxString &fileName, int lineNumber)
//{
//    return CheckSocketConnected(true, wxT("Debugger RemoveBreakPoint")) && CheckSocketWrite(
//        GetSocketBase()->WriteCmd(LUA_DEBUGGER_CMD_REMOVE_BREAKPOINT) &&
//        GetSocketBase()->WriteString(fileName) &&
//        GetSocketBase()->WriteInt32(lineNumber),
//        wxT("Debugger RemoveBreakPoint"));
//}


bool LuaDebugger::checkSocketConnected(bool sendEvent, const wxString& msg)
{
    if (getSocket() == NULL)
    {
        if (sendEvent)
        {
            //wxLuaDebuggerEvent debugEvent(wxEVT_WXLUA_DEBUGGER_DEBUGGEE_DISCONNECTED, this);
            //debugEvent.SetMessage(wxT("Debugger socket not created. ") + msg);
            //SendEvent(debugEvent);
        }

        return false;
    }
    else if (!getSocket()->isConnected())
    {
        if (sendEvent)
        {
            //wxLuaDebuggerEvent debugEvent(wxEVT_WXLUA_DEBUGGER_DEBUGGEE_DISCONNECTED, this);
            //debugEvent.SetMessage(wxT("Debugger socket not connected. ") + msg);
            //SendEvent(debugEvent);
        }

        return false;
    }

    return true;
}

bool LuaDebugger::checkSocketRead(bool isOk, const wxString& msg)
{
    if (!isOk)
    {
        wxString s = wxT("Failed reading from the debugger socket. ") + msg + wxT("\n");
        s += getSocketErrorMsg();

        //wxLuaDebuggerEvent debugEvent(wxEVT_WXLUA_DEBUGGER_DEBUGGEE_DISCONNECTED, this);
        //debugEvent.SetMessage(s);
        //SendEvent(debugEvent);
    }

    return isOk;
}

bool LuaDebugger::checkSocketWrite(bool isOk, const wxString& msg)
{
    if (!isOk)
    {
        wxString s = wxT("Failed writing to the debugger socket. ") + msg + wxT("\n");
        s += getSocketErrorMsg();

        //wxLuaDebuggerEvent debugEvent(wxEVT_WXLUA_DEBUGGER_DEBUGGEE_DISCONNECTED, this);
        //debugEvent.SetMessage(s);
        //SendEvent(debugEvent);
    }

    return isOk;
}

wxString LuaDebugger::getSocketErrorMsg()
{
    String s;

    if (serverSocket)
        s += serverSocket->getErrorMsg(true);
    if (acceptedSocket)
        s += acceptedSocket->getErrorMsg(true);

    return wxString::FromUTF8(s.c_str());
}