//***************************************************************
//  File:    ImageSpriteWizard.cpp
//  Date:    5/11/2009
//  Author:  wanglei (wanglei010407@163.com)
//-------------------------------------------------------------
//
//-------------------------------------------------------------
//  This file is part of Hare2D Game Engine.
//  Copyright (C) All Rights Reserved
//***************************************************************
//
//***************************************************************
#include "PCH.h"
#include "ImageSpriteWizard.h"
#include <wx/harecanvas.h>

class ImageSpriteClientData : public wxClientData
{
public:
    Object::Ptr obj;
};

class ImageSpriteWizardPage : public wxWizardPageSimple, public SceneListenerBase
{
public:
    ImageSpriteWizardPage(wxWizard* parent)
        :wxWizardPageSimple(parent)
    {
        wxBoxSizer* boxSizer1 = new wxBoxSizer(wxVERTICAL);
        {
            wxStaticText* staticText = new wxStaticText(this, wxID_ANY,
                _T("This wizard helps you to create ImageSprite."), wxPoint(5,5));
            boxSizer1->Add(staticText, 0, wxALL|wxEXPAND|wxALIGN_LEFT|wxALIGN_TOP, 8);
        }


        wxBoxSizer* boxSizer2 = new wxBoxSizer(wxVERTICAL);
        {
            wxStaticText* staticText = new wxStaticText(this, wxID_ANY, _("File name:"));
            boxSizer2->Add(staticText, 0, wxTOP|wxLEFT|wxRIGHT|wxEXPAND|wxALIGN_LEFT|wxALIGN_TOP, 8);
            uvrcFilesCtrl = new wxListBox(this, wxID_ANY);
            boxSizer2->Add(uvrcFilesCtrl, 0, wxBOTTOM|wxLEFT|wxRIGHT|wxEXPAND|wxALIGN_LEFT|wxALIGN_TOP, 8);
        }
        {
            wxStaticText* staticText = new wxStaticText(this, wxID_ANY, _("Rect name:"));
            boxSizer2->Add(staticText, 0, wxTOP|wxLEFT|wxRIGHT|wxEXPAND|wxALIGN_LEFT|wxALIGN_TOP, 8);
            rectNamesCtrl = new wxListBox(this, wxID_ANY);
            boxSizer2->Add(rectNamesCtrl, 0, wxBOTTOM|wxLEFT|wxRIGHT|wxEXPAND|wxALIGN_LEFT|wxALIGN_TOP, 8);
        }


        wxBoxSizer* boxSizer3 = new wxBoxSizer(wxHORIZONTAL);
        {
            boxSizer3->Add(boxSizer2, 1, wxEXPAND, 5);
            reviewPanel = new wxPanel(this, wxID_ANY);
            boxSizer3->Add(reviewPanel, 1, wxEXPAND | wxALL, 5);
            boxSizer1->Add(boxSizer3, 0, wxBOTTOM|wxLEFT|wxRIGHT|wxEXPAND|wxALIGN_LEFT|wxALIGN_TOP, 8);
        }

        SetSizer(boxSizer1);
        boxSizer1->Fit(this);
        boxSizer1->SetSizeHints(this);
        //Layout();

        wxString fileName = wxT("haha.uvrc");
        Object::Ptr obj = Object::importObject(fileName.ToUTF8().data());
        if (obj){
            ImageSpriteClientData* data = new ImageSpriteClientData;
            data->obj = obj;
            uvrcFilesCtrl->Insert(fileName, 0, data);
        }

        wxHareCanvas* canvas = new wxHareCanvas(reviewPanel);
        SceneManager* scene = getHareApp()->createSceneManager();
        scene->setSceneListener(this);
        canvas->getRenderWindow()->setSceneManager(scene);

        uvrcFilesCtrl->Connect(wxEVT_LEFT_DOWN, wxMouseEventHandler(ImageSpriteWizardPage::onUVRCListLDown), NULL, this);
        rectNamesCtrl->Connect(wxEVT_LEFT_DOWN, wxMouseEventHandler(ImageSpriteWizardPage::onRectNameListLDown), NULL, this);

    }

    Material* getMaterial()
    {
        ImageSpriteClientData* data = (ImageSpriteClientData*)(uvrcFilesCtrl->GetClientObject(uvrcFilesCtrl->GetSelection()));
        if (!data)
            return NULL;

        AttVisitor attVisitor;
        data->obj->accept(attVisitor);
        Attribute::List::iterator it = attVisitor.attributes.begin();
        for (;it != attVisitor.attributes.end(); ++it){
            Attribute* attribute = *it;
            if (attribute->attrType == Attribute::attrObject && attribute->data && attribute->classInfo){
                if (attribute->classInfo->isDerivedFrom(&Material::CLASS_INFO)){
                    return (Material*)(*(Object**)attribute->data);
                }
            }
        }

        return NULL;
    }

    RectF getRectUV()
    {
        RectF retRect(0, 0, 1, 1);
        ImageSpriteClientData* data = (ImageSpriteClientData*)(uvrcFilesCtrl->GetClientObject(uvrcFilesCtrl->GetSelection()));
        if (!data)
            retRect;

        //AttVisitor attVisitor;
        //data->obj->accept(attVisitor);
        //Attribute::List::iterator it = attVisitor.attributes.begin();
        //for (;it != attVisitor.attributes.end(); ++it){
        //    Attribute* attribute = *it;
        //    wxString className = wxString::FromUTF8(attribute->typeName);
        //    wxString tmp = wxT("RectState");
        //    if (className == tmp){

        //        return retRect;
        //    }
        //}
        return retRect;
    }

    virtual void beginScene()
    {

    }

    virtual void endScene()
    {

    }
    
    virtual void renderScene()
    {
        drawReview();
    }

    void drawReview()
    {
        int width  = 100;//reviewPanel->m_height > reviewPanel->m_width ? reviewPanel->m_width : reviewPanel->m_height;
        RectF rect;
        rect.minX = rect.minY = 0;
        rect.maxX = rect.maxY = width;

        getCanvas()->drawImage(rect, mtrl, rectUV);
    }

    void onUVRCListLDown(wxMouseEvent& event)
    {
        mtrl = getMaterial();
        rectNamesCtrl->Clear();
        rectUV = getRectUV();
    }

    void onRectNameListLDown(wxMouseEvent& event)
    {
        rectUV = getRectUV();
    }

protected:
    wxListBox* uvrcFilesCtrl;
    wxListBox* rectNamesCtrl;
    wxPanel*   reviewPanel;

    Material::Ptr mtrl;
    RectF rectUV;
};

ImageSpriteWizard::ImageSpriteWizard(wxWindow* parent, const wxString& title, wxBitmap& bitmap)
    :wxWizard(parent, wxID_ANY, title, bitmap, wxDefaultPosition, wxDEFAULT_DIALOG_STYLE | wxRESIZE_BORDER)
{
    page = new ImageSpriteWizardPage(this);
    SetPageSize(wxSize(400, 300));
}

wxWizardPageSimple* ImageSpriteWizard::GetFirstPage()
{
    return page;
}

Material::Ptr ImageSpriteWizard::getMaterial()
{
    return page->getMaterial();
}

RectF ImageSpriteWizard::getRectUV()
{
    return page->getRectUV();
}
