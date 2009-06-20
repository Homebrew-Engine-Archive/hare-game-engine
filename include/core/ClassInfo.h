//***************************************************************
//  File:    ClassInfo.h
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
#ifndef _CLASSINFO_H_
#define _CLASSINFO_H_

#include "CorePrerequisites.h"

namespace hare
{
    typedef HashMap<String, ClassInfo*> ClassNameMap;
    typedef Object* (*ObjectConstructorFunc)();

    typedef std::vector<ClassInfo*> ClassInfoList;

    /**
     */
    class CORE_API ClassInfo
    {
    public:
        ClassInfo(const char* name, uint32 version, uint32 size, ClassInfo* base,
                  ObjectConstructorFunc ctor);

        ~ClassInfo();

        Object *createObject() const
        {
            return objectConstructor ? (*objectConstructor)() : 0;
        }

        bool isDynamic() const
        {
            return 0 != objectConstructor;
        }

        const char *getClassName() const
        {
            return className;
        }

        const ClassInfo* getBaseClass() const
        {
            return baseClass;
        }

        uint32 getClassVersion() const
        {
            return classVersion;
        }

        uint32 getSize() const
        {
            return objectSize;
        }

        ObjectConstructorFunc getConstructor() const
        {
            return objectConstructor;
        }

        static ClassInfo *findClass(const String &name);

        bool isDerivedFrom(const ClassInfo* base) const
        {
            return base != 0 && (base == this || (baseClass && baseClass->isDerivedFrom(base)));
        }

        int findSubs(ClassInfoList& list);

    public:
        const char*             className;
        uint32                  classVersion;
        uint32                  objectSize;
        ObjectConstructorFunc   objectConstructor;
    private:
        const ClassInfo*        baseClass;
        static ClassNameMap*    classMap;

        HARE_DECLARE_NO_COPY_CLASS(ClassInfo)
    };
}

#endif
