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
#include "LuaDebugger.h"
#include <wx/xrc/xmlres.h>
#include <wx/aui/aui.h>

int idLuaDebugStart = XRCID("idLuaDebugStart");
int idLuaDebugPause = XRCID("idLuaDebugPause");
int idLuaDebugStop = XRCID("idLuaDebugStop");
int idLuaDebugStepIn = XRCID("idLuaDebugStepIn");
int idLuaDebugStepOver = XRCID("idLuaDebugStepOver");
int idLuaDebugStepOut = XRCID("idLuaDebugStepOut");

int idLuaDebugWatch = XRCID("idLuaDebugWatch");
int idLuaDebugCallStack = XRCID("idLuaDebugCallStack");
int idLuaDebugOutput = XRCID("idLuaDebugOutput");
int idLuaDebugLocals = XRCID("idLuaDebugLocals");

DEFINE_EVENT_TYPE(wxEVT_LUA_DEBUGGER_DEBUGGEE_CONNECTED)
DEFINE_EVENT_TYPE(wxEVT_LUA_DEBUGGER_DEBUGGEE_DISCONNECTED)
DEFINE_EVENT_TYPE(wxEVT_LUA_DEBUGGER_BREAK)
DEFINE_EVENT_TYPE(wxEVT_LUA_DEBUGGER_PRINT)
DEFINE_EVENT_TYPE(wxEVT_LUA_DEBUGGER_ERROR)
DEFINE_EVENT_TYPE(wxEVT_LUA_DEBUGGER_EXIT)
DEFINE_EVENT_TYPE(wxEVT_LUA_DEBUGGER_STACK_ENUM)
DEFINE_EVENT_TYPE(wxEVT_LUA_DEBUGGER_STACK_ENTRY_ENUM)
DEFINE_EVENT_TYPE(wxEVT_LUA_DEBUGGER_TABLE_ENUM)
DEFINE_EVENT_TYPE(wxEVT_LUA_DEBUGGER_EVALUATE_EXPR)

BEGIN_EVENT_TABLE(LuaDebugger, DebuggerPlugin)
    EVT_UPDATE_UI(idLuaDebugStart, LuaDebugger::onMenuUpdateUI)
    EVT_UPDATE_UI(idLuaDebugPause, LuaDebugger::onMenuUpdateUI)
    EVT_UPDATE_UI(idLuaDebugStop, LuaDebugger::onMenuUpdateUI)
    EVT_UPDATE_UI(idLuaDebugStepIn, LuaDebugger::onMenuUpdateUI)
    EVT_UPDATE_UI(idLuaDebugStepOver, LuaDebugger::onMenuUpdateUI)
    EVT_UPDATE_UI(idLuaDebugStepOut, LuaDebugger::onMenuUpdateUI)

    EVT_MENU(idLuaDebugStart, LuaDebugger::onLuaDebugStart)
    EVT_MENU(idLuaDebugPause, LuaDebugger::onLuaDebugPause)
    EVT_MENU(idLuaDebugStop, LuaDebugger::onLuaDebugStop)
    EVT_MENU(idLuaDebugStepIn, LuaDebugger::onLuaDebugStepIn)
    EVT_MENU(idLuaDebugStepOver, LuaDebugger::onLuaDebugStepOver)
    EVT_MENU(idLuaDebugStepOut, LuaDebugger::onLuaDebugStepOut)
    EVT_MENU(idLuaDebugWatch, LuaDebugger::onLuaDebugShowWindow)
    EVT_MENU(idLuaDebugCallStack, LuaDebugger::onLuaDebugShowWindow)
    EVT_MENU(idLuaDebugOutput, LuaDebugger::onLuaDebugShowWindow)
    EVT_MENU(idLuaDebugLocals, LuaDebugger::onLuaDebugShowWindow)

    EVT_LUA_DEBUGGER_DEBUGGEE_CONNECTED(wxID_ANY, LuaDebugger::onLuaDebugDebuggeeConnected)
    EVT_LUA_DEBUGGER_DEBUGGEE_DISCONNECTED(wxID_ANY, LuaDebugger::onLuaDebugDebuggeeDisconnected)
    EVT_LUA_DEBUGGER_EXIT(wxID_ANY, LuaDebugger::onLuaDebugExit)
    EVT_LUA_DEBUGGER_BREAK(wxID_ANY, LuaDebugger::onLuaDebugBreak)
    EVT_LUA_DEBUGGER_STACK_ENUM(wxID_ANY, LuaDebugger::onLuaDebugStackEnum)
    EVT_LUA_DEBUGGER_STACK_ENTRY_ENUM(wxID_ANY, LuaDebugger::onLuaDebugStackEntryEnum)
    EVT_LUA_DEBUGGER_EVALUATE_EXPR(wxID_ANY, LuaDebugger::onLuaDebugEvaluateExpr)
    EVT_LUA_DEBUGGER_TABLE_ENUM(wxID_ANY, LuaDebugger::onLuaDebugTableEnum)
    EVT_LUA_DEBUGGER_PRINT(wxID_ANY, LuaDebugger::onLuaDebugPrint)
    EVT_LUA_DEBUGGER_ERROR(wxID_ANY, LuaDebugger::onLuaDebugError)

