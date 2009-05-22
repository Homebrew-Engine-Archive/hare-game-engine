%module hare

typedef unsigned char	uint8;
typedef signed char		int8;
typedef unsigned short	uint16;
typedef signed short	int16;
typedef unsigned int	uint32;
typedef signed int		int32;
typedef std::string     String;
typedef std::vector<String> StringVector;

%typemap(out) Object* { if ($1 && $owner) ($1)->addRef(); dynamicCastObject(L, $1, $owner); SWIG_arg++; }
%typemap(out) ScriptRunner* { if ($1 && $owner) ($1)->addRef(); dynamicCastObject(L, $1, $owner); SWIG_arg++; }

%newobject importObject;
Object* importObject(const String &url, bool share = true);

%newobject cloneObject;
Object* cloneObject(Object* object);

%typemap(out) Object*;

class Object
{
public:
    bool saveToXml(const String &path);
    bool saveToBin(const String &path);
};

class ScriptRunner : public Object
{
};

%{
    Object* importObject(const String &url, bool share = true)
    {
	    return Object::importObject(url, share);
    }

    Object* cloneObject(Object* object)
    {
	    return Object::cloneObject(object);
    }

    void dynamicCastObject(lua_State* L, Object* object, int owner)
    {
        if (object) {
      	    
       	    swig_type_info* info = 0;
	        const ClassInfo* cls = object->getClassInfo();
	        
	        do {
		        String name = String("_p_") + cls->className;
		        info = SWIG_TypeQuery(name.c_str());
		        if (!info) {
			        cls = cls->getBaseClass();
			        info = 0;
		        }
	        } while(!info && cls);

      	    SWIG_NewPointerObj(L, object, info, owner);
      	    
        } else { 
            SWIG_NewPointerObj(L, 0, SWIGTYPE_p_Object, owner);
        }
    }
%}

typedef void* FileHandle;

enum FileMode
{
    FM_Read,
    FM_Write,
    FM_Append,
};

class FileSystem
{
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

public:
    FileSystem(const char* argv0);
};

FileSystem* getFileSystem();

class Timer
{
public:
	float getDeltaTime();
	float getFPS();
};

Timer& getTimer();
float getTime();

