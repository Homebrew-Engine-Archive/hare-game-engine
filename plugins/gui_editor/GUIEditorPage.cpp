#include "PCH.h"
#include "GUIEditorPage.h"
#include <wx/utils.h>
#include <wx/propgrid/propgrid.h>

class TreeItemData : public wxTreeItemData
{
public:
    TreeItemData(Window* w)       : window(w)  { type = Type_Window;    }
    TreeItemData(Sizer* s)        : sizer(s)   { type = Type_Sizer;     }
    TreeItemData(SizerSpacer* sp) : spacer(sp) { type = Type_Spacer;    }
    TreeItemData(SizerItem* it)   : item(it)   { type = Type_SizerItem; }
    
    bool isWindow()    const { return type == Type_Window;    }
    bool isSizer()     const { return type == Type_Sizer;     }
    bool isSizerItem() const { return type == Type_SizerItem; }
    bool isSpacer()    const { return type == Type_Spacer;    }

    enum ItemDataType { Type_Window, Type_Sizer, Type_SizerItem, Type_Spacer };

    union 
    {
        Window* window;
        Sizer* sizer;
        SizerItem* item;
        SizerSpacer* spacer;
    };

    ItemDataType type;
};

wxString floatToString(float val)
{
    wxString ret;
    wxString template_str;
    wxPropertyGrid::DoubleToString(ret, (double)val, -1, true, &template_str);
    return ret;
}

IMPLEMENT_ABSTRACT_CLASS(GUIEditorPage, EditorPage)

static int idAlignLeft = XRCID("idAlignLeft");
static int idAlignCenterHorizontally = XRCID("idAlignCenterHorizontally");
static int idAlignRight = XRCID("idAlignRight");
static int idAlignTop = XRCID("idAlignTop");
static int idAlignCenterVertically = XRCID("idAlignCenterVertically");
static int idAlignBottom = XRCID("idAlignBottom");
static int idExpand = XRCID("idExpand");
static int idFixedMinsize = XRCID("idFixedMinsize");
static int idShaped = XRCID("idShaped");
static int idLeftBorder = XRCID("idLeftBorder");
static int idRightBorder = XRCID("idRightBorder");
static int idTopBorder = XRCID("idTopBorder");
static int idBottomBorder = XRCID("idBottomBorder");
static int idTreeView = XRCID("idTreeView");
static int idX = XRCID("idX");
static int idY = XRCID("idY");
static int idW = XRCID("idW");
static int idH = XRCID("idH");

BEGIN_EVENT_TABLE(GUIEditorPage, EditorPage)
    EVT_TEXT_ENTER(idX, GUIEditorPage::onTextEntered)
    EVT_TEXT_ENTER(idY, GUIEditorPage::onTextEntered)
    EVT_TEXT_ENTER(idW, GUIEditorPage::onTextEntered)
    EVT_TEXT_ENTER(idH, GUIEditorPage::onTextEntered)
    EVT_TOOL(idAlignLeft, GUIEditorPage::onToolEvent)
    EVT_TOOL(idAlignCenterHorizontally, GUIEditorPage::onToolEvent)
    EVT_TOOL(idAlignRight, GUIEditorPage::onToolEvent)
    EVT_TOOL(idAlignTop, GUIEditorPage::onToolEvent)
    EVT_TOOL(idAlignCenterVertically, GUIEditorPage::onToolEvent)
    EVT_TOOL(idAlignBottom, GUIEditorPage::onToolEvent)
    EVT_TOOL(idExpand, GUIEditorPage::onToolEvent)
    EVT_TOOL(idFixedMinsize, GUIEditorPage::onToolEvent)
    EVT_TOOL(idShaped, GUIEditorPage::onToolEvent)
    EVT_TOOL(idLeftBorder, GUIEditorPage::onToolEvent)
    EVT_TOOL(idRightBorder, GUIEditorPage::onToolEvent)
    EVT_TOOL(idTopBorder, GUIEditorPage::onToolEvent)
    EVT_TOOL(idBottomBorder, GUIEditorPage::onToolEvent)
    EVT_TREE_SEL_CHANGED(idTreeView, GUIEditorPage::onTreeItemSelected)
    EVT_TREE_ITEM_RIGHT_CLICK(idTreeView, GUIEditorPage::onTreeItemRightClick)
    EVT_TREE_BEGIN_DRAG(idTreeView, GUIEditorPage::onTreeBeginDrag)
    EVT_TREE_END_DRAG(idTreeView, GUIEditorPage::onTreeEndDrag)
    EVT_UPDATE_UI(idAlignLeft, GUIEditorPage::onToolUpdateUI)
    EVT_UPDATE_UI(idAlignCenterHorizontally, GUIEditorPage::onToolUpdateUI)
    EVT_UPDATE_UI(idAlignRight, GUIEditorPage::onToolUpdateUI)
    EVT_UPDATE_UI(idAlignTop, GUIEditorPage::onToolUpdateUI)
    EVT_UPDATE_UI(idAlignCenterVertically, GUIEditorPage::onToolUpdateUI)
    EVT_UPDATE_UI(idAlignBottom, GUIEditorPage::onToolUpdateUI)
    EVT_UPDATE_UI(idExpand, GUIEditorPage::onToolUpdateUI)
    EVT_UPDATE_UI(idFixedMinsize, GUIEditorPage::onToolUpdateUI)
    EVT_UPDATE_UI(idShaped, GUIEditorPage::onToolUpdateUI)
    EVT_UPDATE_UI(idLeftBorder, GUIEditorPage::onToolUpdateUI)
    EVT_UPDATE_UI(idRightBorder, GUIEditorPage::onToolUpdateUI)
    EVT_UPDATE_UI(idTopBorder, GUIEditorPage::onToolUpdateUI)
    EVT_UPDATE_UI(idBottomBorder, GUIEditorPage::onToolUpdateUI)
