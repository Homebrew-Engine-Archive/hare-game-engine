//***************************************************************
//  File:    AttVisitor.h
//  Data:    02/04/2009
//  Author:  littlesome (littlesome@live.cn)
//-------------------------------------------------------------
//
//-------------------------------------------------------------
//  This file is part of Hare2D Game Engine.
//  Copyright (C) All Rights Reserved
//***************************************************************
//
//***************************************************************
#ifndef _ATTVISITOR_H_
#define _ATTVISITOR_H_

#include "Object.h"

namespace hare
{
    /** AttVisitor for accessing object's properities
    */
    class CORE_API AttVisitor
    {
    public:
        Attribute::List attributes;
        Object* owner;

    public:
        AttVisitor() : owner(0)
        {
        }
        void visitEnter(const char* name, bool super, uint32 curVersion)
        {
        }

        template <typename T>
        void visitMeta(const char* name, T &obj, const char* typeName, uint32 flags, EnumMap* enumMap)
        {
            assert(owner);
            Attribute* attr = new Attribute;
            attr->attrType = Attribute::attrMeta;
            attr->name = name;
            attr->data = &obj;
            attr->typeName = typeName;
            attr->flags = flags;
            attr->enumMap = enumMap;
            attr->owner = owner;
            attributes.push_back(attr);
        }

        template <typename T>
        void visitMetaArray(const char* name, std::vector<T> &obj, const char* typeName, uint32 flags)
        {
            assert(owner);
            Attribute* attr = new Attribute;
            attr->attrType = Attribute::attrMetaArray;
            attr->name = name;
            attr->data = &obj;
            attr->typeName = typeName;
            attr->flags = flags;
            attr->owner = owner;
            attributes.push_back(attr);
        }

        template <typename T>
        void visitObject(const char* name, T* &obj, ClassInfo *cls, uint32 flags)
        {
            assert(owner);
            Attribute* attr = new Attribute;
            attr->attrType = Attribute::attrObject;
            attr->name = name;
            attr->data = &obj;
            attr->classInfo = cls;
            attr->flags = flags;
            attr->owner = owner;
            attributes.push_back(attr);
        }

        template <typename T>
        void visitObject(const char* name, Pointer<T> &obj, ClassInfo *cls, uint32 flags)
        {
            visitObject(name, obj.pointerRef(), cls, flags);
        }

        template<typename T>
        void visitObjectArray(const char* name, std::vector<T> &obj, ClassInfo *cls, uint32 flags)
        {
            assert(owner);
            Attribute* attr = new Attribute;
            attr->attrType = Attribute::attrObjectArray;
            attr->name = name;
            attr->data = &obj;
            attr->classInfo = cls;
            attr->flags = flags;
            attr->owner = owner;
            attributes.push_back(attr);
        }

        template<typename T>
        void visitObjectList(const char* name, std::list<T> &obj, ClassInfo *cls, uint32 flags)
        {
            assert(owner);
            Attribute* attr = new Attribute;
            attr->attrType = Attribute::attrObjectList;
            attr->name = name;
            attr->data = &obj;
            attr->classInfo = cls;
            attr->flags = flags;
            attr->owner = owner;
            attributes.push_back(attr);
        }
    };
}

#endif
