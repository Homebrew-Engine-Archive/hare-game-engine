%module hare

%{
#include "LuaInputListener.h"
%}

class LuaInputObject : public ReferenceCounted
{
private:
    virtual void _doRelease();
};

LuaInputManager* createLuaInputManager(RenderWindow* window);

%newobject LuaInputManager::createMouse(bool);
%newobject LuaInputManager::createKeyboard(bool);
%newobject LuaInputManager::createJoyStick(bool);

class LuaInputManager : public LuaInputObject
{
private:
    LuaInputManager();
    
public:
    Mouse* createMouse(bool bufferMode);
    Keyboard* createKeyboard(bool bufferMode);
    JoyStick* createJoyStick(bool bufferMode);
};

class Mouse : public LuaInputObject
{
private:
    Mouse();
    
public:
    void setListener(LuaMouseListener* listener);
    LuaMouseListener* getListener();
    
    void capture();
};

class Keyboard : public LuaInputObject
{
private:
    Keyboard();
    
public:
    void setListener(LuaKeyListener* listener);
    LuaKeyListener* getListener();
    
    void capture();
};

class JoyStick : public LuaInputObject
{
private:
    JoyStick();
    
public:
    //void setListener(LuaJoyStickListener* listener);
    //LuaJoyStickListener* getListener();
};

class LuaKeyListener : public LuaInputObject
{
public:
    void setKeyPressedCallback(SWIGLUA_REF func);
    void setKeyReleasedCallback(SWIGLUA_REF func);
};

class LuaMouseListener : public LuaInputObject
{
public:
    void setMouseMovedCallback(SWIGLUA_REF func);
    void setMousePressedCallback(SWIGLUA_REF func);
    void setMouseReleasedCallback(SWIGLUA_REF func);
};

namespace OIS
{
    class EventArg
	{
	private:
		EventArg();
	};
	
    class KeyEvent : public EventArg
    {

    };

    class MouseEvent : public EventArg
    {

    };

    enum MouseButtonID
    {
	    MB_Left = 0, MB_Right, MB_Middle,
	    MB_Button3, MB_Button4,	MB_Button5, MB_Button6,	MB_Button7
    };
}

