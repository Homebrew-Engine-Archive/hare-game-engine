//***************************************************************
//  File:    LuaDebuggerPlugin.cpp
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

BEGIN_EVENT_TABLE(LuaDebuggerPlugin, DebuggerPlugin)
    EVT_UPDATE_UI(idLuaDebugStart, LuaDebuggerPlugin::onMenuUpdateUI)
    EVT_UPDATE_UI(idLuaDebugPause, LuaDebuggerPlugin::onMenuUpdateUI)
    EVT_UPDATE_UI(idLuaDebugStop, LuaDebuggerPlugin::onMenuUpdateUI)
    EVT_UPDATE_UI(idLuaDebugStepIn, LuaDebuggerPlugin::onMenuUpdateUI)
    EVT_UPDATE_UI(idLuaDebugStepOver, LuaDebuggerPlugin::onMenuUpdateUI)
    EVT_UPDATE_UI(idLuaDebugStepOut, LuaDebuggerPlugin::onMenuUpdateUI)

    EVT_MENU(idLuaDebugStart, LuaDebuggerPlugin::onLuaDebugStart)
    EVT_MENU(idLuaDebugPause, LuaDebuggerPlugin::onLuaDebugPause)
    EVT_MENU(idLuaDebugStop, LuaDebuggerPlugin::onLuaDebugStop)
    EVT_MENU(idLuaDebugStepIn, LuaDebuggerPlugin::onLuaDebugStepIn)
    EVT_MENU(idLuaDebugStepOver, LuaDebuggerPlugin::onLuaDebugStepOver)
    EVT_MENU(idLuaDebugStepOut, LuaDebuggerPlugin::onLuaDebugStepOut)
    EVT_MENU(idLuaDebugWatch, LuaDebuggerPlugin::onLuaDebugShowWindow)
    EVT_MENU(idLuaDebugCallStack, LuaDebuggerPlugin::onLuaDebugShowWindow)
    EVT_MENU(idLuaDebugOutput, LuaDebuggerPlugin::onLuaDebugShowWindow)
    EVT_MENU(idLuaDebugLocals, LuaDebuggerPlugin::onLuaDebugShowWindow)

    EVT_LUA_DEBUGGER_DEBUGGEE_CONNECTED(wxID_ANY, LuaDebuggerPlugin::onLuaDebugDebuggeeConnected)
    EVT_LUA_DEBUGGER_DEBUGGEE_DISCONNECTED(wxID_ANY, LuaDebuggerPlugin::onLuaDebugDebuggeeDisconnected)
    EVT_LUA_DEBUGGER_EXIT(wxID_ANY, LuaDebuggerPlugin::onLuaDebugExit)
    EVT_LUA_DEBUGGER_BREAK(wxID_ANY, LuaDebuggerPlugin::onLuaDebugBreak)
    EVT_LUA_DEBUGGER_STACK_ENUM(wxID_ANY, LuaDebuggerPlugin::onLuaDebugStackEnum)
    EVT_LUA_DEBUGGER_STACK_ENTRY_ENUM(wxID_ANY, LuaDebuggerPlugin::onLuaDebugStackEntryEnum)
    EVT_LUA_DEBUGGER_EVALUATE_EXPR(wxID_ANY, LuaDebuggerPlugin::onLuaDebugEvaluateExpr)
    EVT_LUA_DEBUGGER_TABLE_ENUM(wxID_ANY, LuaDebuggerPlugin::onLuaDebugTableEnum)
    EVT_LUA_DEBUGGER_PRINT(wxID_ANY, LuaDebuggerPlugin::onLuaDebugPrint)
    EVT_LUA_DEBUGGER_ERROR(wxID_ANY, LuaDebuggerPlugin::onLuaDebugError)

END_EVENT_TABLE()

// ------------------------------------------------------------------------
//   LuaDebugger
// ------------------------------------------------------------------------
HARE_IMPLEMENT_DYNAMIC_CLASS(LuaDebugger, Object, 0)
{
    HARE_META(remoteDebug, bool)
    HARE_META(hostName, String)
    HARE_META(portNumber, int)
    HARE_META(appPath, String)
    HARE_META(layout, String)
}

LuaDebugger::LuaDebugger()
 : remoteDebug(false), hostName("localhost"),
   portNumber(9981), appPath("./launcher")
{
}

wxString LuaDebugger::getLayout() const
{
    return wxString::FromUTF8(layout.c_str());
}

