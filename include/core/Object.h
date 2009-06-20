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

        /// Enum identifying object storage types
        enum StoreType
        {
            /// Embeded object.
            storeEmbedded,  
            /// Reference a global object.
            storeReference, 
            /// Null object pointer.
            storeNull,	    
        };

        /// Enum identifying object property flags
        enum PropertyFlag
        {
            /// Hide this property in editor.
            propHide = 1 << 1,   
            /// Property is read-only in editor.
            propReadOnly = 1 << 2,   
            /// EnumProperty is used as bit flag.
            propEnumIsFlag = 1 << 3,
            /// String is url of file system.
            propFSUrl = 1 << 4,
            /// Property is color, must be uint32.
            propColor = 1 << 5,
            /// Object can be imported form a file.
            propImport = 1 << 6,
            /// Object can not set null in editor.
            propAvoidNull = 1 << 7,
            /// Object can be created from sub-classes in editor.
            propAllowSubs = 1 << 8,
        };

        /** If this object is derived from some class.
        */
        bool isA(const ClassInfo* base) const;

        /** Import object from url, if <code>share</code> is false, object will be cloned.
            This function auto detects the file's format (xml or binary).
        */
        static Object* importObject(const String &url, bool share = true);

        /** Clone object from existing object.
        */
        static Object* cloneObject(Object *obj);

        /** Find object by url.
        */
        static Object* findByUrl(const String &url);

        /** Load object from xml file, same as importObject(url, false).
        */
        static Object* loadFromXml(const String &url);

        /** Load object from xml file, same as importObject(url, false).
        */
        static Object* loadFromBin(const String &url);

        /** Save object to file in xml format.
        */
        bool saveToXml(const String &path);

        /** Save object to file in binary format.
        */
        bool saveToBin(const String &path);

        /** Set object's url.
        */
        void setUrl(const String& url)
        {
            objUrl = url;
        }

        /** Get object's url.
        */
        const String& getUrl() const
        {
            return objUrl;
        }

        /** Event after one attribute is changed in editor.
        */
        virtual void postEdited(Attribute *attr)
        {
        }

        /** Event after an object is loaded.
        */
        virtual void postLoaded()
        {
        }

        /** Event after an object is saved.
        */
        virtual void postSaved()
        {
        }

        virtual ~Object();

    protected:
        virtual void _doRelease();

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
