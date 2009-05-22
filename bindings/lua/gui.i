%module hare

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
};

class Button : public Window
{
};

class FrameWindow : public Window
{
};
