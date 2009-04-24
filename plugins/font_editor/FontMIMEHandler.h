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
        if (!font)
            return;

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
    FontSceneListenerCache() : font(0) {}

    virtual void beginScene(){}
    virtual void endScene(){}
    virtual void renderScene()
    {
        if (!font)
            return;

        SimpleShader::Ptr shader = new SimpleShader();
        TextureMtrl* texMtrl = new TextureMtrl();
        texMtrl->setTexture(font->getFontTexture());
        shader->setShaderParams(font->getFontExtParams());
        shader->setMaterial(texMtrl);

        getCanvas()->drawImage(0, 0, shader);
    }
public:
    Font* font;
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
    wxHareCanvas* canvasText;
    wxHareCanvas* canvasCache;
    wxTextCtrl* txtSample;

    SceneManager* sceneText;
    SceneManager* sceneCache;
    FontSceneListenerText txtListener;
    FontSceneListenerCache cacheListener;

    Font::Ptr fontPtr;

    FontMIMEHandler* mime;

private:
    void onTextUpdate(wxCommandEvent& event);
    void onSize(wxSizeEvent& event);

    bool Show(bool show = true);

    DECLARE_ABSTRACT_CLASS(FontEditorPage)
};

class FontMIMEHandler : public MIMEHandlerPlugin
{
public:
    FontMIMEHandler();

    virtual bool canHandle(const wxString& filename) const;
    virtual bool openFile(const wxString& filename);

private:

    friend class FontEditorPage;

    FontEditorPage* page;
};

#endif