#ifndef _PYSCENELISTENER_H_
#define _PYSCENELISTENER_H_

#include "graphics/Graphics.h"

class PySceneListener : public SceneListener
{
public:
    PySceneListener()
    {
        begin = NULL;
        render = NULL;
        end = NULL;
    }

    virtual ~PySceneListener()
    {
        Py_XDECREF(begin);
        Py_XDECREF(render);
        Py_XDECREF(end);
    }

    virtual void beginScene()
    {
        call(begin);
    }

    virtual void renderScene()
    {
        call(render);
    }

    virtual void endScene()
    {
        call(end);
    }

    void call(PyObject* func)
    {
        if (func == 0)
            return;

        PyObject* ret = PyObject_CallObject(func, NULL);
        if (!ret)
        {
            PyErr_Print();
        }
        Py_XDECREF(ret);
    }

	void setBeginSceneCallback(PyObject* func)
	{
        Py_XDECREF(begin);
		begin = func;
        Py_XINCREF(begin);
	}

	void setRenderSceneCallback(PyObject* func)
	{
        Py_XDECREF(render);
        render = func;
        Py_XINCREF(render);
	}

	void setEndSceneCallback(PyObject* func)
	{
        Py_XDECREF(end);
        end = func;
        Py_XINCREF(end);
	}

protected:
	PyObject* begin;
	PyObject* render;
	PyObject* end;
};

#endif