END_EVENT_TABLE()

GUIEditorPage::GUIEditorPage(wxWindow* parent) : isModified(false)
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
    Image_ROOT = imageList->Add(bmp);
    bmp.LoadFile(fullPath + wxT("sizer.png"), wxBITMAP_TYPE_PNG);
    Image_SIZER = imageList->Add(bmp);
    bmp.LoadFile(fullPath + wxT("ctrl.png"), wxBITMAP_TYPE_PNG);
    Image_CTRL = imageList->Add(bmp);

    treeCtrl = XRCCTRL(*this, "idTreeView", wxTreeCtrl);
    treeCtrl->SetImageList(imageList);
    treeCtrl->AddRoot(wxT("Root"), Image_ROOT, Image_ROOT);

    textXYWH[0] = XRCCTRL(*this, "idX", wxTextCtrl);
    textXYWH[1] = XRCCTRL(*this, "idY", wxTextCtrl);
    textXYWH[2] = XRCCTRL(*this, "idW", wxTextCtrl);
    textXYWH[3] = XRCCTRL(*this, "idH", wxTextCtrl);

    for (int i = 0; i < 4; ++i)
    {
        textXYWH[i]->Connect(wxEVT_KILL_FOCUS, 
            wxFocusEventHandler(GUIEditorPage::onTextFocusKillEvent), 0, this);
    }

    setTitle(wxT("[GUIEditor]"));

    Layout();

    Window::CLASS_INFO.findSubs(windowClasses);
    Sizer::CLASS_INFO.findSubs(sizerClasses);
    
    Connect(wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(GUIEditorPage::onMenuSelected), 0, this);
    canvas->Connect(wxEVT_SIZE, wxSizeEventHandler(GUIEditorPage::onSize), 0, this);

    rebindProperty();
}

GUIEditorPage::~GUIEditorPage()
{
    treeCtrl->SetImageList(NULL);
    SAFE_DELETE(imageList);

    if (!Manager::isAppShuttingDown())
    {
        Manager::getInstancePtr()->getExplorerManager()->removeAllProperties();
    }
}

uint32 modifyFlag(uint32 flag, bool add, uint32 val)
{
    if (add)
        return flag | val;
    else
        return flag & ~val;
}

bool hasFlag(uint32 flag, uint32 val)
{
    return (flag & val) == val;
}

void GUIEditorPage::onTextFocusKillEvent(wxFocusEvent& event)
{
    wxCommandEvent cmdEvent;
    cmdEvent.SetId(event.GetId());

    wxString valStr;
    
    if (event.GetId() == idX)
        valStr = textXYWH[0]->GetLabel();
    else if (event.GetId() == idY)
        valStr = textXYWH[1]->GetLabel();
    else if (event.GetId() == idW)
        valStr = textXYWH[2]->GetLabel();
    else if (event.GetId() == idH)
        valStr = textXYWH[3]->GetLabel();

    if (!valStr.IsEmpty())
    {
        cmdEvent.SetString(valStr);
        onTextEntered(cmdEvent);
    }
}

