//***************************************************************
//  File:    LuaInputListener.h
//  Data:    05/23/2009
//  Author:  littlesome (littlesome@live.cn)
//-------------------------------------------------------------
//  
//-------------------------------------------------------------
//  This file is part of Hare2D Game Engine.
//  Copyright (C) All Rights Reserved
//***************************************************************
// 
//***************************************************************
#ifndef LUAINPUTLISTENER_H
#define LUAINPUTLISTENER_H

#include "OIS.h"

class LuaInputObject : public ReferenceCounted
{
    HARE_DECLARE_PTR(LuaInputObject)
protected:
    virtual void _doRelease()
    {
        delete this;
    }
};

class LuaKeyListener : public LuaInputObject, public OIS::KeyListener
{
    HARE_DECLARE_PTR(LuaKeyListener)
public:
    LuaKeyListener::LuaKeyListener()
    {
        pressed.L = 0; pressed.ref = 0;
        released.L = 0; released.ref = 0;
    }

    void setKeyPressedCallback(SWIGLUA_REF func)
    {
        pressed = func;
    }

    void setKeyReleasedCallback(SWIGLUA_REF func)
    {
        released = func;
    }

protected:
    void processKeyEvent(SWIGLUA_REF& func, const OIS::KeyEvent& event)
    {
        if (func.L == 0)
            return;

        swiglua_ref_get(&func);

        SWIG_NewPointerObj(func.L, &event, SWIGTYPE_p_OIS__KeyEvent, 0);

        if (lua_pcall(func.L, 1, LUA_MULTRET, 0))
            notify_error(func.L);
    }

    virtual bool keyPressed(const OIS::KeyEvent& event)
    {
        processKeyEvent(pressed, event);
        return true;
    }
    virtual bool keyReleased(const OIS::KeyEvent& event)
    {
        processKeyEvent(released, event);
        return true;
    }

    SWIGLUA_REF pressed;
    SWIGLUA_REF released;
};

class LuaMouseListener : public LuaInputObject, public OIS::MouseListener
{
    HARE_DECLARE_PTR(LuaMouseListener)
public:
    LuaMouseListener::LuaMouseListener()
    {
        moved.L = 0; moved.ref = 0;
        pressed.L = 0; pressed.ref = 0;
        released.L = 0; released.ref = 0;
    }

    void setMouseMovedCallback(SWIGLUA_REF func)
    {
        moved = func;
    }

    void setMousePressedCallback(SWIGLUA_REF func)
    {
        pressed = func;
    }

    void setMouseReleasedCallback(SWIGLUA_REF func)
    {
        released = func;
    }

protected:

    void processKeyEvent(SWIGLUA_REF& func, const OIS::MouseEvent& event, OIS::MouseButtonID id)
    {
        if (func.L == 0)
            return;

        swiglua_ref_get(&func);

        SWIG_NewPointerObj(func.L, &event, SWIGTYPE_p_OIS__MouseEvent, 0);
        lua_pushnumber(func.L, (lua_Number)id);

        if (lua_pcall(func.L, 2, LUA_MULTRET, 0))
            notify_error(func.L);
    }

    virtual bool mouseMoved(const OIS::MouseEvent &event)
    {
        processKeyEvent(moved, event, (OIS::MouseButtonID)-1);
        return true;
    }

    virtual bool mousePressed(const OIS::MouseEvent &event, OIS::MouseButtonID id)
    {
        processKeyEvent(pressed, event, id);
        return true;
    }

    virtual bool mouseReleased(const OIS::MouseEvent &event, OIS::MouseButtonID id)
    {
        processKeyEvent(released, event, id);
        return true;
    }

    SWIGLUA_REF moved;
    SWIGLUA_REF pressed;
    SWIGLUA_REF released;
};

class Mouse;
class Keyboard;
class JoyStick;

