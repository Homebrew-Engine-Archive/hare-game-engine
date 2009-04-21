#ifndef UVEDITORPAGE_H
#define UVEDITORPAGE_H

#include <wx/xrc/xmlres.h>
#include <wx/harecanvas.h>

class UVEditorPage;

/* 
 * RectState
 */
class RectState : public Object
{
    HARE_DECLARE_DYNAMIC_CLASS(RectState)
public:

    enum EditState
    {
        State_None      = 1 << 0,
        State_Selected  = 1 << 1,
        State_Focused   = 1 << 2 | State_Selected,
    };

    bool hasFlag(EditState flag)
    {
        return (state & flag) == flag;
    }

public:
    String name;
    RectUV rect;
    EditState state;
};

/* 
 * UVEditorState
*/
struct UVEditorState
{
    PointF offset;          // Offset from window center to image center
    float scale;
    RectState::List rects;

    UVEditorState() : scale(1.0f), offset(0.0f, 0.0f)
    {
    }
};

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
        return &state;
    }

    RectState* getFocusedRect();

    void addRectUV(const String& name, const RectUV& rect, RectState::EditState state);

    Material* getMaterial()
    {
        return mtrl;
    }

    void setMaterial(Material* material)
    {
        mtrl = material;
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

private:
    wxHareCanvas* canvas;
    SceneManager* scene;
    UVEditorState state;

    static Material::Ptr gridMtrl;
    
    Material::Ptr mtrl;
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
    DECLARE_ABSTRACT_CLASS(UVEditorPage)
};

#endif