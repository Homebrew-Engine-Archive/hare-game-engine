#ifndef GUIEditorPage_H
#define GUIEditorPage_H

#include <wx/xrc/xmlres.h>
#include <wx/harecanvas.h>

/* 
 * GUIEditorPage
*/
class GUIEditorPage : public EditorPage, public SceneListenerBase
{
public:
    GUIEditorPage(wxWindow* parent);
    virtual ~GUIEditorPage();

    void setWindow(Window* window);
    void setTheme(ThemePackage* themes);
    
protected:
    virtual void beginScene()
    {
    }
    virtual void endScene()
    {
    }
    virtual void renderScene();

    void onTreeItemSelected(wxTreeEvent& event);
    void onTreeItemRightClick(wxTreeEvent& event);

private:
    wxHareCanvas* canvas;
    SceneManager* scene;
    GUISystem* guiSys;
    wxTreeCtrl* treeCtrl;
    wxImageList* imageList;
    int rootImage;

    std::list<ClassInfo*> winClasses;

    DECLARE_EVENT_TABLE();
    DECLARE_ABSTRACT_CLASS(GUIEditorPage)
};

#endif