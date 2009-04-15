//***************************************************************
//  File:    BinVisitor.h
//  Date:    10/13/2008
//  Author:  littlesome (littlesome@live.cn)
//-------------------------------------------------------------
//
//-------------------------------------------------------------
//  This file is part of Hare2D Game Engine.
//  Copyright (C) All Rights Reserved
//***************************************************************
//
//***************************************************************
#ifndef _BINVISITOR_H_
#define _BINVISITOR_H_

namespace hare
{
    class BinVisitor
    {
    public:
        enum VisitorAction
        {
            VA_Load,
            VA_Save,
        };
        std::iostream *stream;
        uint32 classVersion;
        VisitorAction action;

    public:
        void visitEnter(const char* name, bool super, uint32 curVersion)
        {
            if (super)
            {
                String header = String("super_version_") + name;
                switch (action)
                {
                case VA_Load:
                    // load the version into classVersion
                    visitMeta<uint32>(header.c_str(), classVersion, "uint32", 0, 0);
                    break;
                case VA_Save:
                    // save the current version
                    visitMeta<uint32>(header.c_str(), curVersion, "uint32", 0, 0);
                    break;
                default:
                    assert(false);
                }
            }
        }

        template <typename T>
        void visitMeta(const char* name, T &obj, const char* typeName, uint32 flags, EnumMap* enumMap)
        {
            switch (action)
            {
            case VA_Load:
            {
                stream->read((char*)&obj, sizeof(T));
                break;
            }
            case VA_Save:
            {
                stream->write((const char*)&obj, sizeof(T));
                break;
            }
            default:
                assert(false);
            }
        }

        template <typename T>
        void visitMetaArray(const char* name, std::vector<T> &obj, const char* typeName, uint32 flags)
        {
            switch (action)
            {
            case VA_Load:
            {
                int num = 0;
                stream->read((char*)&num, sizeof(num));
                obj.resize(num);
                if (num > 0)
                    stream->read((char*)(&(*obj.begin())), num * sizeof(T));
                break;
            }
            case VA_Save:
            {
                int num = (int)obj.size();
                stream->write((const char*)&num, sizeof(num));
                stream->write((const char*)(&(*obj.begin())), num * sizeof(T));
                break;
            }
            default:
                assert(false);
            }
        }

