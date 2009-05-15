//***************************************************************
//  File:    ThemeMIMEHandler.cpp
//  Data:    05/14/2009
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
#include "ThemeMIMEHandler.h"
#include <wx/xrc/xmlres.h>
#include <wx/splitter.h>

class ThemeItemData : public TreeItemBase
{
public:
    TreeItemType getType() { return Type_Theme; }
    Theme::Ptr theme;
    UVEditorState::Ptr uvrc;
};

class RectItemData : public TreeItemBase
{
public:
    TreeItemType getType() { return Type_Rect; }
    RectUV* rect;
    Theme::Ptr theme;
};

IMPLEMENT_ABSTRACT_CLASS(ThemeEditorPage, EditorPage)

BEGIN_EVENT_TABLE(ThemeEditorPage, EditorPage)
    EVT_TREE_SEL_CHANGED(XRCID("idTreeView"), ThemeEditorPage::onTreeItemSelected)
END_EVENT_TABLE()

ThemeEditorPage::ThemeEditorPage(wxWindow* parent, ThemeMIMEHandler* handler)
    : EditorPage(), mime(handler), currItem(0)
{
    wxXmlResource::Get()->LoadPanel(this, parent, wxT("idThemeEditorPanel"));

    // Scene for selecting uv
    {
        SceneManager* scene = getHareApp()->createSceneManager();
        wxPanel* panelB = XRCCTRL(*this, "idPanelB", wxPanel);
        canvasRect = new wxHareCanvas(panelB);
        panelB->GetSizer()->Add(canvasRect, 1, wxEXPAND, 0);
        canvasRect->getRenderWindow()->setSceneManager(scene);
        scene->setSceneListener(this);
        panelB->Connect(wxEVT_ERASE_BACKGROUND, wxEraseEventHandler(ThemeEditorPage::onEraseBackground), 0, this);
        canvasRect->Connect(wxEVT_SIZE, wxSizeEventHandler(ThemeEditorPage::onCanvasRectSize), 0, this);
    }
    
    // Scene for gui preview
    {
        SceneManager* scene = getHareApp()->createSceneManager();
        wxPanel* panelD = XRCCTRL(*this, "idPanelD", wxPanel);
        canvasGUI = new wxHareCanvas(panelD);
        panelD->GetSizer()->Add(canvasGUI, 1, wxEXPAND, 0);
        canvasGUI->getRenderWindow()->setSceneManager(scene);
        scene->setSceneListener(&previewSceneListener);
        panelD->Connect(wxEVT_ERASE_BACKGROUND, wxEraseEventHandler(ThemeEditorPage::onEraseBackground), 0, this);
        canvasGUI->Connect(wxEVT_SIZE, wxSizeEventHandler(ThemeEditorPage::onCanvasGUISize), 0, this);
    }

    treeCtrl = XRCCTRL(*this, "idTreeView", wxTreeCtrl);
    btnBrowse = XRCCTRL(*this, "idBtnBrowse", wxButton);
    txtUVRCUrl = XRCCTRL(*this, "idTextUrl", wxTextCtrl);
    listBox = XRCCTRL(*this, "idListBox", wxListBox);

    btnBrowse->Enable(false);

    btnBrowse->Connect(wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(ThemeEditorPage::onBrowse), 0, this);
    listBox->Connect(wxEVT_COMMAND_LISTBOX_SELECTED, wxCommandEventHandler(ThemeEditorPage::onListSelected), 0, this);

    wxSplitterWindow* splitterH = XRCCTRL(*this, "idSplitterH", wxSplitterWindow);
    splitterH->SetSashPosition(parent->GetSize().GetHeight() / 4);

    wxSplitterWindow* splitterAB = XRCCTRL(*this, "idSplitterAB", wxSplitterWindow);
    splitterAB->SetSashPosition(parent->GetSize().GetWidth() / 4);

    setTitle(wxT("[ThemeEditor]"));

    Layout();
}

ThemeEditorPage::~ThemeEditorPage()
{
}

void ThemeEditorPage::renderScene()
{
    if (currItem)
    {
        if (currItem->getType() == TreeItemBase::Type_Rect)
        {
            RectItemData* rectData = (RectItemData*)currItem;
            Material* mtrl = rectData->theme->getMaterial();
            RectF& uvRect = *rectData->rect;
            if (mtrl && mtrl->getTextureMtrl() && mtrl->getTextureMtrl()->getTexture())
            {
                float w = mtrl->getTextureMtrl()->getTexture()->getWidth() * uvRect.width();
                float h = mtrl->getTextureMtrl()->getTexture()->getHeight() * uvRect.height();
                getCanvas()->drawImage(RectF(0, 0, w, h), mtrl, uvRect);
            }
        }
    }
}

void ThemeEditorPage::onEraseBackground(wxEraseEvent& event)
{
}

void ThemeEditorPage::onCanvasRectSize(wxSizeEvent& event)
{
    wxSize size = canvasRect->GetClientSize();
    if (size.GetWidth() > 0 && size.GetHeight() > 0){
        canvasRect->getRenderWindow()->resize(size.GetWidth(), size.GetHeight());
    }
    event.Skip();
}

void ThemeEditorPage::onCanvasGUISize(wxSizeEvent& event)
{
    wxSize size = canvasGUI->GetClientSize();
    if (size.GetWidth() > 0 && size.GetHeight() > 0){
        canvasGUI->getRenderWindow()->resize(size.GetWidth(), size.GetHeight());
    }
    event.Skip();
}

