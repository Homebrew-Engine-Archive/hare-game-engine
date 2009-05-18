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

WX_PG_DECLARE_VARIANT_DATA(wxPGVariantDataPointF, PointF, WXDLLIMPEXP_PG)
WX_PG_DECLARE_VARIANT_DATA(wxPGVariantDataSizeF, SizeF, WXDLLIMPEXP_PG)

namespace hare
{
    // --------------------------------------------------------
    // PropertyGridPage
    // --------------------------------------------------------
    class PropertyGridPage : public wxPropertyGridPage
    {
    public:
        PropertyGridPage(EditorPage* page) : editorPage(page) {};
        Object::Ptr object;
        Attribute::List attributes;

        EditorPage* editorPage;

    protected:
        void onPropertySelect(wxPropertyGridEvent& event);
        void onPropertyChanging(wxPropertyGridEvent& event);
        void onPropertyChange(wxPropertyGridEvent& event);
        void onPageChange(wxPropertyGridEvent& event);

        void notifyObjectEdited(Object* object, Attribute* attr);

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
            : wxEnumProperty(label, name), currentSelection(-1)
        {
        }
    public:
        std::vector<ClassInfo*> subClasses;
        int currentSelection;
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

    class PointFProperty : public wxPGProperty
    {
        WX_PG_DECLARE_PROPERTY_CLASS(PointFProperty)
    public:
        PointFProperty(const wxString& label = wxPG_LABEL, const wxString& name = wxPG_LABEL,
            const PointF& value = PointF(0, 0));
        virtual ~PointFProperty();

        WX_PG_DECLARE_PARENTAL_METHODS()
        WX_PG_DECLARE_BASIC_TYPE_METHODS()
    protected:
        void SetValueI(const PointF& value)
        {
            m_value = PointFToVariant(value);
        }
    };

    class SizeFProperty : public wxPGProperty
    {
        WX_PG_DECLARE_PROPERTY_CLASS(SizeFProperty)
    public:
        SizeFProperty(const wxString& label = wxPG_LABEL, const wxString& name = wxPG_LABEL,
            const SizeF& value = SizeF(0, 0));
        virtual ~SizeFProperty();

        WX_PG_DECLARE_PARENTAL_METHODS()
        WX_PG_DECLARE_BASIC_TYPE_METHODS()
    protected:
        void SetValueI(const SizeF& value)
        {
            m_value = SizeFToVariant(value);
        }
    };

    void setPropertyGridObject(PropertyGridPage* page, Object* obj);
}

#endif
