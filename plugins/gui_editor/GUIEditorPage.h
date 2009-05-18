#ifndef GUIEditorPage_H
#define GUIEditorPage_H

#include <wx/xrc/xmlres.h>
#include <wx/harecanvas.h>

class TreeItemData;
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

    virtual void setModified(bool modified);
    virtual bool getIsModified() const { return isModified; }
    virtual bool save();
    bool saveAs();
    void updateTitle();
    
protected:
    virtual void beginScene()
    {
    }
    virtual void endScene()
    {
    }
    virtual void renderScene();

    void drawHelperRect(TreeItemData* data, uint32 color = 0xFFFF0000);

    void onSize(wxSizeEvent& event);
    void onTreeItemSelected(wxTreeEvent& event);
    void onTreeItemRightClick(wxTreeEvent& event);
    void onMenuSelected(wxCommandEvent& event);
    void onTreeBeginDrag(wxTreeEvent& event);
    void onTreeEndDrag(wxTreeEvent& event);

    void addWindow(Window* window);
    void addSizer(Sizer* sizer);

    virtual bool Show(bool show = true);

private:
    wxHareCanvas* canvas;
    SceneManager* scene;
    GUISystem::Ptr guiSys;
    wxTreeCtrl* treeCtrl;
    wxImageList* imageList;
    int rootImage;
    int ctrlImage;
    int sizerImage;
    
    ClassInfoList windowClasses;
    ClassInfoList sizerClasses;

    bool isModified;

    wxTreeItemId draggedItem;

    Object::Ptr clipboardObject;

    DECLARE_EVENT_TABLE();
    DECLARE_ABSTRACT_CLASS(GUIEditorPage)
};

#endif