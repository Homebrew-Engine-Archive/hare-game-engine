//***************************************************************
//  File:    FileSystem.cpp
//  Data:    12/11/2008
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
#include "FileSystem.h"
#include "StringUtil.h"
#include <physfs.h>

namespace hare
{
    HARE_IMPLEMENT_SINGLETON(FileSystem)

    FileSystem* getFileSystem()
    {
        return FileSystem::getSingletonPtr();
    }

    FileSystem::FileSystem(const char* argv0)
    {
        PHYSFS_init(argv0);
    }

    FileSystem::~FileSystem()
    {
        PHYSFS_deinit();
    }

    bool FileSystem::addSearchPath(const String& dir)
    {
        return PHYSFS_addToSearchPath(dir.c_str(), 1) ? true : false;
    }

    bool FileSystem::removeSearchPath(const String& dir)
    {
        return PHYSFS_removeFromSearchPath(dir.c_str()) ? true : false;
    }

    const char* FileSystem::getUserDir()
    {
        return PHYSFS_getUserDir();
    }

    const char* FileSystem::getBaseDir()
    {
        return PHYSFS_getBaseDir();
    }

    const char* FileSystem::getLastError()
    {
        return PHYSFS_getLastError();
    }

    const char* FileSystem::getWriteDir()
    {
        return PHYSFS_getWriteDir();
    }

    bool FileSystem::setWriteDir(const String& dir)
    {
        return PHYSFS_setWriteDir(dir.c_str()) ? true : false;
    }

    const char* FileSystem::getRealDir(const String& dir)
    {
        return PHYSFS_getRealDir(dir.c_str());
    }

    bool FileSystem::exists(const String& path)
    {
        return PHYSFS_exists(path.c_str()) ? true : false;
    }

    bool FileSystem::isDir(const String& path)
    {
        return PHYSFS_isDirectory(path.c_str()) ? true : false;
    }

    bool FileSystem::isFile(const String& path)
    {
        return !isDir(path);
    }

    bool FileSystem::mkDir(const String& path)
    {
        return PHYSFS_mkdir(path.c_str()) ? true : false;
    }

    bool FileSystem::remove(const String& path)
    {
        return PHYSFS_delete(path.c_str()) ? true : false;
    }

    StringVector FileSystem::enumFiles(const String& path)
    {
        StringVector files;
        char **rc = PHYSFS_enumerateFiles(path.c_str());
        if (rc != 0)
        {
            for (char **i = rc; *i != 0; i++)
            {
                files.push_back(*i);
            }
            PHYSFS_freeList(rc);
        }

        return files;
    }

    FileHandle FileSystem::openFile(const String& filename, FileMode mode)
    {
        FileHandle h = 0;
        if (mode == FM_Read)
            h = (FileHandle)PHYSFS_openRead(filename.c_str());
        else if (mode == FM_Write)
            h = (FileHandle)PHYSFS_openWrite(filename.c_str());
        else if (mode == FM_Append)
            h = (FileHandle)PHYSFS_openAppend(filename.c_str());
        return h;
    }

    bool FileSystem::closeFile(FileHandle file)
    {
        return PHYSFS_close((PHYSFS_File*)file) ? true : false;
    }

    int FileSystem::readFile(FileHandle file, char* buffer, int size, int count)
    {
        return (int)PHYSFS_read((PHYSFS_File*)file, buffer, size, count);
    }

    int FileSystem::writeFile(FileHandle file, const char* buffer, int size, int count)
    {
        return (int)PHYSFS_write((PHYSFS_File*)file, buffer, size, count);
    }

    bool FileSystem::eof(FileHandle file)
    {
        return PHYSFS_eof((PHYSFS_File*)file) ? true : false;
    }

    int FileSystem::tell(FileHandle file)
    {
        return (int)PHYSFS_tell((PHYSFS_File*)file);
    }

    bool FileSystem::seek(FileHandle file, int pos)
    {
        return PHYSFS_seek((PHYSFS_File*)file, pos) ? true : false;
    }

    int FileSystem::size(FileHandle file)
    {
        return (int)PHYSFS_fileLength((PHYSFS_File*)file);
    }
}
