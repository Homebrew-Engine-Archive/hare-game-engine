#include "PCH.h"
#include "GUIEditorPage.h"


IMPLEMENT_ABSTRACT_CLASS(GUIEditorPage, EditorPage)

BEGIN_EVENT_TABLE(GUIEditorPage, EditorPage)
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

    Layout();
}

GUIEditorPage::~GUIEditorPage()
{
    delete guiSys;
    guiSys = 0;
}

void GUIEditorPage::renderScene()
{
    guiSys->render();
}
