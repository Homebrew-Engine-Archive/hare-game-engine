//***************************************************************
//  File:    Object.h
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
#ifndef _OBJECT_H_
#define _OBJECT_H_

#include "CorePrerequisites.h"
#include "ReferenceCounted.h"

namespace hare
{
    class CORE_API Attribute : public ReferenceCounted
    {
        HARE_DECLARE_PTR(Attribute)
    public:

        enum AttributeType
        {
            attrNone,
            attrMeta,
            attrMetaArray,
            attrObject,
            attrObjectArray,
            attrObjectList,
        };

        bool hasFlag(uint32 flag) const
        {
            return (flags & flag) == flag;
        }

        AttributeType attrType;
        const char* name;
        uint32 flags;
        const char* typeName;
        void* data;
        ClassInfo* classInfo;
        EnumMap* enumMap;
        Object* owner;

        Attribute() : attrType(attrNone), name(0), flags(0),
            typeName(0), data(0), classInfo(0), enumMap(0), owner(0)
        {
        }

    private:
        virtual void _doRelease();
    };


    /** The base class for most of the engine objects
    */
    class CORE_API Object : public ReferenceCounted
    {
        HARE_DECLARE_DYNAMIC_CLASS(Object)

    public:
        enum StoreType
        {
            storeEmbedded,  // embeded object.
            storeReference, // reference a global object.
            storeNull,	    // null object pointer.
        };

        enum PropertyFlag
        {
            propHide            = 1 << 1,   // hide in editor
            propReadOnly        = 1 << 2,   // read only in editor
            propEnumIsFlag      = 1 << 3,   // enum is used as bit flag
            propFSUrl           = 1 << 4,   // string is url of file system
            propColor           = 1 << 5,   // edit this value as color, value type must be uint32
            propImport          = 1 << 6,   // object can be imported form a file
            propAvoidNull       = 1 << 7,   // object can not set null in editor
            propAllowSubs       = 1 << 8,   // object can be created from sub-classes in editor
        };

        bool isA(const ClassInfo* base) const;

        /** import Object from url, if share = false, object will be cloned.
        */
        static Object* importObject(const String &url, bool share = true);
        static Object* cloneObject(Object *obj);
        static Object* findByUrl(const String &url);
        static Object* loadFromXml(const String &url);
        static Object* loadFromBin(const String &url);

        bool saveToXml(const String &path);
        bool saveToBin(const String &path);

        void setUrl(const String& url)
        {
            objUrl = url;
        }

        const String& getUrl() const
        {
            return objUrl;
        }

        // event after one attribute is changed in editor..
        virtual void postEdited(Attribute *attr)
        {
        }

        // event after an object is loaded.
        virtual void postLoaded()
        {
        }

        // event after an object is saved.
        virtual void postSaved()
        {
        }

        virtual ~Object();

    protected:
        String objUrl;
    };

    typedef HashMap<String, Object*> ObjectUrlMap;

    CORE_API ObjectUrlMap& getObjectUrlMap();
    CORE_API void setObjUrl(Object *obj, const String& url);
    CORE_API const char* getVersionString();

    CORE_API void core_init(const char* argv0);
    CORE_API void core_quit();
}


#endif
