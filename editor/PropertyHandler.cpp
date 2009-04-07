//***************************************************************
//  File:    PropertyHandler.cpp
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
#include "PCH.h"
#include "PropertyHandler.h"
#include "FileSystemExplorer.h"
#include <wx/propgrid/propdev.h>
#include <wx/propgrid/manager.h>
#include <wx/propgrid/propgrid.h>

namespace hare_editor
{
    IMPLEMENT_DYNAMIC_CLASS(ObjectEnumProperty, wxEnumProperty)

    //-----------------------------------------------------------------------------------
    // RGBAColourProperty
    //-----------------------------------------------------------------------------------
    RGBAColourProperty::RGBAColourProperty(const wxString& label, const wxString& name,
        const wxColour& value) : wxColourProperty(label, name, value)
    {
        AddChild(new wxIntProperty(wxT("R"), wxPG_LABEL, value.Red()));
        AddChild(new wxIntProperty(wxT("G"), wxPG_LABEL, value.Green()));
        AddChild(new wxIntProperty(wxT("B"), wxPG_LABEL, value.Blue()));
        AddChild(new wxIntProperty(wxT("A"), wxPG_LABEL, value.Alpha()));
    }

    RGBAColourProperty::~RGBAColourProperty()
    {
    }

    void RGBAColourProperty::RefreshChildren()
    {
        if (!GetCount()) return;
        wxColour colour;
        colour << m_value;
        Item(0)->SetValue(colour.Red());
        Item(1)->SetValue(colour.Green());
        Item(2)->SetValue(colour.Blue());
        Item(3)->SetValue(colour.Alpha());
    }

    void RGBAColourProperty::ChildChanged(wxVariant& thisValue, int childIndex, wxVariant& childValue) const
    {
        wxColour colour;
        colour << thisValue;
        long newColour = childValue.GetLong();
        unsigned char R = colour.Red();
        unsigned char G = colour.Green();
        unsigned char B = colour.Blue();
        unsigned char A = colour.Alpha();
        switch (childIndex)
        {
        case 0: R = newColour; break;
        case 1: G = newColour; break;
        case 2: B = newColour; break;
        case 3: A = newColour; break;
        }
        colour.Set(R, G, B, A);
        thisValue << colour;
    }

    //-----------------------------------------------------------------------------------
    // FSUrlProperty
    //-----------------------------------------------------------------------------------
    WX_PG_IMPLEMENT_DERIVED_PROPERTY_CLASS(FSUrlProperty, wxLongStringProperty, const wxString&)

    FSUrlProperty::FSUrlProperty(const wxString& label, const wxString& name, const wxString& value)
        : wxLongStringProperty(label, name, value)
    {
        m_flags |= wxPG_NO_ESCAPE;
    }

    FSUrlProperty::~FSUrlProperty()
    {
    }

    bool FSUrlProperty::OnButtonClick(wxPropertyGrid* propGrid, wxString& value)
    {
        wxSize size(300, 400);

        FileSystemDialog dlg(propGrid, _("FileSystem"), value,
            wxDEFAULT_DIALOG_STYLE | wxRESIZE_BORDER,
            propGrid->GetGoodEditorDialogPosition(this, size),
            size);

        if (dlg.ShowModal() == wxID_OK)
        {
            value = dlg.GetPath();
            return true;
        }
        return false;
    }

    void bindAttribute(Attribute* attr, PropertyGridPage* page, wxPGProperty* parent);

    template <typename T>
    void doModifyMeta(Attribute* attr, wxVariant& val)
    {
        T value;
        if (attr->hasFlag(Object::propColor))
        {
            wxColour colour;
            colour << val;
            unsigned char R = colour.Red();
            unsigned char G = colour.Green();
            unsigned char B = colour.Blue();
            unsigned char A = colour.Alpha();

            value = (A << 24) | (R << 16) | (G <<  8) | B;

            T* oldData = (T*)attr->data;
            *oldData = (T)value;
            attr->owner->postEdited(attr);
            //
        }
        else
        {
            value = wxPGVariantToInt(val);
            T* oldData = (T*)attr->data;
            *oldData = (T)value;
            attr->owner->postEdited(attr);
            val = (long)*oldData;
        }

    }