void GUIEditorPage::onTextEntered(wxCommandEvent& event)
{
    Window* window = guiSys->getRoot();

    if (!window)
        return;

    TreeItemData* data = (TreeItemData*)treeCtrl->GetItemData(treeCtrl->GetSelection());
    if (data)
    {
        if (data->isSizerItem())
            window = data->item->getWindow();
    }

    int id = event.GetId();

    wxString valStr = event.GetString();
    float value = wxAtof(valStr);

    if (id == idX)
    {
        window->setPosition(value, window->getPosition().y);
    }
    if (id == idY)
    {
        window->setPosition(window->getPosition().x, value);
    }
    if (id == idW)
    {
        window->setSize(value, window->getSize().cy);
    }
    if (id == idH)
    {
        window->setSize(window->getSize().cx, value);
    }

    guiSys->getRoot()->layout();

    event.Skip();
}

void GUIEditorPage::onToolEvent(wxCommandEvent& event)
{
    TreeItemData* data = (TreeItemData*)treeCtrl->GetItemData(treeCtrl->GetSelection());

    if (data && data->isSizerItem())
    {
        int id = event.GetId();

        uint32 flag = data->item->getFlag();

        if (id == idAlignLeft)
        {
            flag = modifyFlag(flag, false, uiAlign_HorizontalMask);
            flag = modifyFlag(flag, event.IsChecked(), uiAlign_Left);
        }
        else if (id == idAlignCenterHorizontally)
        {
            flag = modifyFlag(flag, false, uiAlign_HorizontalMask);
            flag = modifyFlag(flag, event.IsChecked(), uiAlign_Center_Horizontal);
        }
        else if (id == idAlignRight)
        {
            flag = modifyFlag(flag, false, uiAlign_HorizontalMask);
            flag = modifyFlag(flag, event.IsChecked(), uiAlign_Right);
        }
        else if (id == idAlignTop)
        {
            flag = modifyFlag(flag, false, uiAlign_VerticalMask);
            flag = modifyFlag(flag, event.IsChecked(), uiAlign_Top);
        }
        else if (id == idAlignCenterVertically)
        {
            flag = modifyFlag(flag, false, uiAlign_VerticalMask);
            flag = modifyFlag(flag, event.IsChecked(), uiAlign_Center_Vertical);
        }
        else if (id == idAlignBottom)
        {
            flag = modifyFlag(flag, false, uiAlign_VerticalMask);
            flag = modifyFlag(flag, event.IsChecked(), uiAlign_Bottom);
        }
        else if (id == idExpand)
        {
            flag = modifyFlag(flag, event.IsChecked(), uiExpand);
        }
        else if (id == idFixedMinsize)
        {
            flag = modifyFlag(flag, event.IsChecked(), uiFixed_Minsize);
        }
        else if (id == idShaped)
        {
            flag = modifyFlag(flag, event.IsChecked(), uiShaped);
        }
        else if (id == idLeftBorder)
        {
            flag = modifyFlag(flag, event.IsChecked(), uiWest);
        }
        else if (id == idRightBorder)
        {
            flag = modifyFlag(flag, event.IsChecked(), uiEast);
        }
        else if (id == idTopBorder)
        {
            flag = modifyFlag(flag, event.IsChecked(), uiNorth);
        }
        else if (id == idBottomBorder)
        {
            flag = modifyFlag(flag, event.IsChecked(), uiSouth);
        }
        else
        {
            event.Skip();
            return;
        }
        
        data->item->setFlag(flag);
        if (guiSys && guiSys->getRoot())
            guiSys->getRoot()->layout();
        rebindProperty();
    }
}

