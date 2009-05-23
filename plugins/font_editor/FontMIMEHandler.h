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

class FontSceneListenerText : public SceneListener
{
    HARE_DECLARE_PTR(FontSceneListenerText)
public:
    FontSceneListenerText() : font(0) {}

    virtual void renderScene()
    {
        if (!font)
            return;
        Font::Ptr old_font = getCanvas()->getFont();
        getCanvas()->setFont(font);
        getCanvas()->drawText(100, 100, text);
        getCanvas()->setFont(old_font);
    }
public:
    Font* font;
    String text;
};

class FontSceneListenerCache : public SceneListener
{
    HARE_DECLARE_PTR(FontSceneListenerCache)
public:
    FontSceneListenerCache() : font(0) 
    {
        shader = new SimpleShader();
        mtrl = new TextureMtrl();
        shader->setMaterial(mtrl);
    }

    virtual void renderScene()
    {
        if (!font)
            return;

        mtrl->setTexture(font->getFontTexture());
        shader->setShaderParams(font->getFontExtParams());
        getCanvas()->drawImage(0, 0, shader);
    }
public:
    Font* font;
    SimpleShader::Ptr shader;
    TextureMtrl::Ptr mtrl;
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

    FontSceneListenerText::Ptr txtListener;
    FontSceneListenerCache::Ptr cacheListener;

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