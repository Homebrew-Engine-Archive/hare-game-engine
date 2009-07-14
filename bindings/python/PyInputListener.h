//***************************************************************
//  File:    PyInputListener.h
//  Data:    05/30/2009
//  Author:  littlesome (littlesome@live.cn)
//-------------------------------------------------------------
//  
//-------------------------------------------------------------
//  This file is part of Hare2D Game Engine.
//  Copyright (C) All Rights Reserved
//***************************************************************
// 
//***************************************************************
#ifndef PYINPUTLISTENER_H
#define PYINPUTLISTENER_H

#include "OIS.h"

bool notify_error();

class PyInputObject : public ReferenceCounted
{
    HARE_DECLARE_PTR(PyInputObject)
protected:
    virtual void _doRelease()
    {
        delete this;
    }
};

class PyKeyListener : public PyInputObject, public OIS::KeyListener
{
    HARE_DECLARE_PTR(PyKeyListener)
public:
    PyKeyListener()
    {
        pressed = NULL;
        released = NULL;
    }

    virtual ~PyKeyListener()
    {
        Py_XDECREF(pressed);
        Py_XDECREF(released);
    }

    void setKeyPressedCallback(PyObject* func)
    {
        Py_XDECREF(pressed);
        pressed = func;
        Py_XINCREF(pressed);
    }

    void setKeyReleasedCallback(PyObject* func)
    {
        Py_XDECREF(released);
        released = func;
        Py_XINCREF(released);
    }

protected:
    void processKeyEvent(PyObject* func, const OIS::KeyEvent& event)
    {
        if (func == 0)
            return;

        PyObject* e = SWIG_NewPointerObj((void*)&event, SWIGTYPE_p_OIS__KeyEvent, 0);

        PyObject* ret = PyObject_CallFunction(func, "O", e);

        if (!ret)
            notify_error();

        Py_XDECREF(ret);
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

    PyObject* pressed;
    PyObject* released;
};

class PyMouseListener : public PyInputObject, public OIS::MouseListener
{
    HARE_DECLARE_PTR(PyMouseListener)
public:
    PyMouseListener::PyMouseListener()
    {
        moved = NULL;
        pressed = NULL;
        released = NULL;
    }

    void setMouseMovedCallback(PyObject* func)
    {
        moved = func;
    }

    void setMousePressedCallback(PyObject* func)
    {
        pressed = func;
    }

    void setMouseReleasedCallback(PyObject* func)
    {
        released = func;
    }

protected:

    void processKeyEvent(PyObject*& func, const OIS::MouseEvent& event, OIS::MouseButtonID id)
    {
        if (func == 0)
            return;

        PyObject* e = SWIG_NewPointerObj((void*)&event, SWIGTYPE_p_OIS__MouseEvent, 0);

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

    PyObject* moved;
    PyObject* pressed;
    PyObject* released;
};

class Mouse;
class Keyboard;
class JoyStick;

class PyInputManager : public PyInputObject
{
    HARE_DECLARE_PTR(PyInputManager)
public:
    static PyInputManager* createPyInputManager(RenderWindow* window)
    {
        if (window)
        {
            size_t winHandle = 0;
            if (!window->getCustomData("WINDOW", &winHandle))
                return NULL;
            OIS::InputManager* inputManager = OIS::InputManager::createInputSystem(winHandle);

            if (inputManager)
            {
                return new PyInputManager(inputManager);
            }
        }
        return NULL;
    }

    ~PyInputManager()
    {
        OIS::InputManager::destroyInputSystem(inputManager);
    }

    void destroyInputObject(OIS::Object* obj)
    {
        inputManager->destroyInputObject(obj);
    }

protected:
    PyInputManager(OIS::InputManager* im) : inputManager(im)
    {
    }

public:
    Mouse* createMouse(bool bufferMode);
    Keyboard* createKeyboard(bool bufferMode);
    JoyStick* createJoyStick(bool bufferMode);
protected:
    OIS::InputManager* inputManager;
};

PyInputManager* createPyInputManager(RenderWindow* window)
{
    return PyInputManager::createPyInputManager(window);
}

class Mouse : public PyInputObject
{
public:
    Mouse(PyInputManager* im, OIS::Mouse* mouse) : inputManager(im), mouse(mouse) 
    {
    }

    virtual ~Mouse()
    {
        if (inputManager && mouse)
            inputManager->destroyInputObject(mouse);
    }

    void setListener(PyMouseListener* listener)
    {
        mouse->setEventCallback((OIS::MouseListener*)listener);
        mouseListener = listener;
    }

    PyMouseListener* getListener()
    {
        return mouseListener;
    }

    const OIS::MouseState& getMouseState() const
    {
        return mouse->getMouseState();
    }

    void capture()
    {
        mouse->capture();
    }

protected:
    PyMouseListener::Ptr mouseListener;
    PyInputManager::Ptr inputManager;
    OIS::Mouse* mouse;
};

class Keyboard : public PyInputObject
{
public:
    Keyboard(PyInputManager* im, OIS::Keyboard* keyboard) : inputManager(im), keyboard(keyboard) 
    {
    }
    virtual ~Keyboard()
    {
        if (inputManager && keyboard)
            inputManager->destroyInputObject(keyboard);
    }
    void setListener(PyKeyListener* listener)
    {
        keyboard->setEventCallback((OIS::KeyListener*)listener);
        keyListener = listener;
    }
    PyKeyListener* getListener()
    {
        return keyListener;
    }

    void capture()
    {
        keyboard->capture();
    }


protected:
    PyKeyListener::Ptr keyListener;
    PyInputManager::Ptr inputManager;
    OIS::Keyboard* keyboard;
};

class JoyStick : public PyInputObject
{
public:
    JoyStick(PyInputManager* im, OIS::JoyStick* joystick) : inputManager(im), joystick(joystick) 
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
    PyInputManager::Ptr inputManager;
    OIS::JoyStick* joystick;
};

Mouse* PyInputManager::createMouse(bool bufferMode)
{
    OIS::Mouse* mouse = (OIS::Mouse*)inputManager->createInputObject(OIS::OISMouse, bufferMode);
    if (mouse)
        return new Mouse(this, mouse);
    else
        return NULL;
}
Keyboard* PyInputManager::createKeyboard(bool bufferMode)
{
    OIS::Keyboard* keyboard = (OIS::Keyboard*)inputManager->createInputObject(OIS::OISKeyboard, bufferMode);
    if (keyboard)
        return new Keyboard(this, keyboard);
    else
        return NULL;
}
JoyStick* PyInputManager::createJoyStick(bool bufferMode)
{
    OIS::JoyStick* joystick = (OIS::JoyStick*)inputManager->createInputObject(OIS::OISJoyStick, bufferMode);
    if (joystick)
        return new JoyStick(this, joystick);
    else
        return NULL;
}

#endif