void GUIEditorPage::onToolUpdateUI(wxUpdateUIEvent& event)
{
    if (!treeCtrl->GetSelection().IsOk())
    {
        event.Skip();
        return;
    }

    TreeItemData* data = (TreeItemData*)treeCtrl->GetItemData(treeCtrl->GetSelection());

    if (!data || !data->isSizerItem())
        event.Enable(false);
    else
    {
        event.Enable(true);

        int id = event.GetId();

        uint32 flag = data->item->getFlag();

        if (id == idAlignLeft)
            event.Check(hasFlag(flag, uiAlign_Left));
        else if (id == idAlignCenterHorizontally)
            event.Check(hasFlag(flag, uiAlign_Center_Horizontal));
        else if (id == idAlignRight)
            event.Check(hasFlag(flag, uiAlign_Right));
        else if (id == idAlignTop)
            event.Check(hasFlag(flag, uiAlign_Top));
        else if (id == idAlignCenterVertically)
            event.Check(hasFlag(flag, uiAlign_Center_Vertical));
        else if (id == idAlignBottom)
            event.Check(hasFlag(flag, uiAlign_Bottom));
        else if (id == idExpand)
            event.Check(hasFlag(flag, uiExpand));
        else if (id == idFixedMinsize)
            event.Check(hasFlag(flag, uiFixed_Minsize));
        else if (id == idShaped)
            event.Check(hasFlag(flag, uiShaped));
        else if (id == idLeftBorder)
            event.Check(hasFlag(flag, uiWest));
        else if (id == idRightBorder)
            event.Check(hasFlag(flag, uiEast));
        else if (id == idTopBorder)
            event.Check(hasFlag(flag, uiNorth));
        else if (id == idBottomBorder)
            event.Check(hasFlag(flag, uiSouth));
        else
            event.Skip();
    }
}

void GUIEditorPage::onSize(wxSizeEvent& event)
{
    wxSize size = canvas->GetClientSize();
    if (size.GetWidth() > 0 && size.GetHeight() > 0){
        canvas->getRenderWindow()->resize(size.GetWidth(), size.GetHeight());
    }
    event.Skip();
}

void GUIEditorPage::onTreeBeginDrag(wxTreeEvent& event)
{
    if (event.GetItem() == treeCtrl->GetRootItem())
    {
        return;
    }

    draggedItem = event.GetItem();
    event.Allow();
}

void GUIEditorPage::onTreeEndDrag(wxTreeEvent& event)
{
    bool copy = ::wxGetKeyState(WXK_CONTROL);

    wxTreeItemId itemSrc = draggedItem, itemDst = event.GetItem();

    // ensure that itemDst is not itemSrc or a child of itemSrc
    wxTreeItemId item = itemDst;
    while (item.IsOk())
    {
        if (item == itemSrc)
        {
            return;
        }
        item = treeCtrl->GetItemParent(item);
    }
}

void GUIEditorPage::rebindProperty()
{
    if (treeCtrl->GetSelection().IsOk())
    {
        TreeItemData* itemData = (TreeItemData*)treeCtrl->GetItemData(treeCtrl->GetSelection());

        if (itemData)
        {
            Object* object = NULL;

            if (itemData->isWindow())
                object = itemData->window;
            else if (itemData->isSizer())
                object = itemData->sizer;
            else if (itemData->isSizerItem())
                object = itemData->item;
            else if (itemData->isSpacer())
                object = itemData->spacer;

            if (object)
            {
                Manager::getInstancePtr()->getExplorerManager()->removeAllProperties();
                Manager::getInstancePtr()->getExplorerManager()->bindProperty(wxT("Properity"), object, this);
                return;
            }
        }
    }

    Manager::getInstancePtr()->getExplorerManager()->removeAllProperties();
    Manager::getInstancePtr()->getExplorerManager()->bindProperty(wxT("Properity"), guiSys);
}

wxTreeItemId GUIEditorPage::bindTreeItem(const wxTreeItemId& parentId, Window* window, TreeItemData* data)
{
    assert(window);

    wxTreeItemId itemId = treeCtrl->AppendItem(parentId, wxString::FromUTF8(window->getClassInfo()->className), 
        Image_CTRL, Image_CTRL, data ? data : new TreeItemData(window));

    if (window->getSizer())
    {
        return bindTreeItem(itemId, window->getSizer());
    }

    return itemId;
}

