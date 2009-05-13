#ifndef UVEDITORPAGE_H
#define UVEDITORPAGE_H

#include <wx/xrc/xmlres.h>
#include <wx/harecanvas.h>

class UVEditorPage;

/* 
 * UVEditorPage
*/
class UVEditorPage : public EditorPage, public SceneListenerBase
{
public:
    UVEditorPage(wxWindow* parent);
    virtual ~UVEditorPage(); 

    UVEditorState* getEditorState()
    {
        return state;
    }

    RectState* getFocusedRect();

    void setUVState(UVEditorState* state);

    void addRectUV(const String& name, const RectUV& rect, RectState::EditState state);

    Material* getMaterial()
    {
        return state ? state->mtrl : NULL;
    }

    void setMaterial(Material* material)
    {
        if (state)
            state->mtrl = material;
    }
protected:

    virtual void beginScene()
    {
    }
    virtual void endScene()
    {
    }
    virtual void renderScene()
    {
        drawImpl();
    }
    virtual void drawImpl();

private:
    void onSize(wxSizeEvent& event);
    void onEraseBackground(wxEraseEvent& event);
    void onMouseLeftDown(wxMouseEvent& event);
    void onMouseLeftUp(wxMouseEvent& event);
    void onMouseMove(wxMouseEvent& event);
    void onMouseWheel(wxMouseEvent& event);
    void onScaleSlider(wxScrollEvent& event);
    void onScaleEntered(wxCommandEvent& event);
    void onListItemSelected(wxCommandEvent& event);
    void onToolBarUpdateUI(wxUpdateUIEvent& event);
    void onToolCommand(wxCommandEvent& event);

    void updateMouseState();

protected:
    wxHareCanvas* canvas;
    SceneManager* scene;
    UVEditorState::Ptr state;

    static Material::Ptr gridMtrl;
    
    wxSlider* slider;
    wxTextCtrl* text;
    wxListBox* list;

    wxPoint mouseDownPos;
    wxCursor* dragCursor;

    enum DragAction
    {
        DA_SizingLeftTop,
        DA_SizingLeft,
        DA_SizingLeftBottom,
        DA_SizingTop,
        DA_SizingBottom,
        DA_SizingRightTop,
        DA_SizingRight,
        DA_SizingRightBottom,

        DA_Moving,
        DA_DraggingCanvas,

        DA_None,
    };

    DragAction dragAction;

    DECLARE_EVENT_TABLE();
};

#endif