void LuaDebugger::setLayout(const wxString& val)
{
    layout = val.ToUTF8().data();
}

bool LuaDebugger::isRemoteDebug() const
{
    return remoteDebug;
}
void LuaDebugger::setRemoteDebug(bool val)
{
    remoteDebug = val;
}
wxString LuaDebugger::getHostName() const
{
    return wxString::FromUTF8(hostName.c_str());
}
void LuaDebugger::setHostName(const wxString& val)
{
    hostName = val.ToUTF8().data();
}
int LuaDebugger::getPortNumber() const
{
    return portNumber;
}
void LuaDebugger::setPortNumber(int val)
{
    portNumber = val;
}
wxString LuaDebugger::getAppPath() const
{
    return wxString::FromUTF8(appPath.c_str());
}
void LuaDebugger::setAppPath(const wxString& val)
{
    appPath = val.ToUTF8().data();
}

// ------------------------------------------------------------------------
//   LuaDebuggerProcess
// ------------------------------------------------------------------------
void LuaDebuggerProcess::OnTerminate(int pid, int status)
{
    // If this is being deleted from the onDetach of LuaDebuggerPlugin
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
//   LuaDebuggerPlugin
// ------------------------------------------------------------------------
LuaDebuggerPlugin::LuaDebuggerPlugin()
 : debuggeeProcess(NULL), debuggeePID(-1), serverSocket(0), thread(0), acceptedSocket(0),
   callStackWindow(0), watchWindow(0), outputWindow(0), localWindow(0), currStackLevel(0),
   state(Debugger_Stoped), projectActived(false)
{
}

LuaDebuggerPlugin::~LuaDebuggerPlugin()
{
}

void LuaDebuggerPlugin::onAttach()
{
    callStackWindow = new LuaCallStackWindow(Manager::getInstancePtr()->getAppWindow(), this);

    {
        EditorDockEvent event(editorEVT_ADD_DOCK_WINDOW);
        event.info.Name(wxT("LuaCallStackPane")).Caption(_("LuaDebugger Call Stack")).Float().BestSize(150, 150)
            .FloatingSize(450, 150).MinSize(150, 150).Hide();
        event.window = callStackWindow;
        Manager::getInstancePtr()->processEvent(event);
    }

    {
        EditorDockEvent event(editorEVT_ADD_DOCK_WINDOW);
        watchWindow = new LuaWatchWindow(Manager::getInstancePtr()->getAppWindow(), this);
        event.info.Name(wxT("LuaWatchPane")).Caption(_("LuaDebugger Watch")).Float().BestSize(150, 150)
            .FloatingSize(450, 150).MinSize(150, 150).Hide();
        event.window = watchWindow;
        Manager::getInstancePtr()->processEvent(event);
    }

    {
        EditorDockEvent event(editorEVT_ADD_DOCK_WINDOW);
        outputWindow = new LuaOutputWindow(Manager::getInstancePtr()->getAppWindow(), this);
        event.info.Name(wxT("LuaDebugOutputPane")).Caption(_("LuaDebugger Output")).Float().BestSize(150, 150)
            .FloatingSize(450, 150).MinSize(150, 150).Hide();
        event.window = outputWindow;
        Manager::getInstancePtr()->processEvent(event);
    }

    {
        EditorDockEvent event(editorEVT_ADD_DOCK_WINDOW);
        localWindow = new LuaLocalWindow(Manager::getInstancePtr()->getAppWindow(), this);
        event.info.Name(wxT("LuaLocalsPane")).Caption(_("LuaDebugger Locals")).Float().BestSize(150, 150)
            .FloatingSize(450, 150).MinSize(150, 150).Hide();
        event.window = localWindow;
        Manager::getInstancePtr()->processEvent(event);
    }

    config = (LuaDebugger*)Object::importObject("/editor/lua_debugger.xml");
    if (!config)
        config = new LuaDebugger();

    Manager::getInstancePtr()->registerEvent(editorEVT_APP_BEFORE_SHUTDOWN,
        new TEventHandler<LuaDebuggerPlugin, EditorEvent>(this, &LuaDebuggerPlugin::onAppBeforeShutdown));
    Manager::getInstancePtr()->registerEvent(editorEVT_PROJECT_ACTIVED,
        new TEventHandler<LuaDebuggerPlugin, EditorEvent>(this, &LuaDebuggerPlugin::onProjectActived));
}

void LuaDebuggerPlugin::onDetach(bool isShutDown)
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

void LuaDebuggerPlugin::onProjectActived(EditorEvent& event)
{
    if (event.project)
    {
        if (event.project->debuggerName == config->getClassInfo()->className)
        {
            projectActived = true;
            return;
        }
    }

    projectActived = false;
}

void LuaDebuggerPlugin::onAppBeforeShutdown(EditorEvent& event)
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

bool LuaDebuggerPlugin::buildMenuBar(wxMenuBar* menuBar)
{
    wxMenu* menu = wxXmlResource::Get()->LoadMenu(wxT("lua_debugger_menu"));
    menuBar->Insert(3, menu, _("Lua&Debug"));

    return true;
}

bool LuaDebuggerPlugin::buildToolBar(wxAuiToolBar* toolBar)
{
    wxString zipFile = Manager::getInstancePtr()->getAppDir() + wxT("/resources.zip#zip:");

    toolBar->AddTool(idLuaDebugStart, _("Start"), 
        Manager::loadBitmap(zipFile + wxT("start.png")), wxT("Start"));
    toolBar->AddTool(idLuaDebugPause, _("Pause"), 
        Manager::loadBitmap(zipFile + wxT("pause.png")), wxT("Pause"));
    toolBar->AddTool(idLuaDebugStop, _("Stop"), 
        Manager::loadBitmap(zipFile + wxT("stop.png")), wxT("Stop"));
    toolBar->AddSeparator();
    toolBar->AddTool(idLuaDebugStepIn, _("StepIn"), 
        Manager::loadBitmap(zipFile + wxT("step_in.png")), wxT("StepIn"));
    toolBar->AddTool(idLuaDebugStepOver, _("StepOver"), 
        Manager::loadBitmap(zipFile + wxT("step_over.png")), wxT("StepOver"));
    toolBar->AddTool(idLuaDebugStepOut, _("StepOut"), 
        Manager::loadBitmap(zipFile + wxT("step_out.png")), wxT("StepOut"));
    toolBar->AddSeparator();
    toolBar->AddTool(idLuaDebugLocals, _("Locals"), 
        Manager::loadBitmap(zipFile + wxT("locals.png")), wxEmptyString);
    toolBar->AddTool(idLuaDebugWatch, _("Watch"), 
        Manager::loadBitmap(zipFile + wxT("watch.png")), wxEmptyString);
    toolBar->AddTool(idLuaDebugCallStack, _("Call Stack"), 
        Manager::loadBitmap(zipFile + wxT("call_stack.png")), wxEmptyString);
    toolBar->AddTool(idLuaDebugOutput, _("Debug Output"), 
        Manager::loadBitmap(zipFile + wxT("output.png")), wxEmptyString);

    return true;
}

void LuaDebuggerPlugin::OnEndDebugeeProcess(wxProcessEvent& event)
{
    if (debuggeeProcess != NULL)
    {
        LuaDebuggerEvent debugEvent(wxEVT_LUA_DEBUGGER_EXIT, this);
        debugEvent.strMessage = StringUtil::format("Process [%d] ended with exit code : %d\n", event.GetPid(), event.GetExitCode());
        wxPostEvent(this, debugEvent);
    }

    event.Skip();
}

void LuaDebuggerPlugin::onMenuUpdateUI(wxUpdateUIEvent& event)
{
    if (Manager::isAppShuttingDown())
    {
        event.Skip();
        return;
    }

    if (!projectActived)
    {
        event.Enable(false);
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

void LuaDebuggerPlugin::onLuaDebugStart(wxCommandEvent& event)
{
    start();
}

void LuaDebuggerPlugin::onLuaDebugPause(wxCommandEvent& event)
{
    breakExec();
}

void LuaDebuggerPlugin::onLuaDebugStop(wxCommandEvent& event)
{
    stopDebugger();
}

void LuaDebuggerPlugin::onLuaDebugStepIn(wxCommandEvent& event)
{
    step();
}

void LuaDebuggerPlugin::onLuaDebugStepOver(wxCommandEvent& event)
{
    stepOver();
}

void LuaDebuggerPlugin::onLuaDebugStepOut(wxCommandEvent& event)
{
    stepOut();
}

void LuaDebuggerPlugin::onLuaDebugShowWindow(wxCommandEvent& event)
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

bool LuaDebuggerPlugin::start()
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

bool LuaDebuggerPlugin::step()
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

bool LuaDebuggerPlugin::stepOver()
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

bool LuaDebuggerPlugin::stepOut()
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

bool LuaDebuggerPlugin::continueExec()
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

bool LuaDebuggerPlugin::breakExec()
{
    return checkSocketConnected(true, wxT("Debugger Break")) && checkSocketWrite(
        SocketHelper::writeCmd(getSocket(), LUA_DEBUGGER_CMD_DEBUG_BREAK),
        wxT("Debugger Break"));
}

bool LuaDebuggerPlugin::reset()
{
    return checkSocketConnected(true, wxT("Debugger Reset")) && checkSocketWrite(
        SocketHelper::writeCmd(getSocket(), LUA_DEBUGGER_CMD_RESET),
        wxT("Debugger Reset"));
}

bool LuaDebuggerPlugin::enumerateStack()
{
    if (state != Debugger_Breaked)
        return false;

    return checkSocketConnected(true, wxT("Debugger EnumerateStack")) && checkSocketWrite(
        SocketHelper::writeCmd(getSocket(), LUA_DEBUGGER_CMD_ENUMERATE_STACK),
        wxT("Debugger EnumerateStack"));
}

bool LuaDebuggerPlugin::enumerateStackEntry(int stackEntry)
{
    if (state != Debugger_Breaked)
        return false;

    return checkSocketConnected(true, wxT("Debugger EnumerateStackEntry")) && checkSocketWrite(
        SocketHelper::writeCmd(getSocket(), LUA_DEBUGGER_CMD_ENUMERATE_STACK_ENTRY) &&
        SocketHelper::writeInt(getSocket(), stackEntry),
        wxT("Debugger EnumerateStackEntry"));
}

bool LuaDebuggerPlugin::enumerateTable(int stackRef, const String &table)
{
    if (state != Debugger_Breaked)
        return false;

    return checkSocketConnected(true, wxT("Debugger EnumerateTable")) && checkSocketWrite(
        SocketHelper::writeCmd(getSocket(), LUA_DEBUGGER_CMD_ENUMERATE_TABLE) &&
        SocketHelper::writeInt(getSocket(), stackRef) &&
        SocketHelper::writeString(getSocket(), table),
        wxT("Debugger EnumerateTable"));
}

bool LuaDebuggerPlugin::evaluateExpr(int stackRef, const String &strExpression)
{
    if (state != Debugger_Breaked)
        return false;

    return checkSocketConnected(true, wxT("Debugger EvaluateExpr")) && checkSocketWrite(
        SocketHelper::writeCmd(getSocket(), LUA_DEBUGGER_CMD_EVALUATE_EXPR) &&
        SocketHelper::writeInt(getSocket(), stackRef) &&
        SocketHelper::writeString(getSocket(), strExpression),
        wxT("Debugger EvaluateExpr"));
}

bool LuaDebuggerPlugin::addBreakPoint(const String &fileName, int lineNumber)
{
    if (state == Debugger_Stoped)
        return true;

    return checkSocketConnected(true, wxT("Debugger AddBreakPoint")) && checkSocketWrite(
        SocketHelper::writeCmd(getSocket(), LUA_DEBUGGER_CMD_ADD_BREAKPOINT) &&
        SocketHelper::writeString(getSocket(), fileName) &&
        SocketHelper::writeInt(getSocket(), lineNumber),
        wxT("Debugger AddBreakPoint"));
}

bool LuaDebuggerPlugin::removeBreakPoint(const String &fileName, int lineNumber)
{
    if (state == Debugger_Stoped)
        return true;

    return checkSocketConnected(true, wxT("Debugger RemoveBreakPoint")) && checkSocketWrite(
        SocketHelper::writeCmd(getSocket(), LUA_DEBUGGER_CMD_REMOVE_BREAKPOINT) &&
        SocketHelper::writeString(getSocket(), fileName) &&
        SocketHelper::writeInt(getSocket(), lineNumber),
        wxT("Debugger RemoveBreakPoint"));
}

bool LuaDebuggerPlugin::clearAllBreakPoints()
{
    return checkSocketConnected(true, wxT("Debugger ClearAllBreakPoints")) && checkSocketWrite(
        SocketHelper::writeCmd(getSocket(), LUA_DEBUGGER_CMD_CLEAR_ALL_BREAKPOINTS),
        wxT("Debugger ClearAllBreakPoints"));
}

long LuaDebuggerPlugin::startDebuggee()
{
    if (debuggeeProcess == NULL)
    {
        Project* prj = Manager::getInstancePtr()->getExplorerManager()->getProjectExplorer()->getActiveProject();

        wxCHECK_MSG(prj != NULL, false, wxT("Cannot find active project"));

        // Note : the LuaDebuggerPlugin is not the event handler for process end event
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

bool LuaDebuggerPlugin::startDebugger()
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

bool LuaDebuggerPlugin::stopDebugger()
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

bool LuaDebuggerPlugin::killDebuggee()
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

void LuaDebuggerPlugin::threadFunction()
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
int LuaDebuggerPlugin::handleDebuggeeEvent(int event_type)
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

bool LuaDebuggerPlugin::checkSocketConnected(bool sendEvent, const wxString& msg)
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

bool LuaDebuggerPlugin::checkSocketRead(bool isOk, const wxString& msg)
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

bool LuaDebuggerPlugin::checkSocketWrite(bool isOk, const wxString& msg)
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

wxString LuaDebuggerPlugin::getSocketErrorMsg()
{
    String s;

    if (serverSocket)
        s += serverSocket->getErrorMsg(true);
    if (acceptedSocket)
        s += acceptedSocket->getErrorMsg(true);

    return wxString::FromUTF8(s.c_str());
}

void LuaDebuggerPlugin::clearEditorDebugMark()
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

void LuaDebuggerPlugin::syncEditor(const String& fileName, int lineNumber)
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

void LuaDebuggerPlugin::setCurrStackLevel(int stackLevel)
{
    if (stackLevel != currStackLevel)
    {
        currStackLevel = stackLevel;
        watchWindow->redraw();
    }
}

int LuaDebuggerPlugin::getCurrStackLevel()
{
    return currStackLevel;
}

void LuaDebuggerPlugin::onLuaDebugDebuggeeConnected(LuaDebuggerEvent& event)
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

void LuaDebuggerPlugin::onLuaDebugDebuggeeDisconnected(LuaDebuggerEvent& event)
{
    outputWindow->append(wxString::FromUTF8(event.strMessage.c_str()), Log_Error);
}

void LuaDebuggerPlugin::onLuaDebugExit(LuaDebuggerEvent& event)
{
    outputWindow->append(wxString::FromUTF8(event.strMessage.c_str()), Log_Info);

    stopDebugger();

    state = Debugger_Stoped;

    clearEditorDebugMark();

    restoreLayout();
}

void LuaDebuggerPlugin::onLuaDebugBreak(LuaDebuggerEvent& event)
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

void LuaDebuggerPlugin::onLuaDebugPrint(LuaDebuggerEvent& event)
{
    outputWindow->append(wxString::FromUTF8(event.strMessage.c_str()), Log_Print);
}

void LuaDebuggerPlugin::onLuaDebugError(LuaDebuggerEvent& event)
{
    outputWindow->append(wxString::FromUTF8(event.strMessage.c_str()), Log_Error);
}

void LuaDebuggerPlugin::onLuaDebugStackEnum(LuaDebuggerEvent& event)
{
    callStackWindow->setCallStackData(event.debugData);
}

void LuaDebuggerPlugin::onLuaDebugStackEntryEnum(LuaDebuggerEvent& event)
{
    if (event.stackRef == getCurrStackLevel())
    {
        localWindow->updateLocalData(event.debugData);
    }
}

void LuaDebuggerPlugin::onLuaDebugEvaluateExpr(LuaDebuggerEvent& event)
{
    watchWindow->updateWatchData(event.debugData);
}

void LuaDebuggerPlugin::onLuaDebugTableEnum(LuaDebuggerEvent& event)
{
    watchWindow->updateTableData(event.strMessage, event.debugData);
    localWindow->updateTableData(event.strMessage, event.debugData);
}

void LuaDebuggerPlugin::switchToDebugLayout()
{
    EditorEvent queryEvent(editorEVT_LAYOUT_QUERY);
    Manager::getInstancePtr()->processEvent(queryEvent);
    previousLayout = queryEvent.strData;

    EditorEvent switchEvent(editorEVT_LAYOUT_SWITCH);
    switchEvent.strData = config->getLayout();
    Manager::getInstancePtr()->processEvent(switchEvent);
}

void LuaDebuggerPlugin::restoreLayout()
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
