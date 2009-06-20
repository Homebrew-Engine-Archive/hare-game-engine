// ***************************************************************
//  File:   Singleton.h
//  Date:   2008-03-03 19:00
//  Author: littlesome
//  -------------------------------------------------------------
//
//  -------------------------------------------------------------
//  Copyright (C) 2008 - All Rights Reserved
// ***************************************************************
//
// ***************************************************************
#ifndef SINGLETON_H
#define SINGLETON_H

#include <assert.h>

namespace hare
{
    /** Template class for creating single-instance global classes.
    */
    template<typename T> class Singleton
    {
    public:
	    Singleton()
        {
		    assert(instance == 0);
		    instance = (T*)this;
	    }

	    static T& getSingleton()
        {
		    assert(instance != 0);
		    return *instance;
	    }

	    static T* getSingletonPtr()
        {
		    return instance;
	    }

    protected:
	    static T* instance;
    };

    template <typename T> T* Singleton<T>::instance = 0;

#define HARE_DECLARE_SINGLETON(CLASS)               \
    public:	static CLASS& getSingleton();           \
        static CLASS* getSingletonPtr();

#define HARE_IMPLEMENT_SINGLETON(CLASS)             \
    CLASS& CLASS::getSingleton()                    \
    { return Singleton<CLASS>::getSingleton(); }    \
    CLASS* CLASS::getSingletonPtr()                 \
    { return Singleton<CLASS>::getSingletonPtr(); }
}

#endif
