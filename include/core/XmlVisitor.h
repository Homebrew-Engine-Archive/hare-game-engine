//***************************************************************
//  File:    XmlVisitor.h
//  Date:    10/12/2008
//  Author:  littlesome (littlesome@live.cn)
//-------------------------------------------------------------
//
//-------------------------------------------------------------
//  This file is part of Hare2D Game Engine.
//  Copyright (C) All Rights Reserved
//***************************************************************
//
//***************************************************************
#ifndef _XMLVISITOR_H_
#define _XMLVISITOR_H_

#include "tinyxml/tinyxml.h"
#include "StringConverter.h"
#include "StringUtil.h"
#include "Object.h"
#include "Exception.h"

namespace hare_core
{
    class CORE_API XmlVisitor
    {
    public:
        enum VisitorAction
        {
            VA_Load,
            VA_Save,
        };
        TiXmlNode *node;
        u32 classVersion;
        VisitorAction action;

    public:
        void visitEnter(const char* name, bool super, u32 curVersion)
        {
            if (super)
            {
                String header = String("super_version_") + name;
                switch (action)
                {
                case VA_Load:
                    // load the version into classVersion
                    visitMeta(header.c_str(), classVersion, "u32", 0, 0);
                    break;
                case VA_Save:
                    // save the current version
                    visitMeta(header.c_str(), curVersion, "u32", 0, 0);
                    break;
                default:
                    assert(false);
                }
            }
        }

        template <typename T>
        void visitMeta(const char* name, T &obj, const char* typeName, u32 flags, EnumMap* enumMap)
        {
            switch (action)
            {
            case VA_Load:
            {
                TiXmlElement *elem = node->FirstChildElement(name);
                if (!elem)
                    return;

                String className = elem->Attribute("class");
                assert(className == typeName);

                const char* val = elem->Attribute("value");
                assert(val);
                StringConverter::parse(val, obj);
                break;
            }
            case VA_Save:
            {
                TiXmlElement *elem = node->InsertEndChild(TiXmlElement(name))->ToElement();
                elem->SetAttribute("class", typeName);
                elem->SetAttribute("value", StringConverter::toString(obj).c_str());
                break;
            }
            default:
                assert(false);
            }
        }

        template <typename T>
        void visitMetaArray(const char* name, std::vector<T> &obj, const char* typeName, u32 flags)
        {
            switch (action)
            {
            case VA_Load:
            {
                TiXmlElement *elem = node->FirstChildElement(name);
                if (!elem)
                    return;

                String className = elem->Attribute("class");
                assert(className == "meta_array");

                int num = 0;
                elem->Attribute("number", &num);
                obj.resize(num);

                XmlVisitor _v = *this;
                _v.node = elem;
                for (int i = 0; i < num; ++i)
                {
                    _v.visitMeta(StringUtil::format("item%d", i).c_str(), obj[i], typeName, flags, 0);
                }
                break;
            }
            case VA_Save:
            {
                TiXmlElement *elem = node->InsertEndChild(TiXmlElement(name))->ToElement();

                elem->SetAttribute("class", "meta_array");
                int num = (int)obj.size();
                elem->SetAttribute("number", num);

                XmlVisitor _v = *this;
                _v.node = elem;
                for (int i = 0; i < num; ++i)
                {
                    _v.visitMeta(StringUtil::format("item%d", i).c_str(), obj[i], typeName, flags, 0);
                }
                break;
            }
            default:
                assert(false);
            }
        }

