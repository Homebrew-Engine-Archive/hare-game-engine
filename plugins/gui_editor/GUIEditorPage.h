#ifndef GUIEditorPage_H
#define GUIEditorPage_H

#include <wx/xrc/xmlres.h>
#include <wx/harecanvas.h>

class TreeItemData;
/* 
 * GUIEditorPage
*/
class GUIEditorPage : public EditorPage
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
    friend class SceneListenerEditorWrapper<GUIEditorPage>;
    void renderScene();

    void drawHelperRect(TreeItemData* data, uint32 color = 0xFFFF0000);

    void rebindProperty();
    void rebindTreeView();

    wxTreeItemId bindTreeItem(const wxTreeItemId& parentId, Window* window, TreeItemData* data = 0);
    wxTreeItemId bindTreeItem(const wxTreeItemId& parentId, Sizer* sizer, TreeItemData* data = 0);
    wxTreeItemId bindTreeItem(const wxTreeItemId& parentId, SizerItem* item, TreeItemData* data = 0);


    void onSize(wxSizeEvent& event);
    void onTreeItemSelected(wxTreeEvent& event);
    void onTreeItemRightClick(wxTreeEvent& event);
    void onMenuSelected(wxCommandEvent& event);
    void onTreeBeginDrag(wxTreeEvent& event);
    void onTreeEndDrag(wxTreeEvent& event);
    void onToolEvent(wxCommandEvent& event);
    void onToolUpdateUI(wxUpdateUIEvent& event);
    void onTextEntered(wxCommandEvent& event);
    void onTextFocusKillEvent(wxFocusEvent& event);

    void addWindow(const wxTreeItemId& parentId, Window* window);
    void addSizer(const wxTreeItemId& parentId, Sizer* sizer);

    virtual bool Show(bool show = true);

    virtual void onAssociatedObjectEdited(Object* object, Attribute* attr)
    {
        if (guiSys && guiSys->getRoot())
        {
            guiSys->getRoot()->layout();
        }
        setModified(true);
    }

private:
    wxHareCanvas* canvas;
    SceneManager* scene;
    GUISystem::Ptr guiSys;
    wxTreeCtrl* treeCtrl;
    wxImageList* imageList;
    wxTextCtrl* textXYWH[4];
    
    int Image_ROOT;
    int Image_CTRL;
    int Image_SIZER;
    int Image_SPACER;
    
    ClassInfoList windowClasses;
    ClassInfoList sizerClasses;

    bool isModified;

    wxTreeItemId draggedItem;

    DECLARE_EVENT_TABLE();
    DECLARE_ABSTRACT_CLASS(GUIEditorPage)
};

#endif