    template <>
    void doModifyMeta<String>(Attribute* attr, wxVariant& val)
    {
        if (val.GetType() == wxT("string"))
        {
            String* oldData = (String*)attr->data;
            *oldData = val.GetString().ToUTF8().data();
            attr->owner->postEdited(attr);
            val = wxString::FromUTF8(oldData->c_str());
        }
    }

    template <>
    void doModifyMeta<f32>(Attribute* attr, wxVariant& val)
    {
        double value = 0.0;
        if (wxPGVariantToDouble(val, &value))
        {
            f32* oldData = (f32*)attr->data;
            *oldData = (f32)value;
            attr->owner->postEdited(attr);
            val = (double)*oldData;
        }
    }

    template <>
    void doModifyMeta<f64>(Attribute* attr, wxVariant& val)
    {
        double value = 0.0;
        if (wxPGVariantToDouble(val, &value))
        {
            f64* oldData = (f64*)attr->data;
            *oldData = (f64)value;
            attr->owner->postEdited(attr);
            val = (double)*oldData;
        }
    }

    /*template <typename T>
    void doModifyMetaArray(Attribute* attr, wxVariant& val)
    {
        wxArrayString arr = val.GetArrayString();
        std::vector<T>& oldData = *(std::vector<T>*)attr->data;
        oldData.resize(arr.GetCount());
        for (size_t i = 0; i < arr.GetCount(); ++i)
        {
            String strVal = arr[i].ToUTF8().data();
            StringConverter::parse(strVal, oldData[i]);
            String str = StringConverter::toString(oldData[i]);
            arr[i] = wxString::FromUTF8(str.c_str());
        }
        val = arr;
    }*/

    template <typename T>
    wxPGProperty* doBindEnum(const T& value, Attribute* attr, PropertyGridPage* page, wxPGProperty* parent)
    {
        wxPGProperty* prop = 0;
        wxPGChoices choices;
        EnumMap::iterator it = attr->enumMap->begin();
        for (; it != attr->enumMap->end(); ++it)
        {
            choices.Add(wxString::FromUTF8(it->first.c_str()), it->second);
        }
        if (attr->hasFlag(Object::propEnumIsFlag))
        {
            prop = page->AppendIn(parent, new wxFlagsProperty(wxString::FromUTF8(attr->name), wxPG_LABEL,
                choices, (long)value));
        }
        else
        {
            prop = page->AppendIn(parent, new wxEnumProperty(wxString::FromUTF8(attr->name), wxPG_LABEL,
                choices, (long)value));
        }
        prop->SetHelpString(wxT("[enum]"));
        prop->SetClientData(attr);
        return prop;
    }

    template <typename T>
    void doBindMeta(Attribute* attr, PropertyGridPage* page, wxPGProperty* parent)
    {
        T* value = (T*)attr->data;
        wxPGProperty* prop = 0;

        if (attr->enumMap)
        {
            prop = doBindEnum(*value, attr, page, parent);
        }
        else
        {
            prop = page->AppendIn(parent, new wxIntProperty(wxString::FromUTF8(attr->name), wxPG_LABEL,
                *value));
            prop->SetAttribute(wxPG_ATTR_MIN, std::numeric_limits<T>::min());
            prop->SetAttribute(wxPG_ATTR_MAX, std::numeric_limits<T>::max());
            prop->SetClientData(attr);
            prop->SetHelpString(wxString::Format(wxT("[%s]"), wxString::FromUTF8(attr->typeName).c_str()));
        }

        if (attr->hasFlag(Object::propReadOnly))
            prop->SetFlag(wxPG_PROP_READONLY | wxPG_PROP_DISABLED);
    }

