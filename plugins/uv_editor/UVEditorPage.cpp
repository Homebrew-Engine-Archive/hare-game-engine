#include "PCH.h"
#include "UVEditorPage.h"
#include "drag_hand.xpm"

//////////////////////////////////////////////////////////////////////////
class MoveRectCommand : public Command
{
public:
    MoveRectCommand() {}

protected:
    virtual void _doExecute()
    {

    }
    virtual void _doRestore()
    {

    }
};

//////////////////////////////////////////////////////////////////////////
int idScaleSlider = XRCID("idScaleSlider");
int idScaleTxt =    XRCID("idScaleTxt");
int idPanelR =      XRCID("idPanelR");
int idListItems =   XRCID("idListItems");
int idAlignLeft =   XRCID("idAlignLeft");
int idAlignRight =  XRCID("idAlignRight");
int idAlignTop =    XRCID("idAlignTop");
int idAlignBottom = XRCID("idAlignBottom");
int idSameWidth =   XRCID("idSameWidth");
int idSameHeight =  XRCID("idSameHeight");
int idSameSize =    XRCID("idSameSize");

BEGIN_EVENT_TABLE(UVEditorPage, EditorPage)
    EVT_COMMAND_SCROLL(idScaleSlider, UVEditorPage::onScaleSlider)
    EVT_TEXT_ENTER(idScaleTxt,        UVEditorPage::onScaleEntered)
    EVT_LISTBOX(idListItems,          UVEditorPage::onListItemSelected)
    EVT_UPDATE_UI(idAlignLeft,        UVEditorPage::onToolBarUpdateUI)
    EVT_UPDATE_UI(idAlignRight,       UVEditorPage::onToolBarUpdateUI)
    EVT_UPDATE_UI(idAlignTop,         UVEditorPage::onToolBarUpdateUI)
    EVT_UPDATE_UI(idAlignBottom,      UVEditorPage::onToolBarUpdateUI)
    EVT_UPDATE_UI(idSameWidth,        UVEditorPage::onToolBarUpdateUI)
    EVT_UPDATE_UI(idSameHeight,       UVEditorPage::onToolBarUpdateUI)
    EVT_UPDATE_UI(idSameSize,         UVEditorPage::onToolBarUpdateUI)
    EVT_MENU(idAlignLeft,             UVEditorPage::onToolCommand)
    EVT_MENU(idAlignRight,            UVEditorPage::onToolCommand)
    EVT_MENU(idAlignTop,              UVEditorPage::onToolCommand)
    EVT_MENU(idAlignBottom,           UVEditorPage::onToolCommand)
    EVT_MENU(idSameWidth,             UVEditorPage::onToolCommand)
    EVT_MENU(idSameHeight,            UVEditorPage::onToolCommand)
    EVT_MENU(idSameSize,              UVEditorPage::onToolCommand)
END_EVENT_TABLE()

UVEditorPage::UVEditorPage(wxWindow* parent) : isModified(false)
{
    wxXmlResource::Get()->LoadPanel(this, parent, wxT("idUVEditorPanel"));

    scene = getHareApp()->createSceneManager();
    scene->setSceneListener(this);

    wxPanel* panel = XRCCTRL(*this, "idPanelR", wxPanel);
    canvas = new wxHareCanvas(panel);
    panel->GetSizer()->Add(canvas, 1, wxEXPAND, 0);
    canvas->getRenderWindow()->setSceneManager(scene);

    canvas->Connect(wxEVT_SIZE, wxSizeEventHandler(UVEditorPage::onSize), 0, this);
    panel->Connect(wxEVT_ERASE_BACKGROUND, wxEraseEventHandler(UVEditorPage::onEraseBackground), 0, this);

    canvas->Connect(wxEVT_MOUSEWHEEL, wxMouseEventHandler(UVEditorPage::onMouseWheel), 0, this);
    canvas->Connect(wxEVT_MOTION, wxMouseEventHandler(UVEditorPage::onMouseMove), 0, this);
    canvas->Connect(wxEVT_LEFT_DOWN, wxMouseEventHandler(UVEditorPage::onMouseLeftDown), 0, this);
    canvas->Connect(wxEVT_LEFT_UP, wxMouseEventHandler(UVEditorPage::onMouseLeftUp), 0, this);

    slider = XRCCTRL(*this, "idScaleSlider", wxSlider);
    text = XRCCTRL(*this, "idScaleTxt", wxTextCtrl);
    list = XRCCTRL(*this, "idListItems", wxListBox);

    dragCursor = new wxCursor((const char**)drag_hand_xpm);
    gridMtrl = (Material*)Object::importObject("/editor/grid.material");

    Layout();
}

