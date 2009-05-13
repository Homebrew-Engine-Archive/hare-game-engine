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
    UVEditorState::Ptr obj;
};

void searchUVRC(const String& basePath, wxListBox* listBox)
{
    FileSystem* fs = FileSystem::getSingletonPtr();
    StringVector files = fs->enumFiles(basePath);
    for (size_t i = 0; i < files.size(); ++i){
        String fullFileName = basePath + files[i];
        if (fs->isDir(fullFileName)){
            searchUVRC(fullFileName + "/", listBox);
        }else{
            String baseName;
            String ext;
            StringUtil::splitFilename(fullFileName, baseName, ext);
            StringUtil::toLowerCase(ext);
            if (ext == "uvrc"){
                UVEditorState* obj = (UVEditorState*)Object::importObject(fullFileName.c_str());
                if (obj){
                    ImageSpriteClientData* data = new ImageSpriteClientData;
                    data->obj = obj;
                    wxString item = wxString::FromUTF8((basePath + files[i]).c_str());
                    listBox->Insert(item, 0, data);            
                }
            }
        } 
    }
}


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
            uvrcFilesCtrl->SetWindowStyle(wxLB_SINGLE|wxNO_BORDER);
            boxSizer2->Add(uvrcFilesCtrl, 0, wxBOTTOM|wxLEFT|wxRIGHT|wxEXPAND|wxALIGN_LEFT|wxALIGN_TOP, 8);
        }
        {
            wxStaticText* staticText = new wxStaticText(this, wxID_ANY, _("Rect name:"));
            boxSizer2->Add(staticText, 0, wxTOP|wxLEFT|wxRIGHT|wxEXPAND|wxALIGN_LEFT|wxALIGN_TOP, 8);
            rectNamesCtrl = new wxListBox(this, wxID_ANY);
            rectNamesCtrl->SetWindowStyle(wxLB_SINGLE|wxNO_BORDER);
            boxSizer2->Add(rectNamesCtrl, 0, wxBOTTOM|wxLEFT|wxRIGHT|wxEXPAND|wxALIGN_LEFT|wxALIGN_TOP, 8);
        }

        canvas = new wxHareCanvas(this);
        
        wxBoxSizer* boxSizer3 = new wxBoxSizer(wxHORIZONTAL);
        {
            boxSizer3->Add(boxSizer2, 1, wxEXPAND, 5);
            boxSizer3->Add(canvas, 1, wxEXPAND | wxALL, 5);
            boxSizer1->Add(boxSizer3, 0, wxBOTTOM|wxLEFT|wxRIGHT|wxEXPAND|wxALIGN_LEFT|wxALIGN_TOP, 8);
        }

        SetSizer(boxSizer1);
        Layout();

        searchUVRC("/", uvrcFilesCtrl);

        if (uvrcFilesCtrl->GetCount() > 0){
            uvrcFilesCtrl->SetSelection(0, true);
            wxMouseEvent event;
            onUVRCListLDown(event);
        }

        SceneManager* scene = getHareApp()->createSceneManager();
        scene->setSceneListener(this);
        canvas->getRenderWindow()->setSceneManager(scene);

        uvrcFilesCtrl->Connect(wxEVT_COMMAND_LISTBOX_SELECTED, wxMouseEventHandler(ImageSpriteWizardPage::onUVRCListLDown), NULL, this);
        rectNamesCtrl->Connect(wxEVT_COMMAND_LISTBOX_SELECTED, wxMouseEventHandler(ImageSpriteWizardPage::onRectNameListLDown), NULL, this);
        canvas->Connect(wxEVT_SIZE, wxSizeEventHandler(ImageSpriteWizardPage::onSize), 0, this);
    }

    Material* getMaterial()
    {
        int selectID = uvrcFilesCtrl->GetSelection();
        if (selectID == -1)
            return NULL;
        ImageSpriteClientData* data = (ImageSpriteClientData*)(uvrcFilesCtrl->GetClientObject(selectID));
        if (!data)
            return NULL;

        UVEditorState::Ptr uvState = data->obj;

        return uvState->mtrl;
    }

    RectF getRectUV()
    {
        RectF retRect(0, 0, 1, 1);
        int selectID = uvrcFilesCtrl->GetSelection();
        if (selectID == -1)
            return retRect;

        ImageSpriteClientData* data = (ImageSpriteClientData*)(uvrcFilesCtrl->GetClientObject(selectID));
        if (!data)
            retRect;

        selectID = rectNamesCtrl->GetSelection();
        if (selectID == -1)
            return retRect;

        wxString rectName = rectNamesCtrl->GetString(selectID);

        UVEditorState::Ptr uvState = data->obj;
        RectState::List list = uvState->rects;
        RectState::List::iterator it = list.begin();
        for (;it != list.end(); ++it){
            RectState* rectState = *it;
            if (rectState->name == String(rectName.ToUTF8().data())){
                return rectState->rect;
            }
        }

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
        wxSize size = canvas->GetClientSize();
        int width  = size.GetWidth() > size.GetHeight() ? size.GetHeight() : size.GetWidth();
        RectF rect;
        rect.minX = rect.minY = 0;
        rect.maxX = rect.maxY = width;

        getCanvas()->drawImage(rect, mtrl, rectUV);
    }

    void onUVRCListLDown(wxMouseEvent& event)
    {
        mtrl = getMaterial();
        rectNamesCtrl->Clear();

        int selectID = uvrcFilesCtrl->GetSelection();
        if (selectID == -1)
            return;
        ImageSpriteClientData* data = (ImageSpriteClientData*)(uvrcFilesCtrl->GetClientObject(selectID));
        if (!data)
            return;

        UVEditorState::Ptr uvState = data->obj;
        RectState::List list = uvState->rects;
        RectState::List::iterator it = list.begin();
        for (;it != list.end(); ++it){
            RectState* rectState = *it;
            wxString tmp = wxString::FromUTF8(rectState->name.c_str());
            rectNamesCtrl->Insert(tmp, 0);
        }

        rectUV = getRectUV();
    }

    void onRectNameListLDown(wxMouseEvent& event)
    {
        rectUV = getRectUV();
    }

    void onSize(wxSizeEvent& event)
    {
        wxSize size = canvas->GetClientSize();
        if (size.GetWidth() > 0 && size.GetHeight() > 0)
        {
            canvas->getRenderWindow()->resize(size.GetWidth(), size.GetHeight());
        }
        event.Skip();
    }

protected:
    wxListBox* uvrcFilesCtrl;
    wxListBox* rectNamesCtrl;
    wxHareCanvas* canvas;

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