    template <>
    void doBindMeta<String>(Attribute* attr, PropertyGridPage* page, wxPGProperty* parent)
    {
        String* value = (String*)attr->data;

        wxPGProperty* prop = 0;

        if (attr->hasFlag(Object::propFSUrl))
        {
            prop = page->AppendIn(parent, new FSUrlProperty(wxString::FromUTF8(attr->name), wxPG_LABEL,
                wxString::FromUTF8(value->c_str())));
            prop->SetFlag(wxPG_PROP_NOEDITOR);
        }
        else
        {
            prop = page->AppendIn(parent, new wxStringProperty(wxString::FromUTF8(attr->name), wxPG_LABEL,
                wxString::FromUTF8(value->c_str())));
        }
        prop->SetClientData(attr);
        prop->SetHelpString(wxT("[String]"));
        if (attr->hasFlag(Object::propReadOnly))
            prop->SetFlag(wxPG_PROP_READONLY | wxPG_PROP_DISABLED);
    }

    template <>
    void doBindMeta<f32>(Attribute* attr, PropertyGridPage* page, wxPGProperty* parent)
    {
        f32* value = (f32*)attr->data;
        wxPGProperty* prop = page->AppendIn(parent, new wxFloatProperty(wxString::FromUTF8(attr->name), wxPG_LABEL,
            *value));
        prop->SetClientData(attr);
        prop->SetHelpString(wxT("[f32]"));
        if (attr->hasFlag(Object::propReadOnly))
            prop->SetFlag(wxPG_PROP_READONLY | wxPG_PROP_DISABLED);
    }

    template <>
    void doBindMeta<f64>(Attribute* attr, PropertyGridPage* page, wxPGProperty* parent)
    {
        f64* value = (f64*)attr->data;
        wxPGProperty* prop = page->AppendIn(parent, new wxFloatProperty(wxString::FromUTF8(attr->name), wxPG_LABEL,
            *value));
        prop->SetClientData(attr);
        prop->SetHelpString(wxT("[f64]"));
        if (attr->hasFlag(Object::propReadOnly))
            prop->SetFlag(wxPG_PROP_READONLY | wxPG_PROP_DISABLED);
    }

    // unsigned int does not work with wxVariant ;(
    template <>
    void doBindMeta<u32>(Attribute* attr, PropertyGridPage* page, wxPGProperty* parent)
    {
        u32* value = (u32*)attr->data;
        wxPGProperty* prop = 0;
        if (attr->enumMap)
        {
            prop = doBindEnum(*value, attr, page, parent);
        }
        else if (attr->hasFlag(Object::propColor))
        {
            u32 col = *value;
            unsigned char A = (col>>24) & 0xFF;
            unsigned char R = (col>>16) & 0xFF;
            unsigned char G = (col>>8) & 0xFF;
            unsigned char B = col & 0xFF;
            wxColour colour(R, G, B, A);
            prop = page->AppendIn(parent, new RGBAColourProperty(wxString::FromUTF8(attr->name), wxPG_LABEL,
                colour));
            prop->SetClientData(attr);
            prop->SetHelpString(wxT("[Color]"));
        }
        else
        {
            prop = page->AppendIn(parent, new wxUIntProperty(wxString::FromUTF8(attr->name), wxPG_LABEL,
                *value));
            prop->SetClientData(attr);
            prop->SetHelpString(wxT("[u32]"));
        }
        if (attr->hasFlag(Object::propReadOnly))
            prop->SetFlag(wxPG_PROP_READONLY | wxPG_PROP_DISABLED);
    }

