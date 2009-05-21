//***************************************************************
//  File:    FileSystem.h
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
#ifndef _FILESYSTEM_H_
#define _FILESYSTEM_H_

#include "CorePrerequisites.h"
#include "Singleton.h"

namespace hare
{
    typedef void* FileHandle;

    enum FileMode
    {
        FM_Read,
        FM_Write,
        FM_Append,
    };

    class CORE_API FileSystem : public Singleton<FileSystem>
    {
        HARE_DECLARE_SINGLETON(FileSystem)

    public:
        /**
        * Adds a directory to the search path.
        **/
        bool addSearchPath(const String& dir);

        /**
        * Removes a directory from the PhysFS search path.
        **/
        bool removeSearchPath(const String& dir);

        /**
        * Gets the user home directory.
        **/
        const char* getUserDir();

        /**
        * Gets the directory from which the application
        * was run.
        **/
        const char* getBaseDir();

        /**
        * Gets the last error
        **/
        const char* getLastError();

        /**
        * Gets the current write directory, if any. An empty
        * string will be returned if no directory is set.
        **/
        const char* getWriteDir();

        /**
        * Sets the current write directory.
        **/
        bool setWriteDir(const String& dir);

        const char* getRealDir(const String& dir);

        /**
        * Checks if some file exists in the current search path.
        * @param path The file (or directory) to check for.
        **/
        bool exists(const String& path);

        /**
        * Checks if an existing path is a directory.
        * @param file The filename to check.
        **/
        bool isDir(const String& path);

        /**
        * Checks if an existing path is a file,
        * @param file The filename to check.
        **/
        bool isFile(const String& path);

        /**
        * Creates a directory. Write dir must be set.
        * @param path The directory to create.
        **/
        bool mkDir(const String& path);

        /**
        * Removes a file (or directory).
        * @param path The file or directory to remove.
        **/
        bool remove(const String& path);

        StringVector enumFiles(const String& path);

        /**
        * Open a file in the search path.
        * @param filename The filename (and path) of the file.
        **/
        FileHandle openFile(const String& filename, FileMode mode);

        /**
        * Closes a file.
        * @param file The file to close.
        **/
        bool closeFile(FileHandle file);

        /**
        * Reads count bytes from an open file.
        * @param file The file handle.
        * @param count The number of bytes to read. Defaults
        * to the size of the file.
        **/
        int readFile(FileHandle file, char* buffer, int size, int count);

        /**
        * Write the bytes in data to the file. File
        * must be opened for write.
        * @param file The file handle to write to.
        **/
        int writeFile(FileHandle file, const char* buffer, int size, int count);

        /**
        * Check if end-of-file is reached.
        * @return True if EOF, false otherwise.
        **/
        bool eof(FileHandle file);

        /**
        * Gets the current position in a file.
        * @param file An open File.
        **/
        int tell(FileHandle file);

        /**
        * Seek to a position within a file.
        * @param pos The position to seek to.
        **/
        bool seek(FileHandle file, int pos);

        /**
        * Gets the size of a file.
        * @param file An open File's handle.
        **/
        int size(FileHandle file);

        void reset(const char* argv0);

    public:
        FileSystem(const char* argv0);

        ~FileSystem();
    };

    CORE_API FileSystem* getFileSystem();

    //class File
    //{
    //public:
    //    File();
    //    File(const String& filename, FileMode mode = FM_Read);

    //    FileMode getMode() const;

    //    readAll();

    //    write();

    //private:
    //    char* data;
    //};
}

#endif