UVEditorPage::~UVEditorPage()
{
    delete dragCursor;
    dragCursor = NULL;
}

void UVEditorPage::updateTitle()
{
    String title = "[UVEditor]";
    if (state) title += state->getUrl();

    if (isModified)
        setTitle(wxT("*") + wxString::FromUTF8(title.c_str()));
    else
        setTitle(wxString::FromUTF8(title.c_str()));
}

void UVEditorPage::undo()
{
    cmdProcessor.undo();
    setModified(!cmdProcessor.isAtSavePoint());
}

void UVEditorPage::redo()
{
    cmdProcessor.redo();
    setModified(!cmdProcessor.isAtSavePoint());
}

bool UVEditorPage::canUndo() const
{
    return cmdProcessor.canUndo();
}

bool UVEditorPage::canRedo() const
{
    return cmdProcessor.canRedo();
}

void UVEditorPage::setModified(bool modified)
{
    if (modified != isModified)
    {
        isModified = modified;
        updateTitle();
    }
}

inline int getSign(float v)
{
    return v == 0 ? 0 : (v > 0 ? 1 : -1);
}

RectUV UVEditorPage::makePixelAlign(const RectUV& rect)
{
    RectUV ret = rect;

    if (getMaterial() && getMaterial()->getTextureMtrl() &&
        getMaterial()->getTextureMtrl()->getTexture())
    {
        int32 mtrlW = getMaterial()->getTextureMtrl()->getTexture()->getWidth();
        int32 mtrlH = getMaterial()->getTextureMtrl()->getTexture()->getHeight();

        ret.scale((float)mtrlW, (float)mtrlH);

        ret.minX = (int32)(ret.minX + getSign(ret.minX) * 0.5f) / float(mtrlW);
        ret.minY = (int32)(ret.minY + getSign(ret.minY) * 0.5f) / float(mtrlH);
        ret.maxX = (int32)(ret.maxX + getSign(ret.maxX) * 0.5f) / float(mtrlW);
        ret.maxY = (int32)(ret.maxY + getSign(ret.maxY) * 0.5f) / float(mtrlH);
    }
    return ret;
}