class LuaInputManager : public LuaInputObject
{
    HARE_DECLARE_PTR(LuaInputManager)
public:
    static LuaInputManager* createLuaInputManager(RenderWindow* window)
    {
        if (window)
        {
            OIS::InputManager* inputManager = OIS::InputManager::createInputSystem((size_t)window->getWindowParams().hwnd);

            if (inputManager)
            {
                return new LuaInputManager(inputManager);
            }
        }
        return NULL;
    }

    ~LuaInputManager()
    {
        OIS::InputManager::destroyInputSystem(inputManager);
    }

    void destroyInputObject(OIS::Object* obj)
    {
        inputManager->destroyInputObject(obj);
    }

protected:
    LuaInputManager(OIS::InputManager* im) : inputManager(im)
    {
    }

public:
    Mouse* createMouse(bool bufferMode);
    Keyboard* createKeyboard(bool bufferMode);
    JoyStick* createJoyStick(bool bufferMode);
protected:
    OIS::InputManager* inputManager;
};

LuaInputManager* createLuaInputManager(RenderWindow* window)
{
    return LuaInputManager::createLuaInputManager(window);
}

class Mouse : public LuaInputObject
{
public:
    Mouse(LuaInputManager* im, OIS::Mouse* mouse) : inputManager(im), mouse(mouse) 
    {
    }

    virtual ~Mouse()
    {
        if (inputManager && mouse)
            inputManager->destroyInputObject(mouse);
    }

    void setListener(LuaMouseListener* listener)
    {
        mouse->setEventCallback((OIS::MouseListener*)listener);
        mouseListener = listener;
    }

    LuaMouseListener* getListener()
    {
        return mouseListener;
    }

    void capture()
    {
        mouse->capture();
    }

protected:
    LuaMouseListener::Ptr mouseListener;
    LuaInputManager::Ptr inputManager;
    OIS::Mouse* mouse;
};

class Keyboard : public LuaInputObject
{
public:
    Keyboard(LuaInputManager* im, OIS::Keyboard* keyboard) : inputManager(im), keyboard(keyboard) 
    {
    }
    virtual ~Keyboard()
    {
        if (inputManager && keyboard)
            inputManager->destroyInputObject(keyboard);
    }
    void setListener(LuaKeyListener* listener)
    {
        keyboard->setEventCallback((OIS::KeyListener*)listener);
        keyListener = listener;
    }
    LuaKeyListener* getListener()
    {
        return keyListener;
    }

    void capture()
    {
        keyboard->capture();
    }


protected:
    LuaKeyListener::Ptr keyListener;
    LuaInputManager::Ptr inputManager;
    OIS::Keyboard* keyboard;
};

class JoyStick : public LuaInputObject
{
public:
    JoyStick(LuaInputManager* im, OIS::JoyStick* joystick) : inputManager(im), joystick(joystick) 
    {
    }
    virtual ~JoyStick()
    {
        if (inputManager && joystick)
            inputManager->destroyInputObject(joystick);
    }
    //void setListener(JoyStickListener* listener);
    //JoyStickListener* getListener();

protected:
    //JoyStickListenerPtr joystickListener;
    LuaInputManager::Ptr inputManager;
    OIS::JoyStick* joystick;
};

Mouse* LuaInputManager::createMouse(bool bufferMode)
{
    OIS::Mouse* mouse = (OIS::Mouse*)inputManager->createInputObject(OIS::OISMouse, bufferMode);
    if (mouse)
        return new Mouse(this, mouse);
    else
        return NULL;
}
Keyboard* LuaInputManager::createKeyboard(bool bufferMode)
{
    OIS::Keyboard* keyboard = (OIS::Keyboard*)inputManager->createInputObject(OIS::OISKeyboard, bufferMode);
    if (keyboard)
        return new Keyboard(this, keyboard);
    else
        return NULL;
}
JoyStick* LuaInputManager::createJoyStick(bool bufferMode)
{
    OIS::JoyStick* joystick = (OIS::JoyStick*)inputManager->createInputObject(OIS::OISJoyStick, bufferMode);
    if (joystick)
        return new JoyStick(this, joystick);
    else
        return NULL;
}

#endif