wxTreeItemId GUIEditorPage::bindTreeItem(const wxTreeItemId& parentId, Sizer* sizer, TreeItemData* data)
{
    assert(sizer);

    wxTreeItemId itemId = treeCtrl->AppendItem(parentId, wxString::FromUTF8(sizer->getClassInfo()->className), 
        Image_SIZER, Image_SIZER, data ? data : new TreeItemData(sizer));

    SizerItem::List& items = sizer->getChildren();
    SizerItem::List::iterator it = items.begin();
    for (; it != items.end(); ++it)
    {
        SizerItem* item = *it;
        bindTreeItem(itemId, item);
    }

    return itemId;
}

wxTreeItemId GUIEditorPage::bindTreeItem(const wxTreeItemId& parentId, SizerItem* item, TreeItemData* data)
{
    assert(item);

    if (item->getWindow())
    {
        return bindTreeItem(parentId, item->getWindow(), new TreeItemData(item));
    }
    else if (item->getSizer())
    {
        return bindTreeItem(parentId, item->getSizer(), new TreeItemData(item));
    }
    else if (item->getSpacer())
    {
        return treeCtrl->AppendItem(parentId, wxString::FromUTF8(item->getSpacer()->getClassInfo()->className), 
            Image_SPACER, Image_SPACER, new TreeItemData(item->getSpacer()));
    }

    assert(false);

    return wxTreeItemId();
}

void GUIEditorPage::rebindTreeView()
{
    treeCtrl->DeleteAllItems();

    wxTreeItemId parentId = treeCtrl->AddRoot(wxT("Root"), Image_ROOT, Image_ROOT);

    if (guiSys->getRoot())
    {
        Window* window = guiSys->getRoot();
        bindTreeItem(parentId, window);
        window->layout();
    }

    treeCtrl->ExpandAll();
}

void GUIEditorPage::onTreeItemSelected(wxTreeEvent& event)
{
    if (event.GetItem().IsOk())
        treeCtrl->SetItemBold(event.GetItem(), true);
    if (event.GetOldItem().IsOk())
        treeCtrl->SetItemBold(event.GetOldItem(), false);

    rebindProperty();

    TreeItemData* itemData = (TreeItemData*)treeCtrl->GetItemData(treeCtrl->GetSelection());

    if (itemData)
    {
        Window* window = NULL;

        if (itemData->isWindow())
            window = itemData->window;
        else if (itemData->isSizerItem())
            window = itemData->item->getWindow();

        if (window)
        {
            wxString x, y;
            wxString template_str;

            textXYWH[0]->SetValue(floatToString(window->getPosition().x));
            textXYWH[1]->SetValue(floatToString(window->getPosition().y));
            textXYWH[2]->SetValue(floatToString(window->getSize().cx));
            textXYWH[3]->SetValue(floatToString(window->getSize().cy));

            textXYWH[0]->Enable(true);
            textXYWH[1]->Enable(true);
            textXYWH[2]->Enable(true);
            textXYWH[3]->Enable(true);
        }
        else
        {
            for (int i = 0; i < 4; ++i)
            {
                textXYWH[i]->SetValue(wxEmptyString);
                textXYWH[i]->Enable(false);
            }
        }
    }

    event.Skip();
}

void GUIEditorPage::onTreeItemRightClick(wxTreeEvent& event)
{
    treeCtrl->SelectItem(event.GetItem());

    wxMenu* contexMenu = new wxMenu;
    TreeItemData* data = (TreeItemData*)treeCtrl->GetItemData(event.GetItem());

    bool add_sizer_menu = false;
    bool add_window_menu = false;

    if (data)
    {
        if (data->isWindow())
        {
            if (!data->window->getSizer())
                add_sizer_menu = true;
        }
        else if (data->isSizerItem())
        {
            if (data->item->getWindow())
            {
                Window* window = data->item->getWindow();
                if (!window->getSizer())
                    add_sizer_menu = true;
            }
            else if (data->item->getSizer())
            {
                add_sizer_menu = true;
                add_window_menu = true;
            }
        }
        else if (data->isSizer())
        {
            add_sizer_menu = true;
            add_window_menu = true;
        }
    }
    else
    {
        if (!guiSys->getRoot())
        {
            add_window_menu = true;
            add_sizer_menu = false;
        }
    }

    if (add_sizer_menu)
    {
        int nextId = windowClasses.size();
        for (size_t i = 0; i < sizerClasses.size(); ++i)
        {
            contexMenu->Append(nextId, wxT("Add ") + wxString::FromUTF8(sizerClasses[i]->className));
            nextId++;
        }
    }
    
    if (add_sizer_menu && add_window_menu)
        contexMenu->AppendSeparator();

    if (add_window_menu)
    {
        int nextId = 0;
        for (size_t i = 0; i < windowClasses.size(); ++i)
        {
            contexMenu->Append(nextId, wxT("Add ") + wxString::FromUTF8(windowClasses[i]->className));
            nextId++;
        }
    }

    PopupMenu(contexMenu);

    delete contexMenu;

    event.Skip();
}

