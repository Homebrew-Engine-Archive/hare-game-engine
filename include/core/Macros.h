//***************************************************************
//  File:    Macros.h
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
#ifndef _MACROS_H_
#define _MACROS_H_

#include "MathCommon.h"

/*-----------------------------------------------------------------------*/
/*  some commonly used macros						            		 */
/*-----------------------------------------------------------------------*/
#define SAFE_DELETE(p)       { if (p) { delete (p);     (p) = NULL; } }
#define SAFE_DELETE_ARRAY(p) { if (p) { delete[] (p);   (p) = NULL; } }
#define SAFE_RELEASE(p)      { if (p) { (p)->Release(); (p) = NULL; } }

/*-----------------------------------------------------------------------*/
/* macro to define a class without copy ctor nor assignment operator     */
/*-----------------------------------------------------------------------*/
#define HARE_DECLARE_NO_COPY_CLASS(classname)                             \
private:                                                                  \
    classname(const classname&);                                          \
    classname& operator=(const classname&);
/*-----------------------------------------------------------------------*/
#define HARE_DECLARE_NO_ASSIGN_CLASS(classname)                           \
private:                                                                  \
    classname& operator=(const classname&);

/*-----------------------------------------------------------------------*/
/* dynamic class macros                                                  */
/*-----------------------------------------------------------------------*/
#define HARE_DECLARE_PTR(name)                                            \
public:                                                                   \
    typedef Pointer<name> Ptr;                                            \
    typedef std::vector<Ptr> Array;                                       \
    typedef std::list<Ptr> List;
/*-----------------------------------------------------------------------*/
#define HARE_DECLARE_ABSTRACT_CLASS(name)                                 \
public:                                                                   \
    HARE_DECLARE_PTR(name)                                                \
    static ClassInfo CLASS_INFO;                                          \
    virtual ClassInfo *getClassInfo() const;                              \
    virtual void accept(XmlVisitor &visitor, bool super = false);         \
    virtual void accept(BinVisitor &visitor, bool super = false);         \
    virtual void accept(AttVisitor &visitor, bool super = false);         \
    template <typename T> void name##_accept(T &visitor);
/*-----------------------------------------------------------------------*/
#define HARE_DECLARE_DYNAMIC_CLASS(name)                                  \
    HARE_DECLARE_ABSTRACT_CLASS(name)                                     \
    static Object* createObject();                                        \
/*-----------------------------------------------------------------------*/
#define HARE_IMPLEMENT_CLASS_COMMON(name, base, func, ver)                \
    ClassInfo name::CLASS_INFO(#name, ver, (int)sizeof(name),             \
        &base::CLASS_INFO, (ObjectConstructorFunc)func);                  \
    ClassInfo *name::getClassInfo() const {                               \
        return &name::CLASS_INFO;                                         \
    }                                                                     \
    void name::accept(XmlVisitor &visitor, bool super) {                  \
        base::accept(visitor, true);                                      \
        uint32 savedVer = visitor.classVersion;                           \
        visitor.visitEnter(#name, super, CLASS_INFO.classVersion);        \
        name##_accept(visitor);                                           \
        visitor.classVersion = savedVer;                                  \
    }                                                                     \
    void name::accept(BinVisitor &visitor, bool super) {                  \
        base::accept(visitor, true);                                      \
        uint32 savedVer = visitor.classVersion;                           \
        visitor.visitEnter(#name, super, CLASS_INFO.classVersion);        \
        name##_accept(visitor);                                           \
        visitor.classVersion = savedVer;                                  \
    }                                                                     \
    void name::accept(AttVisitor &visitor, bool super) {                  \
        base::accept(visitor, true);                                      \
        visitor.owner = this;                                             \
        visitor.visitEnter(#name, super, CLASS_INFO.classVersion);        \
        name##_accept(visitor);                                           \
    }                                                                     \
    template <typename T> void name::name##_accept(T &visitor)
/*-----------------------------------------------------------------------*/
#define HARE_IMPLEMENT_ABSTRACT_CLASS(name, base, ver)                    \
    HARE_IMPLEMENT_CLASS_COMMON(name, base, 0, ver)
/*-----------------------------------------------------------------------*/
#define HARE_IMPLEMENT_DYNAMIC_CLASS(name, base, ver)                     \
    Object* name::createObject() {                                        \
        return new name;                                                  \
    }                                                                     \
    HARE_IMPLEMENT_CLASS_COMMON(name, base, name::createObject, ver)
/*-----------------------------------------------------------------------*/

#define HARE_ENUM_F(DATA, TYPE, ENUM, FLAG) visitor.visitMeta(#DATA, DATA, #TYPE, FLAG, getEnumValue<ENUM>());
#define HARE_META_F(DATA, TYPE, FLAG)       visitor.visitMeta(#DATA, DATA, #TYPE, FLAG, 0);
#define HARE_META_ARRAY_F(DATA, TYPE, FLAG) visitor.visitMetaArray(#DATA, DATA, #TYPE, FLAG);
#define HARE_OBJ_F(DATA, TYPE, FLAG)        visitor.visitObject(#DATA, DATA, &TYPE::CLASS_INFO, FLAG);
#define HARE_OBJ_ARRAY_F(DATA, TYPE, FLAG)  visitor.visitObjectArray(#DATA, DATA, &TYPE::CLASS_INFO, FLAG);
#define HARE_OBJ_LIST_F(DATA, TYPE, FLAG)   visitor.visitObjectList(#DATA, DATA, &TYPE::CLASS_INFO, FLAG);

#define HARE_ENUM(DATA, TYPE, ENUM)         HARE_ENUM_F(DATA, TYPE, ENUM, 0)
#define HARE_META(DATA, TYPE)               HARE_META_F(DATA, TYPE, 0)
#define HARE_META_ARRAY(DATA, TYPE)         HARE_META_ARRAY_F(DATA, TYPE, 0)
#define HARE_OBJ(DATA, TYPE)                HARE_OBJ_F(DATA, TYPE, 0)
#define HARE_OBJ_ARRAY(DATA, TYPE)          HARE_OBJ_ARRAY_F(DATA, TYPE, 0)
#define HARE_OBJ_LIST(DATA, TYPE)           HARE_OBJ_LIST_F(DATA, TYPE, 0)

#define HARE_ENUM_BEGIN(TYPE) \
template <> \
inline EnumMap* getEnumValue<TYPE>() { \
    static EnumMap* pEnumMap = 0; \
    if (!pEnumMap) { \
        static EnumMap theEnum; \
        pEnumMap = &theEnum; \
    }
#define HARE_ENUM_NAME_VALUE(NAME, VALUE) pEnumMap->insert(std::make_pair(#NAME, VALUE));
#define HARE_ENUM_VALUE(VALUE) pEnumMap->insert(std::make_pair(#VALUE, VALUE));
#define HARE_ENUM_END() \
    return pEnumMap; \
}

namespace hare
{
    typedef std::map<String, int32> EnumMap;

    template <typename T>
    inline EnumMap* getEnumValue() { return 0; }

    enum Bool
    {
        False,
        True,
    };

    HARE_ENUM_BEGIN(Bool)
        HARE_ENUM_VALUE(False)
        HARE_ENUM_VALUE(True)
    HARE_ENUM_END()
}

#endif