END_EVENT_TABLE()

// ------------------------------------------------------------------------
//   LuaDebuggerConfig
// ------------------------------------------------------------------------
HARE_IMPLEMENT_DYNAMIC_CLASS(LuaDebuggerConfig, Object, 0)
{
    HARE_META(remoteDebug, bool)
    HARE_META(hostName, String)
    HARE_META(portNumber, int)
    HARE_META(appPath, String)
    HARE_META(layout, String)
}

LuaDebuggerConfig::LuaDebuggerConfig()
 : remoteDebug(false), hostName("localhost"),
   portNumber(9981), appPath("./lua_launcher")
{
}

wxString LuaDebuggerConfig::getLayout() const
{
    return wxString::FromUTF8(layout.c_str());
}

void LuaDebuggerConfig::setLayout(const wxString& val)
{
    layout = val.ToUTF8().data();
}

bool LuaDebuggerConfig::isRemoteDebug() const
{
    return remoteDebug;
}
void LuaDebuggerConfig::setRemoteDebug(bool val)
{
    remoteDebug = val;
}
wxString LuaDebuggerConfig::getHostName() const
{
    return wxString::FromUTF8(hostName.c_str());
}
void LuaDebuggerConfig::setHostName(const wxString& val)
{
    hostName = val.ToUTF8().data();
}
int LuaDebuggerConfig::getPortNumber() const
{
    return portNumber;
}
void LuaDebuggerConfig::setPortNumber(int val)
{
    portNumber = val;
}
wxString LuaDebuggerConfig::getAppPath() const
{
    return wxString::FromUTF8(appPath.c_str());
}
void LuaDebuggerConfig::setAppPath(const wxString& val)
{
    appPath = val.ToUTF8().data();
}

