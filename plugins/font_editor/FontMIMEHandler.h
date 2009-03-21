//***************************************************************
//  File:    FontMIMEHandler.h
//  Data:    03/20/2009
//  Author:  littlesome (littlesome@live.cn)
//-------------------------------------------------------------
//  
//-------------------------------------------------------------
//  This file is part of Hare2D Game Engine.
//  Copyright (C) All Rights Reserved
//***************************************************************
// 
//***************************************************************
#ifndef FONTMIMEHANDLER_H
#define FONTMIMEHANDLER_H

#include <wx/harecanvas.h>

class FontMIMEHandler;

class FontSceneListenerText : public SceneListenerBase
{
public:
    FontSceneListenerText() : font(0) {}

    virtual void beginScene(){}
    virtual void endScene(){}
    virtual void renderScene()
    {
        getCanvas()->setFont(font);
        getCanvas()->drawText(100, 100, text);
    }
public:
    Font* font;
    String text;
};

class FontSceneListenerCache : public SceneListenerBase
{
public:
    virtual void beginScene(){}
    virtual void endScene(){}
    virtual void renderScene()
    {
        getCanvas()->drawImage(0, 0, cacheTex);
    }
public:
    Shader::Ptr cacheTex;
};

class FontEditorPage : public EditorPage 
{
    friend class FontMIMEHandler;

private:
    FontEditorPage(wxWindow* parent, FontMIMEHandler* handler, Font* font);
    virtual ~FontEditorPage(); 

    bool changeFont(Font* font);

    bool isOk() const
    {
        return fontPtr.pointer() != NULL;
    }

private:
    wxHareCanvas* canvsText;
    wxHareCanvas* canvsCache;
    wxTextCtrl* txtSample;

    SceneManager* sceneText;
    SceneManager* sceneCache;
    FontSceneListenerText txtListener;
    FontSceneListenerCache cacheListener;

    Font::Ptr fontPtr;

    FontMIMEHandler* mime;

private:
    void onTextUpdate(wxCommandEvent& event);

    DECLARE_ABSTRACT_CLASS(FontEditorPage)
};

class FontMIMEHandler : public MIMEHandlerPlugin
{
public:
    FontMIMEHandler();

    virtual bool canHandle(const wxString& filename) const;
    virtual bool openFile(const wxString& filename);

private:
    FontEditorPage* page;
};

#endif