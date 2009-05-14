//***************************************************************
//  File:    Object.cpp
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
#include "Object.h"
#include "ClassInfo.h"
#include "XmlVisitor.h"
#include "BinVisitor.h"
#include "AttVisitor.h"
#include "FileSystem.h"
#include "Exception.h"

namespace hare
{
    const char* getVersionString()
    {
        return "Hare 1.0.0";
    }

    ObjectUrlMap& getObjectUrlMap()
    {
        static ObjectUrlMap objects;
        return objects;
    }

    void setObjUrl(Object *obj, const String& url)
    {
        if (!obj->getUrl().empty())
        {
            ObjectUrlMap::iterator it = getObjectUrlMap().find(obj->getUrl());
            if (it != getObjectUrlMap().end())
                getObjectUrlMap().erase(it);
        }

        obj->setUrl(url);
        if (!url.empty())
            getObjectUrlMap()[obj->getUrl()] = obj;
    }

    void Attribute::_doRelease()
    {
        delete this;
    }

    Object::~Object()
    {
        setObjUrl(this, StringUtil::EMPTY);
    }

    /* special RTTI for base class Object
    */
    void Object::_doRelease()
    {
        delete this;
    }

    Object* Object::createObject()
    {
        return new Object;
    }

    ClassInfo Object::CLASS_INFO("Object", 0, (int)sizeof(Object),  0, (ObjectConstructorFunc)createObject);

    ClassInfo* Object::getClassInfo() const
    {
        return &CLASS_INFO;
    }

    void Object::accept(XmlVisitor &visitor, bool super)
    {
        uint32 savedVer = visitor.classVersion;

        // do not write this <_super_version_Object class="uint32" value="0" />
        /* visitor.visitEnter("Object", super, CLASS_INFO.classVersion); */
        Object_accept(visitor);

        visitor.classVersion = savedVer;
    }

    void Object::accept(BinVisitor &visitor, bool super)
    {
        uint32 savedVer = visitor.classVersion;
        visitor.visitEnter("Object", super, CLASS_INFO.classVersion);
        Object_accept(visitor);
        visitor.classVersion = savedVer;
    }

    void Object::accept(AttVisitor &visitor, bool super)
    {
        visitor.visitEnter("Object", super, CLASS_INFO.classVersion);
        Object_accept(visitor);
    }

    template <typename T> void Object::Object_accept(T &visitor)
    {
    }

    Object* Object::importObject(const String &url, bool share)
    {
        Object *obj = findByUrl(url);

        if (obj)
        {
            if (!share)
                obj = cloneObject(obj);
        }
        else
        {
            FileSystem* fs = FileSystem::getSingletonPtr();
            FileHandle fh = fs->openFile(url, FM_Read);
            bool isXml = true;

            if (fh)
            {
                int size = fs->size(fh);
                if (size >= 3)
                {
                    char header[3] = { 0 };
                    fs->readFile(fh, header, 3, 1);
                    if (strncmp(header, "BIN", 3) == 0)
                    {
                        isXml = false;
                    }
                }
                fs->closeFile(fh);

                if (isXml)
                    obj = loadFromXml(url);
                else
                    obj = loadFromBin(url);
            }
        }

        return obj;
    }

    Object* Object::loadFromXml(const String &url)
    {
        Object *obj = 0;

        TiXmlDocument doc;
        static char sBuffer[40960];

        FileSystem* fs = FileSystem::getSingletonPtr();
        FileHandle fh = fs->openFile(url, FM_Read);

        if (fh)
        {
            int size = fs->size(fh);
            if (size > 0)
            {
                char* buffer = 0;
                if (size >= 40960)
                    buffer = new char[size + 1];
                else
                    buffer = sBuffer;

                fs->readFile(fh, buffer, size, 1);
                buffer[size] = 0;
                fs->closeFile(fh);
                doc.Parse(buffer);

                if (buffer != sBuffer)
                    delete [] buffer;

                XmlVisitor visitror;
                visitror.node = &doc;
                visitror.action = XmlVisitor::VA_Load;
                visitror.visitObject("Object", obj, 0, 0);

                if (obj)
                {
                    setObjUrl(obj, url);
                }
            }
            else
                fs->closeFile(fh);
        }
        else
        {
            //HARE_EXCEPT(Exception::ERR_FILE_NOT_FOUND, "Cannot open file : " + url, "Object::loadFromXml()");
        }

        return obj;
    }