// ------------------------------------------------------------------------
//   LuaDebuggerProcess
// ------------------------------------------------------------------------
void LuaDebuggerProcess::OnTerminate(int pid, int status)
{
    // If this is being deleted from the onDetach of LuaDebugger
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
//   LuaDebuggerEvent
// ------------------------------------------------------------------------
LuaDebuggerEvent::LuaDebuggerEvent(const LuaDebuggerEvent& event)
 : wxEvent(event), lineNumber(event.lineNumber), fileName(event.fileName),
   strMessage(event.strMessage), debugData(event.debugData), stackRef(event.stackRef)
{
}

LuaDebuggerEvent::LuaDebuggerEvent(wxEventType eventType,
    wxObject* eventObject, int line, const String &file)
 : wxEvent(0, eventType), lineNumber(line), fileName(file), stackRef(0)
{
    SetEventObject(eventObject);
}

// ------------------------------------------------------------------------
//   LuaDebugger
// ------------------------------------------------------------------------
LuaDebugger::LuaDebugger()
 : debuggeeProcess(NULL), debuggeePID(-1), serverSocket(0), thread(0), acceptedSocket(0),
   callStackWindow(0), watchWindow(0), outputWindow(0), localWindow(0), currStackLevel(0),
   state(Debugger_Stoped)
{
}

LuaDebugger::~LuaDebugger()
{
}

void LuaDebugger::onAttach()
{
    callStackWindow = new LuaCallStackWindow(Manager::getInstancePtr()->getAppWindow(), this);

    {
        EditorDockEvent event(editorEVT_ADD_DOCK_WINDOW);
        event.info.Name(wxT("CallStackPane")).Caption(_("Call Stack")).Float().BestSize(150, 150)
            .FloatingSize(450, 150).MinSize(150, 150).Hide();
        event.window = callStackWindow;
        Manager::getInstancePtr()->processEvent(event);
    }

    {
        EditorDockEvent event(editorEVT_ADD_DOCK_WINDOW);
        watchWindow = new LuaWatchWindow(Manager::getInstancePtr()->getAppWindow(), this);
        event.info.Name(wxT("WatchPane")).Caption(_("Watch")).Float().BestSize(150, 150)
            .FloatingSize(450, 150).MinSize(150, 150).Hide();
        event.window = watchWindow;
        Manager::getInstancePtr()->processEvent(event);
    }

    {
        EditorDockEvent event(editorEVT_ADD_DOCK_WINDOW);
        outputWindow = new LuaOutputWindow(Manager::getInstancePtr()->getAppWindow(), this);
        event.info.Name(wxT("DebugOutputPane")).Caption(_("Debug Output")).Float().BestSize(150, 150)
            .FloatingSize(450, 150).MinSize(150, 150).Hide();
        event.window = outputWindow;
        Manager::getInstancePtr()->processEvent(event);
    }

    {
        EditorDockEvent event(editorEVT_ADD_DOCK_WINDOW);
        localWindow = new LuaLocalWindow(Manager::getInstancePtr()->getAppWindow(), this);
        event.info.Name(wxT("LocalsPane")).Caption(_("Locals")).Float().BestSize(150, 150)
            .FloatingSize(450, 150).MinSize(150, 150).Hide();
        event.window = localWindow;
        Manager::getInstancePtr()->processEvent(event);
    }

    config = (LuaDebuggerConfig*)Object::importObject("/editor/lua_debugger.xml");
    if (!config)
        config = new LuaDebuggerConfig();

    Manager::getInstancePtr()->registerEvent(editorEVT_APP_BEFORE_SHUTDOWN,
        new TEventHandler<LuaDebugger, EditorEvent>(this, &LuaDebugger::onAppBeforeShutdown));
}

void LuaDebugger::onDetach(bool isShutDown)
{
    EditorDockEvent event(editorEVT_DEL_DOCK_WINDOW);
    event.window = callStackWindow;
    Manager::getInstancePtr()->processEvent(event);
    event.window = watchWindow;
    Manager::getInstancePtr()->processEvent(event);
    event.window = outputWindow;
    Manager::getInstancePtr()->processEvent(event);
    event.window = localWindow;
    Manager::getInstancePtr()->processEvent(event);

    callStackWindow->Destroy();
    callStackWindow = 0;

    watchWindow->Destroy();
    watchWindow = 0;

    outputWindow->Destroy();
    outputWindow = 0;

    localWindow->Destroy();
    localWindow = 0;
}

void LuaDebugger::onAppBeforeShutdown(EditorEvent& event)
{
    stopDebugger();

    // IMPORTANT!!! wxExecute post a message to the message queue,
    // MUST process it before exit. 3-15-2009 lituo
    while (wxTheApp->Pending() && wxTheApp->Dispatch());

    // we don't delete the the process, we kill it and its OnTerminate deletes it
    if ((debuggeeProcess != NULL) && (debuggeePID > 0) &&
        wxProcess::Exists(debuggeePID))
    {
        debuggeeProcess->debugger = NULL;
        debuggeeProcess = NULL;
        wxProcess::Kill(debuggeePID, wxSIGKILL, wxKILL_CHILDREN);
    }

    while (wxTheApp->Pending() && wxTheApp->Dispatch());

    if (config)
        config->saveToXml("/editor/lua_debugger.xml");
}

bool LuaDebugger::buildMenuBar(wxMenuBar* menuBar)
{
    wxMenu* menu = wxXmlResource::Get()->LoadMenu(wxT("lua_debugger_menu"));
    menuBar->Insert(3, menu, _("Lua&Debug"));

    return true;
}

bool LuaDebugger::buildToolBar(wxAuiToolBar* toolBar)
{
    wxString fullPath = Manager::getInstancePtr()->getAppDir() + wxT("/resources/");
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
    toolBar->AddSeparator();
    bmp.LoadFile(fullPath + wxT("locals.png"), wxBITMAP_TYPE_PNG);
    toolBar->AddTool(idLuaDebugLocals, _("Locals"), bmp, wxEmptyString);
    bmp.LoadFile(fullPath + wxT("watch.png"), wxBITMAP_TYPE_PNG);
    toolBar->AddTool(idLuaDebugWatch, _("Watch"), bmp, wxEmptyString);
    bmp.LoadFile(fullPath + wxT("call_stack.png"), wxBITMAP_TYPE_PNG);
    toolBar->AddTool(idLuaDebugCallStack, _("Call Stack"), bmp, wxEmptyString);
    bmp.LoadFile(fullPath + wxT("output.png"), wxBITMAP_TYPE_PNG);
    toolBar->AddTool(idLuaDebugOutput, _("Debug Output"), bmp, wxEmptyString);

    return true;
}

void LuaDebugger::OnEndDebugeeProcess(wxProcessEvent& event)
{
    if (debuggeeProcess != NULL)
    {
        LuaDebuggerEvent debugEvent(wxEVT_LUA_DEBUGGER_EXIT, this);
        debugEvent.strMessage = StringUtil::format("Process [%d] ended with exit code : %d\n", event.GetPid(), event.GetExitCode());
        wxPostEvent(this, debugEvent);
    }

    event.Skip();
}

void LuaDebugger::onMenuUpdateUI(wxUpdateUIEvent& event)
{
    if (Manager::isAppShuttingDown())
    {
        event.Skip();
        return;
    }

    int id = event.GetId();

    if (id == idLuaDebugStart)
        event.Enable(state == Debugger_Stoped || state == Debugger_Breaked);
    else if (id == idLuaDebugPause)
        event.Enable(state == Debugger_Running);
    else if (id == idLuaDebugStop)
        event.Enable(state != Debugger_Stoped);
    else if (id == idLuaDebugStepIn)
        event.Enable(state == Debugger_Stoped || state == Debugger_Breaked);
    else if (id == idLuaDebugStepOver)
        event.Enable(state == Debugger_Stoped || state == Debugger_Breaked);
    else if (id == idLuaDebugStepOut)
        event.Enable(state == Debugger_Breaked);
    else
        event.Skip();
}

void LuaDebugger::onLuaDebugStart(wxCommandEvent& event)
{
    start();
}

void LuaDebugger::onLuaDebugPause(wxCommandEvent& event)
{
    breakExec();
}

void LuaDebugger::onLuaDebugStop(wxCommandEvent& event)
{
    stopDebugger();
}

void LuaDebugger::onLuaDebugStepIn(wxCommandEvent& event)
{
    step();
}

void LuaDebugger::onLuaDebugStepOver(wxCommandEvent& event)
{
    stepOver();
}

void LuaDebugger::onLuaDebugStepOut(wxCommandEvent& event)
{
    stepOut();
}

void LuaDebugger::onLuaDebugShowWindow(wxCommandEvent& event)
{
    wxWindow* win = 0;

    if (event.GetId() == idLuaDebugWatch)
        win = watchWindow;
    else if (event.GetId() == idLuaDebugCallStack)
        win = callStackWindow;
    else if (event.GetId() == idLuaDebugOutput)
        win = outputWindow;
    else if (event.GetId() == idLuaDebugLocals)
        win = localWindow;

    if (win)
    {
        EditorDockEvent evt(editorEVT_SHOW_DOCK_WINDOW);
        evt.window = win;
        evt.show = !win->IsShownOnScreen();
        Manager::getInstancePtr()->processEvent(evt);
    }
}

bool LuaDebugger::start()
{
    bool ret = false;

    if (state == Debugger_Stoped)
    {
        if (!startDebugger())
            return ret;

        outputWindow->clear();

        switchToDebugLayout();

        outputWindow->append(wxT("Debugger started, waiting for connection...\n"), Log_Info);

        if (!config->isRemoteDebug())
            ret = startDebuggee() > 0;
        else
        {
            wxString cmd = wxString::Format(wxT("Remote debugging mode, start debuggee with '-debug %s:%d'\n"),
                config->getHostName().c_str(),
                config->getPortNumber());
            outputWindow->append(cmd, Log_Info);
        }

        startCmd = StartCmd_Go;
        state = Debugger_Wait;
    }
    else if (state == Debugger_Breaked)
    {
        ret = continueExec();
    }

    return ret;
}

bool LuaDebugger::step()
{
    bool ret = false;

    if (state == Debugger_Stoped)
    {
        ret = start();
        startCmd = StartCmd_StepIn;
    }
    else
    {
        ret = checkSocketConnected(true, wxT("Debugger Step")) && checkSocketWrite(
            SocketHelper::writeCmd(getSocket(), LUA_DEBUGGER_CMD_DEBUG_STEP),
            wxT("Debugger Step"));

        if (ret)
        {
            clearEditorDebugMark();
            state = Debugger_Running;
        }
    }

   return ret;
}

bool LuaDebugger::stepOver()
{
    bool ret = false;

    if (state == Debugger_Stoped)
    {
        ret = start();
        startCmd = StartCmd_StepOver;
    }
    else
    {
        ret = checkSocketConnected(true, wxT("Debugger StepOver")) && checkSocketWrite(
            SocketHelper::writeCmd(getSocket(), LUA_DEBUGGER_CMD_DEBUG_STEPOVER),
            wxT("Debugger StepOver"));

        if (ret)
        {
            clearEditorDebugMark();
            state = Debugger_Running;
        }
    }

    return ret;
}

bool LuaDebugger::stepOut()
{
    bool ret = checkSocketConnected(true, wxT("Debugger StepOut")) && checkSocketWrite(
        SocketHelper::writeCmd(getSocket(), LUA_DEBUGGER_CMD_DEBUG_STEPOUT),
        wxT("Debugger StepOut"));

    if (ret)
    {
        clearEditorDebugMark();

        state = Debugger_Running;
    }

    return ret;
}

bool LuaDebugger::continueExec()
{
    bool ret = checkSocketConnected(true, wxT("Debugger Continue")) && checkSocketWrite(
        SocketHelper::writeCmd(getSocket(), LUA_DEBUGGER_CMD_DEBUG_CONTINUE),
        wxT("Debugger Continue"));

    if (ret)
    {
        clearEditorDebugMark();

        state = Debugger_Running;
    }

    return ret;
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
    if (state != Debugger_Breaked)
        return false;

    return checkSocketConnected(true, wxT("Debugger EnumerateStack")) && checkSocketWrite(
        SocketHelper::writeCmd(getSocket(), LUA_DEBUGGER_CMD_ENUMERATE_STACK),
        wxT("Debugger EnumerateStack"));
}

bool LuaDebugger::enumerateStackEntry(int stackEntry)
{
    if (state != Debugger_Breaked)
        return false;

    return checkSocketConnected(true, wxT("Debugger EnumerateStackEntry")) && checkSocketWrite(
        SocketHelper::writeCmd(getSocket(), LUA_DEBUGGER_CMD_ENUMERATE_STACK_ENTRY) &&
        SocketHelper::writeInt(getSocket(), stackEntry),
        wxT("Debugger EnumerateStackEntry"));
}

bool LuaDebugger::enumerateTable(int stackRef, const String &table)
{
    if (state != Debugger_Breaked)
        return false;

    return checkSocketConnected(true, wxT("Debugger EnumerateTable")) && checkSocketWrite(
        SocketHelper::writeCmd(getSocket(), LUA_DEBUGGER_CMD_ENUMERATE_TABLE) &&
        SocketHelper::writeInt(getSocket(), stackRef) &&
        SocketHelper::writeString(getSocket(), table),
        wxT("Debugger EnumerateTable"));
}

bool LuaDebugger::evaluateExpr(int stackRef, const String &strExpression)
{
    if (state != Debugger_Breaked)
        return false;

    return checkSocketConnected(true, wxT("Debugger EvaluateExpr")) && checkSocketWrite(
        SocketHelper::writeCmd(getSocket(), LUA_DEBUGGER_CMD_EVALUATE_EXPR) &&
        SocketHelper::writeInt(getSocket(), stackRef) &&
        SocketHelper::writeString(getSocket(), strExpression),
        wxT("Debugger EvaluateExpr"));
}

bool LuaDebugger::addBreakPoint(const String &fileName, int lineNumber)
{
    if (state == Debugger_Stoped)
        return true;

    return checkSocketConnected(true, wxT("Debugger AddBreakPoint")) && checkSocketWrite(
        SocketHelper::writeCmd(getSocket(), LUA_DEBUGGER_CMD_ADD_BREAKPOINT) &&
        SocketHelper::writeString(getSocket(), fileName) &&
        SocketHelper::writeInt(getSocket(), lineNumber),
        wxT("Debugger AddBreakPoint"));
}

bool LuaDebugger::removeBreakPoint(const String &fileName, int lineNumber)
{
    if (state == Debugger_Stoped)
        return true;

    return checkSocketConnected(true, wxT("Debugger RemoveBreakPoint")) && checkSocketWrite(
        SocketHelper::writeCmd(getSocket(), LUA_DEBUGGER_CMD_REMOVE_BREAKPOINT) &&
        SocketHelper::writeString(getSocket(), fileName) &&
        SocketHelper::writeInt(getSocket(), lineNumber),
        wxT("Debugger RemoveBreakPoint"));
}

bool LuaDebugger::clearAllBreakPoints()
{
    return checkSocketConnected(true, wxT("Debugger ClearAllBreakPoints")) && checkSocketWrite(
        SocketHelper::writeCmd(getSocket(), LUA_DEBUGGER_CMD_CLEAR_ALL_BREAKPOINTS),
        wxT("Debugger ClearAllBreakPoints"));
}

long LuaDebugger::startDebuggee()
{
    if (debuggeeProcess == NULL)
    {
        Project* prj = Manager::getInstancePtr()->getExplorerManager()->getProjectExplorer()->getActiveProject();

        wxCHECK_MSG(prj != NULL, false, wxT("Cannot find active project"));

        // Note : the LuaDebugger is not the event handler for process end event
        debuggeeProcess = new LuaDebuggerProcess(this);
        wxString command = wxString::Format(wxT("%s -game %s -debug %s:%u"),
            config->getAppPath().c_str(),
            wxString::FromUTF8(prj->projectName.c_str()).c_str(),
            config->getHostName().c_str(),
            config->getPortNumber());

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

    if (serverSocket->listen(config->getPortNumber()))
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
        LuaDebuggerEvent debugEvent(wxEVT_LUA_DEBUGGER_ERROR, this);
        debugEvent.strMessage = serverSocket->getErrorMsg(true) + "\n";
        AddPendingEvent(debugEvent);

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
            LuaDebuggerEvent debugEvent(wxEVT_LUA_DEBUGGER_ERROR, this);
            debugEvent.strMessage = acceptedSocket->getErrorMsg(true) + "\n";
            AddPendingEvent(debugEvent);
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

        if (!closeSocket.connect(config->getHostName().ToUTF8().data(), config->getPortNumber()) ||
            !closeSocket.shutdown(SD_BOTH))
        {
            LuaDebuggerEvent debugEvent(wxEVT_LUA_DEBUGGER_ERROR, this);
            debugEvent.strMessage = serverSocket->getErrorMsg(true) + "\n";
            AddPendingEvent(debugEvent);
        }

        wxMilliSleep(100);

        delete svrSocket;
    }

    // One of the above two operations terminates the thread. Wait for it to stop.
    while ((thread != NULL) && thread->IsRunning())
        wxMilliSleep(100);

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
        LuaDebuggerEvent debugEvent(wxEVT_LUA_DEBUGGER_ERROR, this);
        debugEvent.strMessage = serverSocket->getErrorMsg(true) + "\n";
        AddPendingEvent(debugEvent);
    }
    else
    {
        Socket *svrSocket = serverSocket;
        serverSocket = NULL;
        delete svrSocket;

        wxThread::Sleep(500);

        // Notify that a client has connected and we are ready to debug
        LuaDebuggerEvent debugEvent(wxEVT_LUA_DEBUGGER_DEBUGGEE_CONNECTED, this);
        AddPendingEvent(debugEvent);

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

            if ((debug_event == LUA_DEBUGGEE_EVENT_EXIT) ||
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
    LuaDebuggerEvent debugEvent(wxEVT_LUA_DEBUGGER_EXIT, this);
    wxPostEvent(this, debugEvent);
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
                LuaDebuggerEvent debugEvent(wxEVT_LUA_DEBUGGER_BREAK, this, lineNumber, fileName);
                AddPendingEvent(debugEvent);
            }
            else return -1;

            break;
        }
    case LUA_DEBUGGEE_EVENT_PRINT:
        {
            String strMessage;

            if (checkSocketRead(
                SocketHelper::readString(getSocket(), strMessage),
                wxT("Debugger LUADEBUGGEE_EVENT_PRINT")))
            {
                LuaDebuggerEvent debugEvent(wxEVT_LUA_DEBUGGER_PRINT, this);
                debugEvent.strMessage = strMessage;
                AddPendingEvent(debugEvent);
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
                LuaDebuggerEvent debugEvent(wxEVT_LUA_DEBUGGER_ERROR, this);
                debugEvent.strMessage = strMessage;
                AddPendingEvent(debugEvent);
            }
            else return -1;

            break;
        }
    case LUA_DEBUGGEE_EVENT_EXIT:
        {
            LuaDebuggerEvent debugEvent(wxEVT_LUA_DEBUGGER_EXIT, this);
            wxPostEvent(this, debugEvent);
            break;
        }
    case LUA_DEBUGGEE_EVENT_STACK_ENUM:
        {
            Object* debugData = 0;

            if (checkSocketRead(
                SocketHelper::readObject(getSocket(), debugData),
                wxT("Debugger LUA_DEBUGGEE_EVENT_STACK_ENUM")))
            {
                LuaDebuggerEvent debugEvent(wxEVT_LUA_DEBUGGER_STACK_ENUM, this);
                debugEvent.debugData = (LuaDebugData*)debugData;
                AddPendingEvent(debugEvent);
            }
            else return -1;

            break;
        }
    case LUA_DEBUGGEE_EVENT_STACK_ENTRY_ENUM:
        {
            int stackRef = 0;
            Object* debugData = 0;

            if (checkSocketRead(
                SocketHelper::readInt(getSocket(), stackRef) &&
                SocketHelper::readObject(getSocket(), debugData),
                wxT("Debugger LUA_DEBUGGEE_EVENT_STACK_ENTRY_ENUM")))
            {
                LuaDebuggerEvent debugEvent(wxEVT_LUA_DEBUGGER_STACK_ENTRY_ENUM, this);
                debugEvent.debugData = (LuaDebugData*)debugData;
                debugEvent.stackRef = stackRef;
                AddPendingEvent(debugEvent);
            }
            else return -1;

            break;
        }
    case LUA_DEBUGGEE_EVENT_TABLE_ENUM:
        {
            Object* debugData = 0;
            String tableName;

            if (checkSocketRead(
                SocketHelper::readString(getSocket(), tableName) &&
                SocketHelper::readObject(getSocket(), debugData),
                wxT("Debugger LUA_DEBUGGEE_EVENT_TABLE_ENUM")))
            {
                LuaDebuggerEvent debugEvent(wxEVT_LUA_DEBUGGER_TABLE_ENUM, this);
                debugEvent.debugData = (LuaDebugData*)debugData;
                debugEvent.strMessage = tableName;
                AddPendingEvent(debugEvent);
            }
            else return -1;

            break;
        }
    case LUA_DEBUGGEE_EVENT_EVALUATE_EXPR:
        {
            Object* debugData = 0;

            if (checkSocketRead(
                SocketHelper::readObject(getSocket(), debugData),
                wxT("Debugger LUA_DEBUGGEE_EVENT_EVALUATE_EXPR")))
            {
                LuaDebuggerEvent debugEvent(wxEVT_LUA_DEBUGGER_EVALUATE_EXPR, this);
                debugEvent.debugData = (LuaDebugData*)debugData;
                AddPendingEvent(debugEvent);
            }
            else return -1;

            break;
        }
    default :
        return -1; // don't know this event?
    }

    return event_type;
}

