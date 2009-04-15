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
    template<typename T> class Singleton
    {
    public:
	    Singleton()
        {
		    assert(m_Singleton == 0);
		    m_Singleton = (T*)this;
	    }

	    ~Singleton()
        {}

	    static T& getSingleton()
        {
		    assert(m_Singleton != 0);
		    return *m_Singleton;
	    }

	    static T* getSingletonPtr()
        {
		    return m_Singleton;
	    }

    protected:
	    static T* m_Singleton;
    };

    template <typename T> T* Singleton<T>::m_Singleton = 0;

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
