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

namespace hare_core
{
    ClassNameMap* ClassInfo::classMap = 0;

    ClassInfo::ClassInfo(const char* name, u32 version, u32 size, ClassInfo* base,
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


    int ClassInfo::findSubs(std::vector<ClassInfo*>& list)
    {
        int num = 0;

        if (!classMap)
            return num;

        ClassNameMap::iterator it = classMap->begin();

        for (; it != classMap->end(); ++it)
        {
            if (it->second->isDerivedFrom(this))
                list.push_back(it->second);
        }

        return num;
    }

}
