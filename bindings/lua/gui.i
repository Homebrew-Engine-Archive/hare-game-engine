%module hare

%{
#include "LuaScriptRunner.h"
#include "LuaEventHandler.h"
%}

class LuaScriptRunner : public ScriptRunner
{
public:
    String getLastError(bool clear = true);

    bool loadScript(const String& fileName = StringUtil::EMPTY);

    bool callFunction(const String& name);
};

class ThemePackage : public Object
{
};

class GUISystem : public Object
{
public:
    void setRoot(Window* window);
    
    Window*	getRoot();
    
    void setTheme(ThemePackage* themes);
    
    ThemePackage* getTheme();
    
    void render();
    
    bool notifyMouseMove(float deltaX, float deltaY);
    bool notifyMouseWheel(float delta);
    bool notifyMousePosition(float posX, float posY);
    bool notifyMouseButtonUp(uint32 button);
    bool notifyMouseButtonDown(uint32 button);

    bool notifyKeyDown(uint32 key);
    bool notifyKeyUp(uint32 key);
    bool notifyChar(uint32 cp);
};

class EventHandler : public Object
{
};

class Window : public EventHandler
{
public:
    ScriptRunner* getScriptRunner();
};

class Button : public Window
{
};

class FrameWindow : public Window
{
};

typedef int32 EventType;

void connectSelf(Window* window, EventType eventType, SWIGLUA_REF func);
void connectChild(Window* window, int id, EventType eventType, SWIGLUA_REF func);
void connectRange(Window* window, int id, int lastId, EventType eventType, SWIGLUA_REF func);

void disconnectSelf(Window* window, EventType eventType);
void disconnectChild(Window* window, int id, EventType eventType);
void disconnectRange(Window* window, int id, int lastId, EventType eventType);

// Mouse Events
%constant EventType uiEVT_LEFT_DOWN;
%constant EventType uiEVT_LEFT_UP;
%constant EventType uiEVT_LEFT_CLICK;
%constant EventType uiEVT_LEFT_DCLICK;
%constant EventType uiEVT_LEFT_TCLICK;

%constant EventType uiEVT_RIGHT_DOWN;
%constant EventType uiEVT_RIGHT_UP;
%constant EventType uiEVT_RIGHT_CLICK;
%constant EventType uiEVT_RIGHT_DCLICK;
%constant EventType uiEVT_RIGHT_TCLICK;

%constant EventType uiEVT_MIDDLE_DOWN;
%constant EventType uiEVT_MIDDLE_UP;
%constant EventType uiEVT_MIDDLE_CLICK;
%constant EventType uiEVT_MIDDLE_DCLICK;
%constant EventType uiEVT_MIDDLE_TCLICK;

%constant EventType uiEVT_MOTION;
%constant EventType uiEVT_LEAVE_WINDOW;
%constant EventType uiEVT_ENTER_WINDOW;
%constant EventType uiEVT_MOUSEWHEEL;

%constant EventType uiEVT_CHAR;
%constant EventType uiEVT_KEY_DOWN;
%constant EventType uiEVT_KEY_UP;


class Event : public Object
{
private:
    Event();
public:
    Object* getEventSender() const;
    int getId() const;
    void skip(bool skip = true);
    bool isSkipped() const;
};

class MouseEvent : public Event
{
private:
    MouseEvent();
public:
    bool isLeftDown() const;
    bool isMiddleDown() const;
    bool isRightDown() const;
    bool isEnteringEvent() const;
    bool isLeavingEvent() const;
    float getWheelDelta() const;
    PointF getPosition() const;
};

class KeyEvent : public Event
{
private:
    KeyEvent();
public:
    uint32 getKeyCode() const;
    uint32 getChar() const;
};

