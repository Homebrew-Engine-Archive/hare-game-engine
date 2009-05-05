//***************************************************************
//  File:    UVRCMIMEHandler.h
//  Data:    04/15/2009
//  Author:  littlesome (littlesome@live.cn)
//-------------------------------------------------------------
//  
//-------------------------------------------------------------
//  This file is part of Hare2D Game Engine.
//  Copyright (C) All Rights Reserved
//***************************************************************
// 
//***************************************************************
#ifndef UVRCMIMEHANDLER_H
#define UVRCMIMEHANDLER_H

#include "UVEditorPage.h"

class UVRCMIMEHandler;

class UVRCEditorPage : public UVEditorPage 
{
public:
    UVRCEditorPage(wxWindow* parent, UVRCMIMEHandler* handler);
    virtual ~UVRCEditorPage(); 

    virtual void setModified(bool modified);
    virtual bool getIsModified() const { return isModified; }
    virtual bool save();
    bool saveAs();
    void updateTitle();

    void setUVState(UVEditorState* state);

private:
    void onRightClick(wxMouseEvent& event);
    void onMenuEvent(wxCommandEvent& event);

private:
    virtual bool Show(bool show = true);

    UVRCMIMEHandler* mime;
    bool isModified;

    DECLARE_ABSTRACT_CLASS(UVRCEditorPage)
    DECLARE_EVENT_TABLE()
};

class UVRCMIMEHandler : public MIMEHandlerPlugin
{
public:
    UVRCMIMEHandler();

    virtual bool canHandle(const wxString& filename) const;
    virtual bool openFile(const wxString& filename);
};

#endif