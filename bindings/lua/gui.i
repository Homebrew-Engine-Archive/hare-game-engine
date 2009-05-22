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