void UVEditorPage::drawImpl()
{
    if (!state)
        return;

    if (!state->mtrl)
        return;

    if (!getMaterial()->getTextureMtrl())
        return;

    if (!getMaterial()->getTextureMtrl()->getTexture())
        return;

    int32 mtrlW = getMaterial()->getTextureMtrl()->getTexture()->getWidth();
    int32 mtrlH = getMaterial()->getTextureMtrl()->getTexture()->getHeight();
    int32 mtrlHalfW = mtrlW / 2;
    int32 mtrlHalfH = mtrlH / 2;

    RectF rect(state->offset.x - mtrlHalfW, state->offset.y - mtrlHalfH, 
        state->offset.x + mtrlHalfW, state->offset.y + mtrlHalfH);

    rect.scale<float>(state->scale, state->scale);

    RenderWindow* renderWindow = RenderSystem::getSingletonPtr()->getCurRenderWindow();
    uint32 winW = renderWindow->getWidth();
    uint32 winH = renderWindow->getHeight();

    rect.move(winW / 2.0f, winH / 2.0f);

    uint32 oldclr = getCanvas()->getColor();

    getCanvas()->setColor(0xFFFFFFFF);
    float u = (float)winW / gridMtrl->getTextureMtrl()->getTexture()->getWidth();
    float v = (float)winH / gridMtrl->getTextureMtrl()->getTexture()->getHeight();
    getCanvas()->drawImage(RectF(0, 0, winW, winH), gridMtrl, RectF(0, 0, u, v));
    getCanvas()->drawImage(rect, getMaterial());
    getCanvas()->drawRect(rect.minX, rect.minY, rect.maxX, rect.maxY);

    uint32 color = 0;
    bool drawSizers = false;

    uint32 focusedColor = 0xFF0A246A;
    uint32 selectedColor = 0xFF000000;
    uint32 normalColor = 0xFF808080;

    RectState::List::iterator it = state->rects.begin();
    for (; it != state->rects.end(); ++it) 
    {
        RectState* st = *it;
        if (st->hasFlag(RectState::State_Focused))
        {
            color = focusedColor;
            drawSizers = true;
        }
        else if (st->hasFlag(RectState::State_Selected))
        {
            color = selectedColor;
            drawSizers = false;
        }
        else
        {
            color = normalColor;
            drawSizers = false;
        }

        float l = st->rect.minX * rect.width() + rect.minX;
        float t = st->rect.minY * rect.height() + rect.minY;
        float r = st->rect.maxX * rect.width() + rect.minX;
        float b = st->rect.maxY * rect.height() + rect.minY;

        getCanvas()->setColor(color);
        getCanvas()->drawRect(l, t, r, b);

        if (drawSizers)
        {
            float u = (l + r) / 2; 
            float v = (t + b) / 2; 

            getCanvas()->drawRectFill(l - 3, t - 3, l + 3, t + 3);
            getCanvas()->drawRectFill(l - 3, v - 3, l + 3, v + 3);
            getCanvas()->drawRectFill(l - 3, b - 3, l + 3, b + 3);
            getCanvas()->drawRectFill(u - 3, t - 3, u + 3, t + 3);
            getCanvas()->drawRectFill(u - 3, b - 3, u + 3, b + 3);
            getCanvas()->drawRectFill(r - 3, t - 3, r + 3, t + 3);
            getCanvas()->drawRectFill(r - 3, v - 3, r + 3, v + 3);
            getCanvas()->drawRectFill(r - 3, b - 3, r + 3, b + 3);
        }
    }

    getCanvas()->setColor(oldclr);
}

void UVEditorPage::setUVState(UVEditorState* state)
{
    this->state = state;

    list->Clear();

    if (this->state)
    {
        RectState::List::iterator it = state->rects.begin();
        for (; it != state->rects.end(); ++it) 
        {
            RectState* st = *it;
            list->Append(wxString::FromUTF8(st->name.c_str()), st);
        }
    }

    updateTitle();
}

void UVEditorPage::addRectUV(const String& name, const RectUV& rect, RectState::EditState state)
{
    if (!this->state)
        return;

    RectState* st = new RectState;
    st->name = name;
    st->rect = rect;
    st->state = state;
    this->state->rects.push_back(st);

    list->Append(wxString::FromUTF8(st->name.c_str()), st);

    setModified(true);
}

void UVEditorPage::onToolCommand(wxCommandEvent& event)
{
    RectState* focused = getFocusedRect();

    if (focused)
    {
        RectState::List::iterator it = state->rects.begin();
        for (; it != state->rects.end(); ++it) 
        {
            RectState* st = *it;
            if (st != getFocusedRect() && st->hasFlag(RectState::State_Selected))
            {
                if (event.GetId() == idAlignLeft)
                    st->rect.move(focused->rect.minX - st->rect.minX, 0);
                else if (event.GetId() == idAlignTop)
                    st->rect.move(0, focused->rect.minY - st->rect.minY);
                else if (event.GetId() == idAlignRight)
                    st->rect.move(focused->rect.maxX - st->rect.maxX, 0);
                else if (event.GetId() == idAlignBottom)
                    st->rect.move(0, focused->rect.maxY - st->rect.maxY);
                
                if (event.GetId() == idSameWidth || event.GetId() == idSameSize)
                    st->rect.maxX = st->rect.minX + focused->rect.maxX - focused->rect.minX;
                
                if (event.GetId() == idSameHeight || event.GetId() == idSameSize)
                    st->rect.maxY = st->rect.minY + focused->rect.maxY - focused->rect.minY;

                setModified(true);
            }
        }
    }
}