bool LuaDebugger::checkSocketConnected(bool sendEvent, const wxString& msg)
{
    if (getSocket() == NULL)
    {
        if (sendEvent)
        {
            LuaDebuggerEvent debugEvent(wxEVT_LUA_DEBUGGER_DEBUGGEE_DISCONNECTED, this);
            debugEvent.strMessage = String("Debugger socket not created. ") + msg.ToUTF8().data() + "\n";
            AddPendingEvent(debugEvent);
        }

        return false;
    }
    else if (!getSocket()->isConnected())
    {
        if (sendEvent)
        {
            LuaDebuggerEvent debugEvent(wxEVT_LUA_DEBUGGER_DEBUGGEE_DISCONNECTED, this);
            debugEvent.strMessage = String("Debugger socket not connected. ") + msg.ToUTF8().data() + "\n";
            AddPendingEvent(debugEvent);
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

        LuaDebuggerEvent debugEvent(wxEVT_LUA_DEBUGGER_DEBUGGEE_DISCONNECTED, this);
        debugEvent.strMessage = s.ToUTF8().data();
        AddPendingEvent(debugEvent);
    }

    return isOk;
}

bool LuaDebugger::checkSocketWrite(bool isOk, const wxString& msg)
{
    if (!isOk)
    {
        wxString s = wxT("Failed writing to the debugger socket. ") + msg + wxT("\n");
        s += getSocketErrorMsg();

        LuaDebuggerEvent debugEvent(wxEVT_LUA_DEBUGGER_DEBUGGEE_DISCONNECTED, this);
        debugEvent.strMessage = s.ToUTF8().data();
        AddPendingEvent(debugEvent);
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

void LuaDebugger::clearEditorDebugMark()
{
    EditorPageManager* em = Manager::getInstancePtr()->getEditorPageManager();
    for (int i = 0; i < em->getPageCount(); ++i)
    {
        EditorPage* ep = em->getPage(i);
        if (wxIsKindOf(ep, TextEditorPage))
        {
            TextEditorPage* tep = (TextEditorPage*)ep;
            tep->setDebugLine(-1);
        }
    }
}

void LuaDebugger::syncEditor(const String& fileName, int lineNumber)
{
    clearEditorDebugMark();

    Project* project = Manager::getInstancePtr()->getExplorerManager()->getProjectExplorer()->getActiveProject();

    String baseName;
    StringVector strs = StringUtil::split(fileName, "\\/");
    if (strs.size() > 0)
        baseName = strs[strs.size() - 1];

    ProjectFile* pf = project ? project->findFile(baseName) : 0;

    wxString file = Manager::getInstancePtr()->getAppDir() + wxT("scripts/") + wxString::FromUTF8(fileName.c_str());
    TextEditorPage* page = Manager::getInstancePtr()->getEditorPageManager()->openInTextEditor(file);
    if (page)
    {
        if (pf && !page->getProjectFile())
            page->setProjectFile(pf);
        page->gotoLine(lineNumber);
        page->setDebugLine(lineNumber);
    }
}

void LuaDebugger::setCurrStackLevel(int stackLevel)
{
    if (stackLevel != currStackLevel)
    {
        currStackLevel = stackLevel;
        watchWindow->redraw();
    }
}

int LuaDebugger::getCurrStackLevel()
{
    return currStackLevel;
}

void LuaDebugger::onLuaDebugDebuggeeConnected(LuaDebuggerEvent& event)
{
    outputWindow->append(wxT("Debuggee has connected, ready to debug.\n"), Log_Info);

    state = Debugger_Running;

    clearAllBreakPoints();

    Project* project = Manager::getInstancePtr()->getExplorerManager()->getProjectExplorer()->getActiveProject();
    if (project)
    {
        ProjectFile::List::iterator it = project->files.begin();
        for (; it != project->files.end(); ++it)
        {
            for (size_t i = 0; i < (*it)->breakPoints.size(); ++i)
                addBreakPoint("/" + project->projectName + "/" + (*it)->fileName, (*it)->breakPoints[i]);
        }
    }

    // start command
    if (startCmd == StartCmd_Go)
        continueExec();
    else if (startCmd == StartCmd_StepIn)
        step();
    else if (startCmd == StartCmd_StepOver)
        stepOver();
}

void LuaDebugger::onLuaDebugDebuggeeDisconnected(LuaDebuggerEvent& event)
{
    outputWindow->append(wxString::FromUTF8(event.strMessage.c_str()), Log_Error);
}

void LuaDebugger::onLuaDebugExit(LuaDebuggerEvent& event)
{
    outputWindow->append(wxString::FromUTF8(event.strMessage.c_str()), Log_Info);

    stopDebugger();

    state = Debugger_Stoped;

    clearEditorDebugMark();

    restoreLayout();
}

void LuaDebugger::onLuaDebugBreak(LuaDebuggerEvent& event)
{
    state = Debugger_Breaked;

    setCurrStackLevel(0);

    syncEditor(event.fileName, event.lineNumber);

    if (callStackWindow->IsShownOnScreen())
        enumerateStack();

    if (watchWindow->IsShownOnScreen())
        watchWindow->redraw();

    if (localWindow->IsShownOnScreen())
        enumerateStackEntry(getCurrStackLevel());

    Manager::getInstancePtr()->getAppWindow()->Raise();
}

void LuaDebugger::onLuaDebugPrint(LuaDebuggerEvent& event)
{
    outputWindow->append(wxString::FromUTF8(event.strMessage.c_str()), Log_Print);
}

void LuaDebugger::onLuaDebugError(LuaDebuggerEvent& event)
{
    outputWindow->append(wxString::FromUTF8(event.strMessage.c_str()), Log_Error);
}

void LuaDebugger::onLuaDebugStackEnum(LuaDebuggerEvent& event)
{
    callStackWindow->setCallStackData(event.debugData);
}

void LuaDebugger::onLuaDebugStackEntryEnum(LuaDebuggerEvent& event)
{
    if (event.stackRef == getCurrStackLevel())
    {
        localWindow->updateLocalData(event.debugData);
    }
}

void LuaDebugger::onLuaDebugEvaluateExpr(LuaDebuggerEvent& event)
{
    watchWindow->updateWatchData(event.debugData);
}

void LuaDebugger::onLuaDebugTableEnum(LuaDebuggerEvent& event)
{
    watchWindow->updateTableData(event.strMessage, event.debugData);
    localWindow->updateTableData(event.strMessage, event.debugData);
}

void LuaDebugger::switchToDebugLayout()
{
    EditorEvent queryEvent(editorEVT_LAYOUT_QUERY);
    Manager::getInstancePtr()->processEvent(queryEvent);
    previousLayout = queryEvent.strData;

    EditorEvent switchEvent(editorEVT_LAYOUT_SWITCH);
    switchEvent.strData = config->getLayout();
    Manager::getInstancePtr()->processEvent(switchEvent);
}

void LuaDebugger::restoreLayout()
{
    if (!previousLayout.IsEmpty())
    {
        EditorEvent queryEvent(editorEVT_LAYOUT_QUERY);
        Manager::getInstancePtr()->processEvent(queryEvent);
        config->setLayout(queryEvent.strData);

        EditorEvent switchEvent(editorEVT_LAYOUT_SWITCH);
        switchEvent.strData = previousLayout;
        Manager::getInstancePtr()->processEvent(switchEvent);

        previousLayout = wxEmptyString;
    }
}
