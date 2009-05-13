#include "PCH.h"
#include "GUIEditorPage.h"

IMPLEMENT_ABSTRACT_CLASS(GUIEditorPage, EditorPage)

BEGIN_EVENT_TABLE(GUIEditorPage, EditorPage)
    EVT_TREE_SEL_CHANGED(XRCID("idTreeView"), GUIEditorPage::onTreeItemSelected)
    EVT_TREE_ITEM_RIGHT_CLICK(XRCID("idTreeView"), GUIEditorPage::onTreeItemRightClick)
END_EVENT_TABLE()

GUIEditorPage::GUIEditorPage(wxWindow* parent)
{
    wxXmlResource::Get()->LoadPanel(this, parent, wxT("idGUIEditorPanel"));

    scene = getHareApp()->createSceneManager();
    scene->setSceneListener(this);

    wxPanel* panel = XRCCTRL(*this, "idPanelR", wxPanel);
    canvas = new wxHareCanvas(panel);
    panel->GetSizer()->Add(canvas, 1, wxEXPAND, 0);
    canvas->getRenderWindow()->setSceneManager(scene);

    guiSys = new GUISystem();

    imageList = new wxImageList(16, 16);

    wxBitmap bmp;
    wxString fullPath = Manager::getInstancePtr()->getAppDir() + wxT("/resources/");
    bmp.LoadFile(fullPath + wxT("root.png"), wxBITMAP_TYPE_PNG);
    rootImage = imageList->Add(bmp);

    treeCtrl = XRCCTRL(*this, "idTreeView", wxTreeCtrl);
    treeCtrl->SetImageList(imageList);
    treeCtrl->AddRoot(wxT("Root"), rootImage);

    setTitle(wxT("[GUIEditor]"));

    Layout();
}

GUIEditorPage::~GUIEditorPage()
{
    treeCtrl->SetImageList(NULL);
    delete imageList;
    imageList = 0;

    delete guiSys;
    guiSys = 0;
}

void GUIEditorPage::onTreeItemSelected(wxTreeEvent& event)
{

}

void GUIEditorPage::onTreeItemRightClick(wxTreeEvent& event)
{

}

void GUIEditorPage::renderScene()
{
    guiSys->render();
}

void GUIEditorPage::setWindow(Window* window)
{
    guiSys->setRoot(window);
}

void GUIEditorPage::setTheme(ThemePackage* themes)
{
    guiSys->setTheme(themes);
}