void UVEditorPage::onToolBarUpdateUI(wxUpdateUIEvent& event)
{
    if (!getFocusedRect())
    {
        event.Enable(false);
        return;
    }

    RectState::List::iterator it = state->rects.begin();
    for (; it != state->rects.end(); ++it) 
    {
        RectState* st = *it;
        if (st != getFocusedRect() && st->hasFlag(RectState::State_Selected))
        {
            event.Enable(true);
            return;
        }
    }
    
    event.Skip();
}

void UVEditorPage::onListItemSelected(wxCommandEvent& event)
{
    if (!state)
        return;

    RectState::List::iterator it = state->rects.begin();
    for (; it != state->rects.end(); ++it) 
    {
        RectState* st = *it;
        st->state = RectState::State_None;
    }

    wxArrayInt sels;
    list->GetSelections(sels);
    for (size_t i = 0; i < sels.Count(); ++i)
    {
        RectState* st = (RectState*)list->GetClientData(sels[i]);
        st->state = RectState::State_Selected;
    }

    RectState* newSelected = (RectState*)list->GetClientData(event.GetSelection());
    if (newSelected)
    {
        RectState::List::iterator it = std::find(state->rects.begin(), state->rects.end(), newSelected);
        if (it != state->rects.end())
        {
            newSelected->state = RectState::State_Focused;
            state->rects.push_front(newSelected);
            state->rects.erase(it);
        }
    }
}

void UVEditorPage::onScaleSlider(wxScrollEvent& event)
{
    if (!state)
        return;

    int minScale = slider->GetMin();    // -100
    int maxScale = slider->GetMax();    // +100
    int value = slider->GetValue();
    if (value >= 0)
        state->scale = 1.0f + value / 10.0f;
    else
        state->scale = 1.0f / (1.0f - value / 10.0f);

    wxString txt = wxString::Format(wxT("%.0f"), state->scale * 100);
    text->SetLabel(txt);
}

void UVEditorPage::onScaleEntered(wxCommandEvent& event)
{
    if (!state)
        return;

    wxString valStr = text->GetValue();
    int value = wxAtoi(valStr);
    state->scale = value / 100.0f;
    int sliderVal = 0;

    if (state->scale > 1.0f)
        sliderVal = (state->scale - 1.0f) * 10.0f;
    else
        sliderVal = 10.0f - 10.0f / state->scale;

    slider->SetValue(sliderVal);
}

void UVEditorPage::onMouseWheel(wxMouseEvent& event)
{
    if (!state)
        return;

    int value = event.GetWheelRotation() / event.GetWheelDelta();
    state->scale *= pow(1.04f, value);
    if (state->scale < 0.1f)
        state->scale = 0.1f;

    // Update slider
    int sliderVal = 0;
    if (state->scale > 1.0f)
        sliderVal = (state->scale - 1.0f) * 10.0f;
    else
        sliderVal = 10.0f - 10.0f / state->scale;
    slider->SetValue(sliderVal);

    // Update text
    wxString txt = wxString::Format(wxT("%.0f"), state->scale * 100);
    text->SetLabel(txt);
}

RectState* UVEditorPage::getFocusedRect()
{
    if (!state)
        return NULL;

    RectState::List::iterator it = state->rects.begin();
    for (; it != state->rects.end(); ++it) 
    {
        RectState* st = *it;
        if (st->hasFlag(RectState::State_Focused))
            return st;
    }
    return NULL;
}

void UVEditorPage::onMouseLeftUp(wxMouseEvent& event)
{
    if (dragAction == DA_DraggingCanvas)
        dragAction = DA_None;
    onMouseMove(event);
    updateMouseState();
    event.Skip();

    RectState::List::iterator it = state->rects.begin();
    for (; it != state->rects.end(); ++it) 
    {
        RectState* st = *it;
        st->rect = makePixelAlign(st->rect);
    }
}