void ThemeEditorPage::onBrowse(wxCommandEvent& event)
{
    if (currItem)
    {
        if (currItem->getType() == TreeItemBase::Type_Theme)
        {
            ThemeItemData* themeData = (ThemeItemData*)currItem;
            
            wxSize size(400, 400);
            FileSystemDialog dlg(btnBrowse, true, _("FileSystem"), wxT("/"), 0,
                wxDefaultPosition, size);

            if (dlg.ShowModal() == wxID_OK)
            {
                String objectUrl = dlg.GetPath().ToUTF8().data();

                UVEditorState::Ptr imported = (UVEditorState*)Object::importObject(objectUrl);

                if (imported && imported->isA(&UVEditorState::CLASS_INFO))
                {
                    themeData->uvrc = imported;
                    themeData->theme->setMaterial(imported->mtrl);
                    updateUVRC(themeData->uvrc);
                }
            }
        }
        else if (currItem->getType() == TreeItemBase::Type_Rect)
        {
            wxASSERT(false);
        }
    }
}

void ThemeEditorPage::onListSelected(wxCommandEvent& event)
{
    if (currItem)
    {
        if (currItem->getType() == TreeItemBase::Type_Theme)
        {
            // Do nothing, we will not arrive here.
        }
        else if (currItem->getType() == TreeItemBase::Type_Rect)
        {
            RectItemData* rectData = (RectItemData*)currItem;
            RectUV* rect = (RectUV*)listBox->GetClientData(event.GetSelection());
            *rectData->rect = *rect;
        }
    }
}

void ThemeEditorPage::updateUVRC(UVEditorState* uvrc)
{
    listBox->Clear();

    if (uvrc)
    {
        txtUVRCUrl->SetLabel(wxString::FromUTF8(uvrc->getUrl().c_str()));

        int pos = 0;

        RectState::List::iterator it = uvrc->rects.begin();
        for (; it != uvrc->rects.end(); ++it)
        {
            wxString rectName = wxString::FromUTF8((*it)->name.c_str());
            RectUV* data = &(*it)->rect;
            listBox->Insert(rectName, pos, data);
            pos++;
        }
    }
    else
    {
        txtUVRCUrl->SetLabel(wxEmptyString);
    }
}

void ThemeEditorPage::onTreeItemSelected(wxTreeEvent& event)
{
    TreeItemBase* item = (TreeItemBase*)treeCtrl->GetItemData(event.GetItem());
    currItem = item;

    if (currItem)
    {
        if (currItem->getType() == TreeItemBase::Type_Theme)
        {
            ThemeItemData* themeData = (ThemeItemData*)currItem;

            updateUVRC(themeData->uvrc);

            btnBrowse->Enable(true);
            listBox->Enable(false);
        }
        else if (currItem->getType() == TreeItemBase::Type_Rect)
        {
            listBox->SetSelection(-1);
            listBox->Enable(true);
            btnBrowse->Enable(false);
        }
    }
}

void ThemeEditorPage::setThemePackage(ThemePackage* themePackage)
{
    themes = themePackage;
    treeCtrl->DeleteAllItems();
    currItem = 0;

    previewSceneListener.getGUISystem()->setTheme(themes);

    wxTreeItemId rootId = treeCtrl->AddRoot(wxT("ThemePackage"));
    Theme::Array& arrayThemes = themes->getThemes();
    
    for (size_t i = 0; i < arrayThemes.size(); ++i)
    {
        Theme* theme = arrayThemes[i];
        wxString className = wxString::FromUTF8(theme->getClassInfo()->className);
        ThemeItemData* themeData = new ThemeItemData();
        themeData->theme = theme;
        themeData->uvrc = 0;
        wxTreeItemId themeId = treeCtrl->AppendItem(rootId, className, -1, -1, themeData);

        AttVisitor v;
        theme->accept(v);
        Attribute::List::iterator it = v.attributes.begin();
        for (; it != v.attributes.end(); ++it)
        {
            Attribute* attr = *it;
            if (attr->attrType == Attribute::attrMeta && attr->typeName == String("RectUV"))
            {
                wxString rectName = wxString::FromUTF8(attr->name);
                RectUV* value = (RectUV*)attr->data;

                RectItemData* rectData = new RectItemData();
                rectData->rect = value;
                rectData->theme = theme;
                
                treeCtrl->AppendItem(themeId, rectName, -1, -1, rectData);
            }
        }
    }

    treeCtrl->ExpandAll();
}

//////////////////////////////////////////////////////////////////////////
ThemeMIMEHandler::ThemeMIMEHandler()
{

}

bool ThemeMIMEHandler::newPage(ThemePackage* themes, bool isModified)
{
    EditorPageManager* epm = Manager::getInstancePtr()->getEditorPageManager();

    epm->getNotebook()->Freeze();
    ThemeEditorPage* page = new ThemeEditorPage(epm->getNotebook(), this);
    epm->addEditorPage(page);
    epm->getNotebook()->Thaw();

    page->setThemePackage(themes);

    if (isModified)
        page->setModified(isModified);

    int index = epm->getNotebook()->GetPageIndex(page);

    if (index != -1)
        epm->getNotebook()->SetSelection(index);

    return true;
}

bool ThemeMIMEHandler::canHandle(const wxString& filename) const
{
    return filename.Lower().EndsWith(wxT(".theme"));
}

bool ThemeMIMEHandler::openFile(const wxString& filename)
{
    ThemePackage* themes = (ThemePackage*)Object::importObject(filename.ToUTF8().data());

    return newPage(themes, false);
}