void GUIEditorPage::drawHelperRect(TreeItemData* data, uint32 color)
{
    if (data)
    {
        PointF pos(0, 0);
        SizeF size(0, 0);

        if (data->isSizer())
        {
            pos = data->sizer->getPosition();
            pos = windowToScreen(*data->sizer->getContainingWindow(), pos);
            size = data->sizer->getSize();

            getCanvas()->setColor(color);
            getCanvas()->drawRect(pos.x, pos.y, pos.x + size.cx, pos.y + size.cy);
            getCanvas()->setColor(0xFFFFFFFF);
        }
        else if (data->isSizerItem())
        {
            pos = data->item->getPosition();
            size = data->item->getSize();

            Window* window = data->item->getWindow();

            if (window)
            {
                window = window->getParent();
                if (window)
                {
                    pos = windowToScreen(*window, pos);

                    getCanvas()->setColor(color);
                    getCanvas()->drawRect(pos.x, pos.y, pos.x + size.cx, pos.y + size.cy);
                    getCanvas()->setColor(0xFFFFFFFF);
                }
            }
            else
            {
                Sizer* sizer = data->item->getSizer();

                if (sizer)
                {
                    window = sizer->getContainingWindow();
                    if (window)
                    {
                        pos = windowToScreen(*window, pos);

                        getCanvas()->setColor(color);
                        getCanvas()->drawRect(pos.x, pos.y, pos.x + size.cx, pos.y + size.cy);
                        getCanvas()->setColor(0xFFFFFFFF);
                    }
                }
            }
        }
    }
}

void GUIEditorPage::renderScene()
{
    guiSys->render();

    if (treeCtrl->GetSelection().IsOk())
    {
        wxTreeItemId parentId = treeCtrl->GetItemParent(treeCtrl->GetSelection());
        if (parentId.IsOk())
        {
            TreeItemData* parentData = (TreeItemData*)treeCtrl->GetItemData(parentId);
            drawHelperRect(parentData, 0xFF0000FF);
        }

        TreeItemData* data = (TreeItemData*)treeCtrl->GetItemData(treeCtrl->GetSelection());
        drawHelperRect(data, 0xFFFF0000);
    }
}

void GUIEditorPage::setWindow(Window* window)
{
    guiSys->setRoot(window);
    updateTitle();
    rebindTreeView();
}

void GUIEditorPage::setTheme(ThemePackage* themes)
{
    guiSys->setTheme(themes);
}

void GUIEditorPage::addWindow(const wxTreeItemId& parentId, Window* window)
{
    TreeItemData* data = (TreeItemData*)treeCtrl->GetItemData(parentId);

    Sizer* sizer = NULL;
    SizerItem* newItem = NULL;

    if (data)
    {
        if (data->isSizer())
            sizer = data->sizer;
        else if (data->isSizerItem())
            sizer = data->item->getSizer();
    }

    if (sizer)
    {   
        newItem = sizer->add(window);

        Window* parent = sizer->getContainingWindow();

        if (parent)
        {
            window->reparent(parent);
        }
        else
            assert(false);
    }
    else if (!guiSys->getRoot())
    {
        guiSys->setRoot(window);
        window->setSize(640, 480);
    }
    else
        return;

    wxTreeItemId item = treeCtrl->AppendItem(parentId, wxString::FromUTF8(window->getClassInfo()->className), 
        Image_CTRL, Image_CTRL, newItem ? new TreeItemData(newItem) : new TreeItemData(window));

    setModified(true);
    guiSys->getRoot()->layout();

    treeCtrl->Expand(parentId);
    treeCtrl->SelectItem(item);
}

