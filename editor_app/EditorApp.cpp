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
            locale.Init(info->Language);
            
            wxString fullPath = Manager::getInstancePtr()->getAppDir() + wxT("/editor_data/locale/");
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

    FileSystem* fs = FileSystem::getSingletonPtr();

    // editor's resources and write dir
    String editorDir = fs->getBaseDir() + String("editor_data/");
    wxFileName fn(wxString::FromUTF8(editorDir.c_str()));
    fn.Normalize();
    editorDir = fn.GetFullPath().ToUTF8().data();
    fs->setWriteDir(editorDir);
    fs->addSearchPath(editorDir);

    if (!wxApp::OnInit())
        return false;

    InitLocale();

    EditorFrame* frame = new EditorFrame(0, wxT("Editor"));
    frame->Show(true);

    return true;
}

int EditorApp::OnExit()
{
    Manager::free();

    core_quit();

    return wxApp::OnExit();
}
