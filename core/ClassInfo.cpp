//***************************************************************
//  File:    ClassInfo.cpp
//  Date:    09/29/2008
//  Author:  littlesome (littlesome@live.cn)
//-------------------------------------------------------------
//
//-------------------------------------------------------------
//  This file is part of Hare2D Game Engine.
//  Copyright (C) All Rights Reserved
//***************************************************************
//
//***************************************************************
#include "PCH.h"
#include "ClassInfo.h"

namespace hare
{
    ClassNameMap* ClassInfo::classMap = 0;

    ClassInfo::ClassInfo(const char* name, uint32 version, uint32 size, ClassInfo* base,
        ObjectConstructorFunc ctor)
            : className(name)
            , classVersion(version)
            , objectSize(size)
            , objectConstructor(ctor)
            , baseClass(base)
    {
        if (!classMap)
        {
            classMap = new ClassNameMap;
        }
        assert(!findClass(className));
        (*classMap)[className] = this;
    }

    ClassInfo::~ClassInfo()
    {
        if (classMap)
        {
            ClassNameMap::iterator it = classMap->find(className);
            if (it != classMap->end())
            {
                classMap->erase(it);
            }
            if (classMap->size() == 0)
            {
                delete classMap;
                classMap = 0;
            }
        }
    }

    ClassInfo* ClassInfo::findClass(const String &name)
    {
        if (!classMap)
            return 0;

        ClassNameMap::iterator it = classMap->find(name);
        if (it != classMap->end())
        {
            return it->second;
        }

        return 0;
    }


    void ClassInfo::findSubs(ClassInfoList& list)
    {
        list.clear();

        if (!classMap)
            return;

        ClassNameMap::iterator it = classMap->begin();

        for (; it != classMap->end(); ++it)
        {
            if (it->second->isDynamic() && it->second->isDerivedFrom(this))
                list.push_back(it->second);
        }
    }

}