    /*template <typename T>
    void doBindMetaArray(Attribute* attr, PropertyGridPage* page, wxPGProperty* parent)
    {
        std::vector<T>* val = (std::vector<T>*)attr->data;
        wxArrayString arr;
        for (size_t i = 0; i < val->size(); ++i)
        {
            String strVal = StringConverter::toString((*val)[i]);
            arr.Add(wxString::FromUTF8(strVal.c_str()));
        }
        wxPGProperty* prop = page->AppendIn(parent, new wxArrayStringProperty(wxString::FromUTF8(attr->name),
            wxPG_LABEL, arr));
        prop->SetClientData(attr);
        prop->SetHelpString(wxString::Format(wxT("[Array:%s]"), wxString::FromUTF8(attr->typeName).c_str()));
        if (attr->hasFlag(Object::propReadOnly))
            prop->SetFlag(wxPG_PROP_READONLY | wxPG_PROP_DISABLED);
    }*/

    void reBindObject(Attribute* attr, ObjectEnumProperty* prop)
    {
        prop->Empty();

        wxPGChoices choices;
        long selection = -1;
        attr->classInfo->findSubs(prop->subClasses);
        size_t i = 0;
        for (; i != prop->subClasses.size(); ++i)
        {
            choices.Add(wxString::FromUTF8(prop->subClasses[i]->className), i);
            if (prop->subClasses[i] == attr->classInfo)
                selection = i;
        }
        prop->SetChoices(choices);
        if (selection != -1)
            prop->SetValue(selection);
        wxString className = wxString::FromUTF8(attr->classInfo->className);
        prop->SetHelpString(className);
        prop->SetClientData(attr);

        // for referenced object, we should display the URL after its class name.
        Object* obj = *(Object**)attr->data;
        if (obj && !obj->getUrl().empty())
        {
            wxString lable = className + wxT(" [") + wxString::FromUTF8(obj->getUrl().c_str()) + wxT("]");
            int newID = prop->AppendChoice(lable, i);
            prop->SetValue((long)newID);
        }
        if (attr->hasFlag(Object::propReadOnly))
            prop->SetFlag(wxPG_PROP_READONLY | wxPG_PROP_DISABLED);
    }

    void doBindObject(Attribute* attr, PropertyGridPage* page, wxPGProperty* parent)
    {
        ObjectEnumProperty* prop = new ObjectEnumProperty(wxString::FromUTF8(attr->name), wxPG_LABEL);

        page->AppendIn(parent, prop);

        reBindObject(attr, prop);

        Object* obj = *(Object**)attr->data;

        if (obj)
        {
            AttVisitor v;
            v.owner = obj;
            obj->accept(v);
            prop->object = obj;
            prop->attributes = v.attributes;

            Attribute::List::iterator it = v.attributes.begin();
            for (; it != v.attributes.end(); ++it)
            {
                Attribute *at = *it;
                bindAttribute(at, page, prop);
            }
        }
    }

    BEGIN_EVENT_TABLE(PropertyGridPage, wxPropertyGridPage)
        EVT_PG_SELECTED(wxID_ANY, PropertyGridPage::onPropertySelect)
        EVT_PG_CHANGING(wxID_ANY, PropertyGridPage::onPropertyChanging)
        EVT_PG_CHANGED(wxID_ANY, PropertyGridPage::onPropertyChange)
        EVT_PG_PAGE_CHANGED(wxID_ANY, PropertyGridPage::onPageChange)
    END_EVENT_TABLE()