void UVEditorPage::onMouseLeftDown(wxMouseEvent& event)
{
    mouseDownPos = event.GetPosition();

    if (!state)
        return;

    if (!state->mtrl)
        return;

    int32 mtrlW = state->mtrl->getTextureMtrl()->getTexture()->getWidth();
    int32 mtrlH = state->mtrl->getTextureMtrl()->getTexture()->getHeight();
    int32 mtrlHalfW = mtrlW / 2;
    int32 mtrlHalfH = mtrlH / 2;

    RectF rect(state->offset.x - mtrlHalfW, state->offset.y - mtrlHalfH, 
        state->offset.x + mtrlHalfW, state->offset.y + mtrlHalfH);

    rect.scale<float>(state->scale, state->scale);

    rect.move(canvas->GetSize().GetWidth() / 2.0f, canvas->GetSize().GetHeight() / 2.0f);

    PointF pt(mouseDownPos.x, mouseDownPos.y);

    RectState* focused = getFocusedRect();

    RectState::List::iterator it = state->rects.begin();
    for (; it != state->rects.end(); ++it) 
    {
        if (dragAction >= DA_SizingLeftTop && dragAction <= DA_SizingRightBottom)
            break;

        RectState* st = *it;

        float l = st->rect.minX * rect.width() + rect.minX;
        float t = st->rect.minY * rect.height() + rect.minY;
        float r = st->rect.maxX * rect.width() + rect.minX;
        float b = st->rect.maxY * rect.height() + rect.minY;

        RectF rc(l, t, r, b);

        if (rc.isPointIn(pt))
        {
            if (focused)
            {
                if (event.ControlDown())
                    focused->state = RectState::State_Selected;
                else
                    focused->state = RectState::State_None;
            }

            st->state = RectState::State_Focused;
            state->rects.push_front(st);
            state->rects.erase(it);
            break;
        }
    }

    if (!event.ControlDown() && it == state->rects.end() && dragAction == DA_None)
    {
        it = state->rects.begin();
        for (; it != state->rects.end(); ++it) 
        {
            RectState* st = *it;
            st->state = RectState::State_None;
            list->SetSelection(-1);
        }
    }

    // call onMouseMove to get the correct dragAction
    wxMouseEvent fakeMove(wxEVT_MOTION);
    fakeMove.m_x = mouseDownPos.x;
    fakeMove.m_y = mouseDownPos.y;
    onMouseMove(fakeMove);
    
    updateMouseState();
    updateListState();

    event.Skip();
}