    Object* Object::loadFromBin(const String &url)
    {
        Object *obj = 0;

        FileSystem* fs = FileSystem::getSingletonPtr();
        FileHandle fh = fs->openFile(url, FM_Read);

        if (fh)
        {
            int size = fs->size(fh);
            if (size > 0)
            {
                std::string str(size + 1, 0);
                fs->readFile(fh, (char*)str.c_str(), size, 1);
                fs->closeFile(fh);
                std::stringstream stream(str);

                char header[3];
                stream.read(header, 3);
                if (strncmp(header, "BIN", 3) != 0)
                {
                    assert(false);
                }

                BinVisitor visitror;
                visitror.stream = &stream;
                visitror.action = BinVisitor::VA_Load;
                visitror.visitObject("Object", obj, 0, 0);

                if (obj)
                {
                    setObjUrl(obj, url);
                }
            }
            else
                fs->closeFile(fh);
        }
        else
        {
            //HARE_EXCEPT(Exception::ERR_FILE_NOT_FOUND, "Cannot open file : " + url, "Object::loadFromXml()");
        }

        return obj;
    }

    Object* Object::cloneObject(Object *obj)
    {
        if (!obj)
            return 0;

        std::stringstream stream;

        BinVisitor visitor;
        visitor.stream = &stream;
        visitor.action = BinVisitor::VA_Save;

        String savedUrl = obj->getUrl();
        obj->setUrl(StringUtil::EMPTY);

        visitor.visitObject(obj->getClassInfo()->className, obj, obj->getClassInfo(), 0);

        obj->setUrl(savedUrl);

        stream.seekg(0);

        visitor.action = BinVisitor::VA_Load;

        Object *newObj = 0;
        visitor.visitObject(0, newObj, 0, 0);

        return newObj;
    }

    Object* Object::findByUrl(const String &url)
    {
        ObjectUrlMap::iterator it = getObjectUrlMap().find(url);

        if (it != getObjectUrlMap().end())
        {
            return it->second;
        }

        return 0;
    }

    bool Object::isA(const ClassInfo* base) const
    {
        return getClassInfo()->isDerivedFrom(base);
    }

    bool Object::saveToXml(const String &path)
    {
        if (path.empty())
            return false;

        String savePath = path;
        String savedUrl = getUrl();
        setUrl(StringUtil::EMPTY);

        bool ret = true;

        try
        {
            TiXmlDocument doc;
            TiXmlDeclaration dec("1.0", "utf-8", "yes");
            doc.InsertEndChild(dec);

            XmlVisitor visitor;
            visitor.node = &doc;
            visitor.action = XmlVisitor::VA_Save;

            // write object content
            Object* obj = this;
            visitor.visitObject("Object", obj, getClassInfo(), 0);

            TiXmlPrinter printer;
            doc.Accept(&printer);

            FileSystem* fs = FileSystem::getSingletonPtr();
            FileHandle fh = fs->openFile(savePath, FM_Write);
            if (fh)
            {
                fs->writeFile(fh, printer.CStr(), (int)printer.Size(), 1);
                fs->closeFile(fh);
            }
        }
        catch (...)
        {
            ret = false;
        }

        // restore url.
        setUrl(savedUrl);

        return ret;
    }

    bool Object::saveToBin(const String &path)
    {
        if (path.empty())
            return false;

        String savePath = path;
        String savedUrl = getUrl();
        setUrl(StringUtil::EMPTY);

        bool ret = true;
        try
        {
            std::stringstream stream;

            stream.write("BIN", 3);

            BinVisitor visitor;
            visitor.stream = &stream;
            visitor.action = BinVisitor::VA_Save;

            Object *obj = this;
            visitor.visitObject(getClassInfo()->className, obj, getClassInfo(), 0);

            size_t size = stream.tellp();
            stream.seekg(0);

            FileSystem* fs = FileSystem::getSingletonPtr();
            FileHandle fh = fs->openFile(savePath, FM_Write);
            if (fh)
            {
                fs->writeFile(fh, stream.str().c_str(), size, 1);
                fs->closeFile(fh);
            }
        }
        catch (...)
        {
            ret = false;
        }

        setUrl(savedUrl);

        return ret;
    }
}
