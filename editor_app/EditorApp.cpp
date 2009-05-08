//***************************************************************
//  File:    EditorApp.cpp
//  Date:    10/20/2008
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
#include "EditorApp.h"
#include "EditorMain.h"
#include <wx/msgdlg.h>
#include <wx/dir.h>

#if defined(__WXMSW__) && defined(_DEBUG)
#	define  CRTDBG_MAP_ALLOC
#	include <stdlib.h>
#	include <crtdbg.h>
#endif

IMPLEMENT_APP(EditorApp);

BEGIN_EVENT_TABLE(EditorApp, wxApp)
    EVT_IDLE(EditorApp::OnIdle)
END_EVENT_TABLE()

void EditorApp::InitLocale()
{
    AppConfigFile* appConfig = Manager::getInstancePtr()->getConfigManager()->getAppConfigFile();
    if (appConfig)
    {
        wxString lang = appConfig->getLanguage();
        const wxLanguageInfo *info = 0;

        if (!lang.IsEmpty())
            info = wxLocale::FindLanguageInfo(lang);
        if (!info)
            info = wxLocale::GetLanguageInfo(wxLANGUAGE_DEFAULT);

        if (info)
        {
            Log::getSingleton().logInfo("Setup locale for editor, '%s' '%s'", 
                info->CanonicalName.ToUTF8().data(), info->Description.ToUTF8().data());

            locale.Init(info->Language);

            wxString fullPath = Manager::getInstancePtr()->getAppDir() + wxT("/locale/");
            wxLocale::AddCatalogLookupPathPrefix(fullPath);
            fullPath += info->CanonicalName + wxT("/LC_MESSAGES/");

            if (wxDir::Exists(fullPath))
            {
                wxDir dir(fullPath);
                wxString moName;

                if (dir.GetFirst(&moName, _T("*.mo"), wxDIR_FILES))
                {
                    do
                    {
                        locale.AddCatalog(moName);
                        Log::getSingleton().logInfo("Add catalog for locale : '%s'", 
                            moName.ToUTF8().data());
                    }
                    while (dir.GetNext(&moName));
                }
            }
        }
    }
}

bool EditorApp::OnInit()
{
#if defined(__WXMSW__) && defined(_DEBUG)
    _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
    //_CrtSetBreakAlloc(5648);
#endif

    wxString argv0 = argv[0];
    core_init(argv0.ToUTF8().data());

    Log::getSingleton().changeFileName("EditorLog.html");

    FileSystem* fs = FileSystem::getSingletonPtr();

    // -------------------------------------------------------------------
    // Read resource config file
    // -------------------------------------------------------------------
    ConfigFile resource;
    resource.load("resource.cfg");

    // NB: ScriptDir will not be added to search path in editor,
    //     it's used for Workspace.
    String scriptDir = resource.getSetting("ScriptDir");
    wxString workspaceDir = wxString::FromUTF8(scriptDir.c_str());

    Log::getSingleton().logInfo("ScriptDir '%s' is used for workspace in editor mode.",
        scriptDir.c_str());

    // NB: SearchPath is same as WriteDir, so we can save edited result. 
    String writeDir = resource.getSetting("WriteDir");
    fs->addSearchPath(writeDir);
    fs->setWriteDir(writeDir);

    Log::getSingleton().logInfo("Filesystem write dir '%s' is used for search path in editor mode.",
        writeDir.c_str());


    // -------------------------------------------------------------------
    // Read plugin config file
    // -------------------------------------------------------------------
    ConfigFile plugin;
    plugin.load("plugin.cfg");

    String pluginDir = plugin.getSetting("PluginDir");
    StringVector plugins = plugin.getMultiSetting("Plugin");
    
    for (size_t i = 0; i < plugins.size(); ++i)
    {
        String fileName = pluginDir + plugins[i];
        getHareApp()->loadPlugin(fileName);
        Log::getSingleton().logInfo("Load plugin : '%s'", fileName.c_str());
    }

    getHareApp()->startUp();

    if (!wxApp::OnInit())
        return false;

    InitLocale();

    EditorFrame* frame = new EditorFrame(0, wxT("Editor"), workspaceDir);
    frame->Show(true);

    return true;
}

int EditorApp::OnExit()
{
    Manager::free();

    getHareApp()->shutDown();

    getHareApp()->freeAllPlugins();

    core_quit();

    return wxApp::OnExit();
}

void EditorApp::OnIdle(wxIdleEvent& event)
{
    getHareApp()->hareRunFrame();

    event.RequestMore();
}
