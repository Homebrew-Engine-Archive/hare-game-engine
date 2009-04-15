//***************************************************************
//  File:    FindReplaceDialog.h
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
#ifndef FINDREPLACEPANEL_H
#define FINDREPLACEPANEL_H

#include "EditorPrerequisites.h"
#include <wx/dialog.h>

namespace hare
{
    class EDITOR_API FindReplaceDialog : public wxDialog
    {
    public:
        FindReplaceDialog(wxWindow* parent);

    private:
        void onFindNext(wxCommandEvent& event);
        DECLARE_EVENT_TABLE();
    };
}

#endif
