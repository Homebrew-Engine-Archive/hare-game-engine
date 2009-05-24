%module hare

%{
#include "LuaScriptRunner.h"
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