        template <typename T>
        void visitObject(const char* name, T* &obj, ClassInfo *cls, u32 flags)
        {
            switch (action)
            {
            case VA_Load:
            {
                TiXmlElement *elem = node->FirstChildElement(name);

                if (!elem)
                    return;

                Object::StoreType store = Object::storeNull;
                elem->Attribute("store", (int*)&store);

                if (store == Object::storeEmbedded)
                {
                    const char* className = elem->Attribute("class");
                    int _classVersion = 0;
                    elem->Attribute("version", &_classVersion);

                    ClassInfo *cls = ClassInfo::findClass(className);
                    if (!cls)
                    {
                        HARE_EXCEPT(Exception::ERR_INTERNAL_ERROR,
                            "Cannot find runtime class : " + String(className), "XmlVisitor::visitObject()");
                    }

                    obj = (T*)cls->createObject();

                    // read attri.
                    XmlVisitor _v = *this;
                    _v.node = elem;
                    _v.classVersion = _classVersion;
                    obj->accept(_v);

                    ((Object*)obj)->postLoaded();
                }
                else if (store == Object::storeReference)
                {
                    const char* url = elem->Attribute("ref");
                    assert(url);
                    obj = (T*)Object::importObject(url);
                }
                else
                {
                    assert(store == Object::storeNull);
                }
                break;
            }
            case VA_Save:
            {
                TiXmlElement *elem = node->InsertEndChild(TiXmlElement(name))->ToElement();

                if (obj)
                {
                    if (obj->getUrl().empty())
                    {
                        elem->SetAttribute("store", (int)Object::storeEmbedded);
                        elem->SetAttribute("class", cls->className);
                        elem->SetAttribute("version", cls->classVersion);

                        XmlVisitor _v = *this;
                        _v.node = elem;
                        _v.classVersion = cls->classVersion;
                        obj->accept(_v);
                    }
                    else
                    {
                        elem->SetAttribute("store", (int)Object::storeReference);
                        elem->SetAttribute("ref", obj->getUrl().c_str());
                    }
                    ((Object*)obj)->postSaved();
                }
                else
                {
                    elem->SetAttribute("store", (int)Object::storeNull);
                }
                break;
            }
            default:
                assert(false);
            }
        }

        template <typename T>
        void visitObject(const char* name, Pointer<T> &obj, ClassInfo *cls, u32 flags)
        {
            T* tmpObj = 0;
            visitObject(name, tmpObj, cls, flags);
            obj = tmpObj;
        }

        template<typename T>
        void visitObjectArray(const char* name, std::vector<T> &obj, ClassInfo *cls, u32 flags)
        {
            _visitObjects<T, std::vector<T> >(name, obj, cls, flags, "object_array");
        }

        template<typename T>
        void visitObjectList(const char* name, std::list<T> &obj, ClassInfo *cls, u32 flags)
        {
            _visitObjects<T, std::list<T> >(name, obj, cls, flags, "object_list");
        }

    private:
        /* helper function for loading and saving objectarray / objectlist
         * T is the smart ptr
         * U is containtor of T (list / vector)
         */
        template <typename T, typename U>
        void _visitObjects(const char* name, U &obj, ClassInfo *cls, u32 flags, const char* clsName)
        {
            switch (action)
            {
            case VA_Load:
            {
                TiXmlElement *elem = node->FirstChildElement(name);
                if (!elem)
                    return; // can not found this element.

                String className = elem->Attribute("class");
                assert(className == clsName);

                int num = 0;
                elem->Attribute("number", &num);
                obj.resize(num);

                typename U::iterator it = obj.begin();
                for (; it != obj.end(); ++it)
                {
                    String itemName = "item" + StringConverter::toString((int)std::distance(obj.begin(), it));

                    XmlVisitor _v = *this;
                    _v.node = elem;

                    typename T::value_type itemObj = 0;
                    _v.visitObject(itemName.c_str(), itemObj, cls, flags);
                    *it = itemObj;
                }
                break;
            }
            case VA_Save:
            {
                // write class.
                TiXmlElement *elem = node->InsertEndChild(TiXmlElement(name))->ToElement();
                elem->SetAttribute("class", clsName);

                // write num.
                int num = (int)obj.size();
                elem->SetAttribute("number", num);

                // write all items.
                XmlVisitor _v = *this;
                _v.node = elem;

                typename U::iterator it = obj.begin();
                for (; it != obj.end(); ++it)
                {
                    String itemName = "item" + StringConverter::toString((int)std::distance(obj.begin(), it));
                    _v.visitObject(itemName.c_str(), (*it).pointerRef(), cls, flags);
                }
                break;
            }
            default:
                assert(false);
            }
        }
    };
}

#endif
