//***************************************************************
//  File:    SocketHelper.h
//  Data:    02/23/2009
//  Author:  littlesome (littlesome@live.cn)
//-------------------------------------------------------------
//  
//-------------------------------------------------------------
//  This file is part of Hare2D Game Engine.
//  Copyright (C) All Rights Reserved
//***************************************************************
// 
//***************************************************************
#ifndef SOCKETHELPER_H
#define SOCKETHELPER_H

class SocketHelper
{
public:
    static bool writeCmd(Socket* socket, char cmd)
    {
        return socket->write((const char*)&cmd, sizeof(char)) == sizeof(char);
    }

    static bool writeInt(Socket* socket, int val)
    {
        return socket->write((const char*)&val, sizeof(int)) == sizeof(int);
    }

    static bool writeLong(Socket* socket, long val)
    {
        return socket->write((const char*)&val, sizeof(long)) == sizeof(long);
    }

    static bool writeString(Socket* socket, const String& str)
    {
        int len = (int)str.size();
        bool ok = socket->write((const char*)&len, sizeof(int)) == sizeof(int);
        if (ok && (len > 0))
            ok = socket->write(str.c_str(), len) == len;
        return ok;
    }

    static bool writeObject(Socket* socket, Object* object)
    {
        std::stringstream stream;
        BinVisitor visitor;
        visitor.stream = &stream;
        visitor.action = BinVisitor::VA_Save;
        visitor.visitObject(object->getClassInfo()->className, object, object->getClassInfo(), 0);

        int len = (int)stream.tellp();
        stream.seekg(0);
        bool ok = socket->write((const char*)&len, sizeof(int)) == sizeof(int);
        if (ok && (len > 0))
            ok = socket->write(stream.str().c_str(), len) == len;
        return ok;
    }

    static bool readCmd(Socket* socket, unsigned char& val)
    {
        unsigned char value = 0;
        bool ok = socket->read((char*)&value, sizeof(unsigned char)) == sizeof(unsigned char);
        if (ok) val = value;
        return ok;
    }

    static bool readInt(Socket* socket, int& val)
    {
        int value = 0;
        bool ok = socket->read((char*)&value, sizeof(int)) == sizeof(int);
        if (ok) val = value;
        return ok;
    }

    static bool readLong(Socket* socket, long& val)
    {
        long value = 0;
        bool ok = socket->read((char*)&value, sizeof(long)) == sizeof(long);
        if (ok) val = value;
        return ok;
    }

    static bool readString(Socket* socket, String& str)
    {
        String value;
        int len = 0;
        bool ok = socket->read((char*)&len, sizeof(int)) == sizeof(int);
        if (ok && (len > 0))
        {
            value.resize(len + 1, 0);
            ok = socket->read((char*)value.c_str(), len) == len;
        }
        if (ok) str = value;
        return ok;
    }

    static bool readObject(Socket* socket, Object*& object)
    {
        int len = 0;
        bool ok = socket->read((char*)&len, sizeof(int)) == sizeof(int);
        if (ok && (len > 0))
        {
            std::string str(len + 1, 0);
            ok = socket->read((char*)str.c_str(), len) == len;
            if (ok)
            {
                std::stringstream stream(str);
                BinVisitor visitror;
                visitror.stream = &stream;
                visitror.action = BinVisitor::VA_Load;
                visitror.visitObject("Object", object, 0, 0);
            }
        }
        return ok;
    }
};

#endif