void UVEditorPage::onMouseMove(wxMouseEvent& event)
{
    if (!state)
        return;

    if (!state->mtrl)
        return;

    if (event.ControlDown() && event.Dragging())
    {
        dragAction = DA_DraggingCanvas;

        wxPoint offset = event.GetPosition() - mouseDownPos;
        
        state->offset.x += offset.x / state->scale;
        state->offset.y += offset.y / state->scale;

        mouseDownPos = event.GetPosition();
    }
    else if (getFocusedRect())
    {
        int32 mtrlW = state->mtrl->getTextureMtrl()->getTexture()->getWidth();
        int32 mtrlH = state->mtrl->getTextureMtrl()->getTexture()->getHeight();
        int32 mtrlHalfW = mtrlW / 2;
        int32 mtrlHalfH = mtrlH / 2;

        RectF rect(state->offset.x - mtrlHalfW, state->offset.y - mtrlHalfH, 
            state->offset.x + mtrlHalfW, state->offset.y + mtrlHalfH);
        
        rect.scale<float>(state->scale, state->scale);
        rect.move(canvas->GetSize().GetWidth() / 2.0f, canvas->GetSize().GetHeight() / 2.0f);

        RectState* focused = getFocusedRect();

        float l = focused->rect.minX * rect.width() + rect.minX;
        float t = focused->rect.minY * rect.height() + rect.minY;
        float r = focused->rect.maxX * rect.width() + rect.minX;
        float b = focused->rect.maxY * rect.height() + rect.minY;

        float u = (l + r) / 2; 
        float v = (t + b) / 2; 

        if (!event.Dragging() && !event.LeftIsDown())
        {
            PointF pt(event.GetPosition().x, event.GetPosition().y);

            RectF rects[8] = {
                RectF(l - 3, t - 3, l + 3, t + 3),
                RectF(l - 3, v - 3, l + 3, v + 3),
                RectF(l - 3, b - 3, l + 3, b + 3),
                RectF(u - 3, t - 3, u + 3, t + 3),
                RectF(u - 3, b - 3, u + 3, b + 3),
                RectF(r - 3, t - 3, r + 3, t + 3),
                RectF(r - 3, v - 3, r + 3, v + 3),
                RectF(r - 3, b - 3, r + 3, b + 3),
            };

            for (int i = 0; i < 8; ++i)
            {
                if (rects[i].isPointIn(pt))
                {
                    dragAction = (DragAction)i;
                    break;
                }
            }

            if (i >= 8)
            {
                if (RectF(l, t, r, b).isPointIn(pt))
                    dragAction = DA_Moving;
                else
                    dragAction = DA_None;
            }
        }
        else if (event.Dragging() && event.LeftIsDown())
        {
            wxPoint offset = event.GetPosition() - mouseDownPos;
            mouseDownPos = event.GetPosition();

            float x = offset.x;
            float y = offset.y;

            RectState::List::iterator it = state->rects.begin();
            for (; it != state->rects.end(); ++it) 
            {
                RectState* st = *it;

                if (st->hasFlag(RectState::State_Selected))
                {
                    switch (dragAction)
                    {
                    case DA_SizingLeftTop:
                    case DA_SizingLeftBottom:
                    case DA_SizingLeft:
                    case DA_Moving:
                        st->rect.minX += x / rect.width();
                    }
                    switch (dragAction)
                    {
                    case DA_SizingRightBottom:
                    case DA_SizingRightTop:
                    case DA_SizingRight:
                    case DA_Moving:
                        st->rect.maxX += x / rect.width();
                    }
                    switch (dragAction)
                    {
                    case DA_SizingLeftTop:
                    case DA_SizingRightTop:
                    case DA_SizingTop:
                    case DA_Moving:
                        st->rect.minY += y / rect.height();
                    }
                    switch (dragAction)
                    {
                    case DA_SizingLeftBottom:
                    case DA_SizingRightBottom:
                    case DA_SizingBottom:
                    case DA_Moving:
                        st->rect.maxY += y / rect.height();
                    }

                    st->rect.normalize();
                    setModified(true);
                }
            }
        }
    }
    updateMouseState();
    event.Skip();
}

void UVEditorPage::updateListState()
{
    int count = list->GetCount();

    for (int i = 0; i < count; ++i)
    {
        RectState* st = (RectState*)list->GetClientData(i);
        if (st->hasFlag(RectState::State_Selected))
            list->SetSelection(i, true);
        else
            list->SetSelection(i, false);
    }
}

void UVEditorPage::updateMouseState()
{
    switch (dragAction)
    {
    case DA_SizingLeftTop:
    case DA_SizingRightBottom:
        wxSetCursor(wxCursor(wxCURSOR_SIZENWSE));
        break;
    case DA_SizingRightTop:
    case DA_SizingLeftBottom:
        wxSetCursor(wxCursor(wxCURSOR_SIZENESW));
        break;
    case DA_SizingTop:
    case DA_SizingBottom:
        wxSetCursor(wxCursor(wxCURSOR_SIZENS));
        break;
    case DA_SizingLeft:
    case DA_SizingRight:
        wxSetCursor(wxCursor(wxCURSOR_SIZEWE));
        break;
    case DA_Moving:
        wxSetCursor(wxCursor(wxCURSOR_SIZING));
        break;
    case DA_DraggingCanvas:
        wxSetCursor(*dragCursor);
        break;
    }
}

void UVEditorPage::onSize(wxSizeEvent& event)
{
    wxSize size = canvas->GetClientSize();
    if (size.GetWidth() > 0 && size.GetHeight() > 0)
    {
        canvas->getRenderWindow()->resize(size.GetWidth(), size.GetHeight());
    }
    event.Skip();
}

void UVEditorPage::onEraseBackground(wxEraseEvent& event)
{
}