void GUIEditorPage::addSizer(const wxTreeItemId& parentId, Sizer* sizer)
{
    TreeItemData* data = (TreeItemData*)treeCtrl->GetItemData(parentId);

    if (data)
    {
        SizerItem* sizerItem = NULL;
        bool sizer_accepted = false;

        if (data->isWindow() && !data->window->getSizer())
        {
            data->window->setSizer(sizer);
            sizer_accepted = true;
        }
        else if (data->isSizer())
        {
            sizerItem = data->sizer->add(sizer, 1, uiExpand, 5);
            sizer_accepted = true;
        }
        else if (data->isSizerItem())
        {
            Window* itemWindow = data->item->getWindow();
            Sizer* itemSizer = data->item->getSizer();

            if (itemWindow && !itemWindow->getSizer())
            {
                itemWindow->setSizer(sizer);
                sizer_accepted = true;
            }
            else if (itemSizer)
            {
                sizerItem = itemSizer->add(sizer, 1, uiExpand, 5);
                sizer_accepted = true;
            }
        }

        if (sizer_accepted)
        {
            wxTreeItemId item = treeCtrl->AppendItem(parentId, wxString::FromUTF8(sizer->getClassInfo()->className), 
                Image_SIZER, Image_SIZER, sizerItem ? new TreeItemData(sizerItem) : new TreeItemData(sizer));

            setModified(true);

            guiSys->getRoot()->layout();

            treeCtrl->Expand(parentId);
            treeCtrl->SelectItem(item);
        }
    }
}

void GUIEditorPage::onMenuSelected(wxCommandEvent& event)
{
    int id = event.GetId();

    if (!treeCtrl->GetSelection().IsOk())
    {
        event.Skip();
        return;
    }

    if (id >= 0)
    {
        if (id < (int)windowClasses.size())
        {
            ClassInfo* clsInfo = windowClasses[id];
            Window::Ptr window = (Window*)clsInfo->createObject();

            if (window)
                addWindow(treeCtrl->GetSelection(), window);
        }
        else if (id < (int)(windowClasses.size() + sizerClasses.size()))
        {
            ClassInfo* clsInfo = sizerClasses[id - windowClasses.size()];
            Sizer::Ptr sizer = (Sizer*)clsInfo->createObject();

            if (sizer)
                addSizer(treeCtrl->GetSelection(), sizer);
        }
        else
            event.Skip();
    }
}

void GUIEditorPage::updateTitle()
{
    String title = "[GUIEditor]";
    if (guiSys->getRoot()) title += guiSys->getRoot()->getUrl();

    if (isModified)
        setTitle(wxT("*") + wxString::FromUTF8(title.c_str()));
    else
        setTitle(wxString::FromUTF8(title.c_str()));
}

void GUIEditorPage::setModified(bool modified)
{
    if (modified != isModified)
    {
        isModified = modified;
        updateTitle();
    }
}

bool GUIEditorPage::saveAs()
{
    bool ret = false;

    wxFileDialog* dlg = new wxFileDialog(Manager::getInstancePtr()->getAppWindow(),
        _T("Save window layout as"),
        _T(""),
        _T(""),
        _T("Window Layout (*.layout)|*.layout|Any file (*)|*"),
        wxSAVE | wxOVERWRITE_PROMPT);

    if (dlg->ShowModal() == wxID_OK)
    {
        static const char* tempFileName = "/editor/~layout.temp";
        FileSystem::getSingletonPtr()->remove(tempFileName);
        guiSys->getRoot()->saveToXml(tempFileName);
        String dir = FileSystem::getSingletonPtr()->getRealDir(tempFileName);
        dir += tempFileName;
        wxString tempFile = wxString::FromUTF8(dir.c_str());
        wxFileName fName(tempFile);
        if (fName.FileExists())
        {
            wxString fileName = dlg->GetPath();
            ret = wxCopyFile(fName.GetFullPath(), fileName);
        }
    }

    dlg->Destroy();
    return ret;
}

bool GUIEditorPage::save()
{
    if (!guiSys->getRoot())
        return true;

    bool ret = false;

    if (guiSys->getRoot()->getUrl().empty())
        ret = saveAs();
    else
        ret = guiSys->getRoot()->saveToXml(guiSys->getRoot()->getUrl());

    if (ret)
        setModified(false);

    return ret;
}

bool GUIEditorPage::Show(bool show)
{
    if (show)
    {
        Manager::getInstancePtr()->getExplorerManager()->removeAllProperties();
    }
    return EditorPage::Show(show);
}