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

protected:

    virtual void beginScene()
    {
    }
    virtual void endScene()
    {
    }
    virtual void renderScene()
    {
    }

private:
    wxHareCanvas* canvas;
    SceneManager* scene;

    DECLARE_EVENT_TABLE();
    DECLARE_ABSTRACT_CLASS(GUIEditorPage)
};

#endif