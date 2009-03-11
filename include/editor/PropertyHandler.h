//***************************************************************
//  File:    PropertyHandler.h
//  Date:    02/05/2009
//  Author:  littlesome (littlesome@live.cn)
//-------------------------------------------------------------
//
//-------------------------------------------------------------
//  This file is part of Hare2D Game Engine.
//  Copyright (C) All Rights Reserved
//***************************************************************
//
//***************************************************************
#ifndef PROPERTYHANDLER_H
#define PROPERTYHANDLER_H

#include "EditorPrerequisites.h"
#include <wx/propgrid/manager.h>

namespace hare_editor
{
    class PropertyGridPage : public wxPropertyGridPage
    {
    public:
        Object::Ptr object;
        Attribute::List attributes;

    protected:
        void onPropertySelect(wxPropertyGridEvent& event);
        void onPropertyChanging(wxPropertyGridEvent& event);
        void onPropertyChange(wxPropertyGridEvent& event);
        void onPageChange(wxPropertyGridEvent& event);
    
    private:
        DECLARE_EVENT_TABLE()
    };

    class ObjectEnumProperty : public wxEnumProperty
    {
    public:
        ObjectEnumProperty(const wxString& label = wxPG_LABEL, const wxString& name = wxPG_LABEL)
            : wxEnumProperty(label, name)
        {
        }
    public:
        std::vector<ClassInfo*> subClasses;
        Object::Ptr object;
        Attribute::List attributes;
    };
    
    void setPropertyGridObject(PropertyGridPage* page, Object* obj);
}

#endif