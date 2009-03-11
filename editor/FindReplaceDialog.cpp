//***************************************************************
//  File:    FindReplaceDialog.cpp
//  Data:    02/28/2009
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
#include "FindReplaceDialog.h"
#include <wx/xrc/xmlres.h>

namespace hare_editor
{
    BEGIN_EVENT_TABLE(FindReplaceDialog, wxDialog)
        EVT_BUTTON(wxID_OK, FindReplaceDialog::onFindNext)
    END_EVENT_TABLE()

    FindReplaceDialog::FindReplaceDialog(wxWindow* parent)
    {
        wxXmlResource::Get()->LoadDialog(this, parent, wxT("dlgFindReplace"));
    }

    void FindReplaceDialog::onFindNext(wxCommandEvent& event)
    {
        
    }


}