        template <typename T>
        void visitObject(const char* name, T* &obj, ClassInfo *cls, uint32 flags)
        {
            switch (action)
            {
            case VA_Load:
            {
                // read store type.
                Object::StoreType store = Object::storeNull;
                stream->read((char*)&store, sizeof(store));

                if (store == Object::storeEmbedded)
                {
                    // read class.
                    String className;
                    _visitMetaString(stream, className);

                    uint32 _classVersion = 0;
                    int objSize = 0;

                    stream->read((char*)&_classVersion, sizeof(_classVersion));
                    stream->read((char*)&objSize, sizeof(objSize));

                    ClassInfo *cls = ClassInfo::findClass(className.c_str());
                    if (!cls)
                    {

                    }
                    obj = (T*)cls->createObject();
                    BinVisitor _v = *this;
                    _v.classVersion = _classVersion;
                    obj->accept(_v);
                    ((Object*)obj)->postLoaded();
                }
                else if (store == Object::storeReference)
                {
                    String url;
                    _visitMetaString(stream, url);
                    assert(!url.empty());

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
                if (obj)
                {
                    if (obj->getUrl().empty())
                    {
                        Object::StoreType store = Object::storeEmbedded;
                        stream->write((const char*)&store, sizeof(store));
                        {
                            ClassInfo* cls = obj->getClassInfo();
                            String clsName = cls->className;
                            _visitMetaString(stream, clsName);
                            stream->write((const char*)&cls->classVersion, sizeof(cls->classVersion));
                        }
                        int objSize = -1;
                        // placeholder.
                        stream->write((const char*)&objSize, sizeof(objSize));
                        int posBegin = (int)stream->tellp();
                        {
                            BinVisitor _v = *this;
                            _v.classVersion = cls->classVersion;
                            obj->accept(_v);
                        }
                        // write object size.
                        objSize = (int)stream->tellp() - posBegin;
                        stream->seekp(posBegin - sizeof(int), std::ios::beg);
                        stream->write((const char*)&objSize, sizeof(objSize));
                        stream->seekp(0, std::ios::end);
                    }
                    else
                    {
                        Object::StoreType store = Object::storeReference;
                        stream->write((const char*)&store, sizeof(store));
                        String url = obj->getUrl();
                        _visitMetaString(stream, url);
                    }

                    ((Object*)obj)->postSaved();
                }
                else
                {
                    Object::StoreType store = Object::storeNull;
                    stream->write((const char*)&store, sizeof(store));
                }
                break;
            }
            default:
                assert(false);
            }
        }

        template <typename T>
        void visitObject(const char* name, Pointer<T> &obj, ClassInfo *cls, uint32 flags)
        {
            T* tmpObj = 0;
            visitObject(name, tmpObj, cls, flags);
            obj = tmpObj;
        }

        template<typename T>
        void visitObjectArray(const char* name, std::vector<T> &obj, ClassInfo *cls, uint32 flags)
        {
            _visitObjects<T, std::vector<T> >(name, obj, cls, flags);
        }

        template<typename T>
        void visitObjectList(const char* name, std::list<T> &obj, ClassInfo *cls, uint32 flags)
        {
            _visitObjects<T, std::list<T> >(name, obj, cls, flags);
        }

    private:
        inline void _visitMetaString(std::iostream* stream, String& str)
        {
            switch (action)
            {
            case VA_Load:
                {
                    int len = 0;
                    stream->read((char*)&len, sizeof(len));
                    str.resize(len);
                    if (len > 0)
                    {
                        stream->read(&(*str.begin()), len);
                    }
                    break;
                }
            case VA_Save:
                {
                    int len = (int)str.length();
                    stream->write((const char*)&len, sizeof(len));
                    if (len > 0)
                    {
                        stream->write(str.c_str(), len);
                    }
                    break;
                }
            default:
                assert(false);
            }
        }

        template <typename T, typename U>
        inline void _visitObjects(const char* name, U& obj, ClassInfo* cls, uint32 flags)
        {
            switch (action)
            {
            case VA_Load:
                {
                    int num = 0;
                    stream->read((char*)&num, sizeof(num));
                    obj.resize(num);

                    BinVisitor _v = *this;
                    typename U::iterator it = obj.begin();
                    for (; it != obj.end(); ++it)
                    {
                        typename T::value_type itemObj = 0;
                        _v.visitObject(0, itemObj, cls, flags);
                        *it = itemObj;
                    }
                    break;
                }
            case VA_Save:
                {
                    int num = (int)obj.size();
                    stream->write((const char*)&num, sizeof(num));
                    BinVisitor _v = *this;
                    typename U::iterator it = obj.begin();
                    for (; it != obj.end(); ++it)
                    {
                        _v.visitObject(0, (*it).pointerRef(), cls, flags);
                    }
                    break;
                }
            default:
                assert(false);
            }
        }
    };

    template <>
    inline void BinVisitor::visitMeta(const char* name, String &obj, const char* typeName, uint32 flags, EnumMap* enumMap)
    {
        _visitMetaString(stream, obj);
    }

    template <>
    inline void BinVisitor::visitMetaArray<String>(const char* name, StringVector &obj, const char* typeName, uint32 flags)
    {
        switch (action)
        {
        case VA_Load:
            {
                int num = 0;
                stream->read((char*)&num, sizeof(num));
                obj.resize(num);
                break;
            }
        case VA_Save:
            {
                int num = (int)obj.size();
                stream->write((const char*)&num, sizeof(num));
                break;
            }
        default:
            assert(false);
        }

        for (size_t i = 0; i < obj.size(); ++i)
            visitMeta(0, obj[i], "String", flags, 0);
    }

}

#endif