    void PropertyGridPage::onPropertySelect(wxPropertyGridEvent& event)
    {

    }
    void PropertyGridPage::onPropertyChanging(wxPropertyGridEvent& event)
    {

    }
    void PropertyGridPage::onPropertyChange(wxPropertyGridEvent& event)
    {
        wxPGProperty* p = event.GetProperty();

        if (p->IsCategory())
            return;
        Attribute* attr = (Attribute*)p->GetClientData();
        if (!attr)
            return;

        wxVariant value = event.GetPropertyValue();

        switch (attr->attrType)
        {
        case Attribute::attrMeta:
            {
                if (attr->typeName == String("String"))
                    doModifyMeta<String>(attr, value);
                else if (attr->typeName == String("u8"))
                    doModifyMeta<u8>(attr, value);
                else if (attr->typeName == String("s8"))
                    doModifyMeta<s8>(attr, value);
                else if (attr->typeName == String("u16"))
                    doModifyMeta<u16>(attr, value);
                else if (attr->typeName == String("s16"))
                    doModifyMeta<s16>(attr, value);
                else if (attr->typeName == String("u32"))
                    doModifyMeta<u32>(attr, value);
                else if (attr->typeName == String("s32"))
                    doModifyMeta<s32>(attr, value);
                else if (attr->typeName == String("f32"))
                    doModifyMeta<f32>(attr, value);
                else if (attr->typeName == String("f64"))
                    doModifyMeta<f64>(attr, value);
                else
                    assert(false);

                p->SetValue(value);
                p->RefreshEditor();
            }
            break;
        case Attribute::attrMetaArray:
            {
            }
            break;
        case Attribute::attrObject:
            {
            }
            break;
        case Attribute::attrObjectArray:
        case Attribute::attrObjectList:
            {
            }
            break;
        }

        // notify each parent that we are modified
        Object* owner = attr->owner;
        wxPGProperty* parent = p->GetParent();
        while (parent)
        {
            if (wxIsKindOf(parent, ObjectEnumProperty))
            {
                ObjectEnumProperty* node = (ObjectEnumProperty*)parent;
                assert(node->object == owner);

                Attribute* at = (Attribute*)node->GetClientData();
                owner = at->owner;
                owner->postEdited(at);
            }

            parent = parent->GetParent();
        }


    }

    void PropertyGridPage::onPageChange(wxPropertyGridEvent& event)
    {

    }

    void bindAttribute(Attribute* attr, PropertyGridPage* page, wxPGProperty* parent)
    {
        if (attr->hasFlag(Object::propHide))
            return;

        switch (attr->attrType)
        {
        case Attribute::attrMeta:
            {
                if (attr->typeName == String("String"))
                    doBindMeta<String>(attr, page, parent);
                else if (attr->typeName == String("u8"))
                    doBindMeta<u8>(attr, page, parent);
                else if (attr->typeName == String("s8"))
                    doBindMeta<s8>(attr, page, parent);
                else if (attr->typeName == String("u16"))
                    doBindMeta<u16>(attr, page, parent);
                else if (attr->typeName == String("s16"))
                    doBindMeta<s16>(attr, page, parent);
                else if (attr->typeName == String("u32"))
                    doBindMeta<u32>(attr, page, parent);
                else if (attr->typeName == String("s32"))
                    doBindMeta<s32>(attr, page, parent);
                else if (attr->typeName == String("f32"))
                    doBindMeta<f32>(attr, page, parent);
                else if (attr->typeName == String("f64"))
                    doBindMeta<f64>(attr, page, parent);
            }
            break;
        case Attribute::attrMetaArray:
            {
            }
            break;
        case Attribute::attrObject:
            {
                doBindObject(attr, page, parent);
            }
            break;
        case Attribute::attrObjectArray:
        case Attribute::attrObjectList:
            {

            }
            break;
        }
    }

    void setPropertyGridObject(PropertyGridPage* page, Object* obj)
    {
        page->Clear();

        if (!obj)
            return;

        wxPGProperty* parent = page->Append(new wxPropertyCategory(
            wxString::FromUTF8(obj->getClassInfo()->className), wxPG_LABEL));

        AttVisitor v;
        v.owner = obj;
        obj->accept(v);
        page->SetClientData(obj);
        page->object = obj;
        page->attributes = v.attributes;

        Attribute::List::iterator it = v.attributes.begin();
        for (; it != v.attributes.end(); ++it)
        {
            Attribute *attr = *it;
            bindAttribute(attr, page, parent);
        }
    }
}
