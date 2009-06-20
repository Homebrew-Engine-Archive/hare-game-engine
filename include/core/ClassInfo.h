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

    /** Class used for run-time type identification.
     */
    class CORE_API ClassInfo
    {
    public:
        ClassInfo(const char* name, uint32 version, uint32 size, ClassInfo* base,
                  ObjectConstructorFunc ctor);

        ~ClassInfo();

        /** Create an object using the default constructor.
        */
        Object *createObject() const
        {
            return objectConstructor ? (*objectConstructor)() : 0;
        }

        /** Test if the class is dynamic.
        */
        bool isDynamic() const
        {
            return 0 != objectConstructor;
        }

        /** Get class's name.
        */
        const char *getClassName() const
        {
            return className;
        }

        /** Get base class.
        */
        const ClassInfo* getBaseClass() const
        {
            return baseClass;
        }

        /** Get class's version.
        */
        uint32 getClassVersion() const
        {
            return classVersion;
        }

        /** Get class's size.
        */
        uint32 getSize() const
        {
            return objectSize;
        }

        /** Get class's constructor function.
        */
        ObjectConstructorFunc getConstructor() const
        {
            return objectConstructor;
        }

        /** Find class by name, return NULL if not found.
        */
        static ClassInfo *findClass(const String &name);

        /** Test if this class derived from some class.
        */
        bool isDerivedFrom(const ClassInfo* base) const
        {
            return base != 0 && (base == this || (baseClass && baseClass->isDerivedFrom(base)));
        }

        /** Find all subclasses derived form this.
        @remarks
            Currently only return dynamic classes. 
        */
        void findSubs(ClassInfoList& list);

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
