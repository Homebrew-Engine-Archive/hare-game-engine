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
#include <wx/propgrid/advprops.h>

namespace hare_editor
{
    // --------------------------------------------------------
    // PropertyGridPage
    // --------------------------------------------------------
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

    // --------------------------------------------------------
    // ObjectEnumProperty
    // --------------------------------------------------------
    class ObjectEnumProperty : public wxEnumProperty
    {
        DECLARE_DYNAMIC_CLASS(ObjectEnumProperty)
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

    // --------------------------------------------------------
    // FSUrlProperty
    // --------------------------------------------------------
    class FSUrlProperty : public wxLongStringProperty
    {
        DECLARE_DYNAMIC_CLASS(FSUrlProperty)
    public:
        FSUrlProperty(const wxString& label = wxPG_LABEL, const wxString& name = wxPG_LABEL,
            const wxString& value = wxEmptyString);
        virtual ~FSUrlProperty();

        virtual bool OnButtonClick(wxPropertyGrid* propGrid, wxString& value);

    protected:
        wxString m_dlgMessage;
    };

    // --------------------------------------------------------
    // RGBAColourProperty
    // --------------------------------------------------------
    class RGBAColourProperty : public wxColourProperty
    {
    public:
        RGBAColourProperty(const wxString& label = wxPG_LABEL,
            const wxString& name = wxPG_LABEL,
            const wxColour& value = *wxWHITE);

        virtual ~RGBAColourProperty();

        WX_PG_DECLARE_PARENTAL_METHODS()
    };

    void setPropertyGridObject(PropertyGridPage* page, Object* obj);
}

#endif
