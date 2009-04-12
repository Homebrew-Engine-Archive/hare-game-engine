/*
 * PSP support routines for PhysicsFS.
 * Code is from posix.c and unix.c with some modification
 */
#define __PHYSICSFS_INTERNAL__

#ifndef S_ISLNK
#	ifdef S_IFLNK
#		define S_ISLNK(m) (((m) & S_IFMT) == S_IFLNK)
#	else
#		define S_ISLNK(m) 0
#		define lstat stat
# endif
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <ctype.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <pwd.h>
#include <dirent.h>
#include <errno.h>
#include <fcntl.h>

#ifdef PHYSFS_HAVE_LLSEEK
#include <linux/unistd.h>
#endif

#include "physfs_internal.h"


const char *__PHYSFS_platformDirSeparator = "/";


char *__PHYSFS_platformCopyEnvironmentVariable(const char *varname)
{
    const char *envr = getenv(varname);
    char *retval = NULL;

    if (envr != NULL)
    {
        retval = (char *) allocator.Malloc(strlen(envr) + 1);
        if (retval != NULL)
            strcpy(retval, envr);
    } /* if */

    return(retval);
} /* __PHYSFS_platformCopyEnvironmentVariable */


static char *getUserNameByUID(void)
{
	  /* For PSP just return 0 */
    return 0;
} /* getUserNameByUID */


static char *getUserDirByUID(void)
{
	  /* For PSP just return 0 */
    return 0;
} /* getUserDirByUID */


char *__PHYSFS_platformGetUserName(void)
{
    char *retval = getUserNameByUID();
    if (retval == NULL)
        retval = __PHYSFS_platformCopyEnvironmentVariable("USER");
    return(retval);
} /* __PHYSFS_platformGetUserName */


char *__PHYSFS_platformGetUserDir(void)
{
    char *retval = __PHYSFS_platformCopyEnvironmentVariable("HOME");
    if (retval == NULL)
        retval = getUserDirByUID();
    return(retval);
} /* __PHYSFS_platformGetUserDir */


int __PHYSFS_platformExists(const char *fname)
{
    struct stat statbuf;
    BAIL_IF_MACRO(lstat(fname, &statbuf) == -1, strerror(errno), 0);
    return(1);
} /* __PHYSFS_platformExists */


int __PHYSFS_platformIsSymLink(const char *fname)
{
    struct stat statbuf;
    BAIL_IF_MACRO(lstat(fname, &statbuf) == -1, strerror(errno), 0);
    return( (S_ISLNK(statbuf.st_mode)) ? 1 : 0 );
} /* __PHYSFS_platformIsSymlink */


int __PHYSFS_platformIsDirectory(const char *fname)
{
    struct stat statbuf;
    BAIL_IF_MACRO(stat(fname, &statbuf) == -1, strerror(errno), 0);
    return( (S_ISDIR(statbuf.st_mode)) ? 1 : 0 );
} /* __PHYSFS_platformIsDirectory */


char *__PHYSFS_platformCvtToDependent(const char *prepend,
                                      const char *dirName,
                                      const char *append)
{
    int len = ((prepend) ? strlen(prepend) : 0) +
              ((append) ? strlen(append) : 0) +
              strlen(dirName) + 1;
    char *retval = (char *) allocator.Malloc(len);

    BAIL_IF_MACRO(retval == NULL, ERR_OUT_OF_MEMORY, NULL);

    /* platform-independent notation is Unix-style already.  :)  */

    if (prepend)
        strcpy(retval, prepend);
    else
        retval[0] = '\0';

    strcat(retval, dirName);

    if (append)
        strcat(retval, append);

    return(retval);
} /* __PHYSFS_platformCvtToDependent */



void __PHYSFS_platformEnumerateFiles(const char *dirname,
                                     int omitSymLinks,
                                     PHYSFS_EnumFilesCallback callback,
                                     const char *origdir,
                                     void *callbackdata)
{
    DIR *dir;
    struct dirent *ent;
    int bufsize = 0;
    char *buf = NULL;
    int dlen = 0;

    if (omitSymLinks)  /* !!! FIXME: this malloc sucks. */
    {
        dlen = strlen(dirname);
        bufsize = dlen + 256;
        buf = (char *) allocator.Malloc(bufsize);
        if (buf == NULL)
            return;
        strcpy(buf, dirname);
        if (buf[dlen - 1] != '/')
        {
            buf[dlen++] = '/';
            buf[dlen] = '\0';
        } /* if */
    } /* if */

    errno = 0;
    dir = opendir(dirname);
    if (dir == NULL)
    {
        allocator.Free(buf);
        return;
    } /* if */

    while ((ent = readdir(dir)) != NULL)
    {
        if (strcmp(ent->d_name, ".") == 0)
            continue;

        if (strcmp(ent->d_name, "..") == 0)
            continue;

        if (omitSymLinks)
        {
            char *p;
            int len = strlen(ent->d_name) + dlen + 1;
            if (len > bufsize)
            {
                p = (char *) allocator.Realloc(buf, len);
                if (p == NULL)
                    continue;
                buf = p;
                bufsize = len;
            } /* if */

            strcpy(buf + dlen, ent->d_name);
            if (__PHYSFS_platformIsSymLink(buf))
                continue;
        } /* if */

        callback(callbackdata, origdir, ent->d_name);
    } /* while */

    allocator.Free(buf);
    closedir(dir);
} /* __PHYSFS_platformEnumerateFiles */


int __PHYSFS_platformMkDir(const char *path)
{
    int rc;
    errno = 0;
    rc = mkdir(path, S_IRWXU);
    BAIL_IF_MACRO(rc == -1, strerror(errno), 0);
    return(1);
} /* __PHYSFS_platformMkDir */


static void *doOpen(const char *filename, int mode)
{
    const int appending = (mode & O_APPEND);
    int fd;
    int *retval;
    errno = 0;

    /* O_APPEND doesn't actually behave as we'd like. */
    mode &= ~O_APPEND;

    fd = open(filename, mode, S_IRUSR | S_IWUSR);
    BAIL_IF_MACRO(fd < 0, strerror(errno), NULL);

    if (appending)
    {
        if (lseek(fd, 0, SEEK_END) < 0)
        {
            close(fd);
            BAIL_MACRO(strerror(errno), NULL);
        } /* if */
    } /* if */

    retval = (int *) allocator.Malloc(sizeof (int));
    if (retval == NULL)
    {
        close(fd);
        BAIL_MACRO(ERR_OUT_OF_MEMORY, NULL);
    } /* if */

    *retval = fd;
    return((void *) retval);
} /* doOpen */


void *__PHYSFS_platformOpenRead(const char *filename)
{
    return(doOpen(filename, O_RDONLY));
} /* __PHYSFS_platformOpenRead */


void *__PHYSFS_platformOpenWrite(const char *filename)
{
    return(doOpen(filename, O_WRONLY | O_CREAT | O_TRUNC));
} /* __PHYSFS_platformOpenWrite */


void *__PHYSFS_platformOpenAppend(const char *filename)
{
    return(doOpen(filename, O_WRONLY | O_CREAT | O_APPEND));
} /* __PHYSFS_platformOpenAppend */


PHYSFS_sint64 __PHYSFS_platformRead(void *opaque, void *buffer,
                                    PHYSFS_uint32 size, PHYSFS_uint32 count)
{
    int fd = *((int *) opaque);
    int max = size * count;
    int rc = read(fd, buffer, max);

    BAIL_IF_MACRO(rc == -1, strerror(errno), rc);
    assert(rc <= max);

    if ((rc < max) && (size > 1))
        lseek(fd, -(rc % size), SEEK_CUR); /* rollback to object boundary. */

    return(rc / size);
} /* __PHYSFS_platformRead */


PHYSFS_sint64 __PHYSFS_platformWrite(void *opaque, const void *buffer,
                                     PHYSFS_uint32 size, PHYSFS_uint32 count)
{
    int fd = *((int *) opaque);
    int max = size * count;
    int rc = write(fd, (void *) buffer, max);

    BAIL_IF_MACRO(rc == -1, strerror(errno), rc);
    assert(rc <= max);

    if ((rc < max) && (size > 1))
        lseek(fd, -(rc % size), SEEK_CUR); /* rollback to object boundary. */

    return(rc / size);
} /* __PHYSFS_platformWrite */


int __PHYSFS_platformSeek(void *opaque, PHYSFS_uint64 pos)
{
    int fd = *((int *) opaque);

    #ifdef PHYSFS_HAVE_LLSEEK
      unsigned long offset_high = ((pos >> 32) & 0xFFFFFFFF);
      unsigned long offset_low = (pos & 0xFFFFFFFF);
      loff_t retoffset;
      int rc = llseek(fd, offset_high, offset_low, &retoffset, SEEK_SET);
      BAIL_IF_MACRO(rc == -1, strerror(errno), 0);
    #else
      BAIL_IF_MACRO(lseek(fd, (int) pos, SEEK_SET) == -1, strerror(errno), 0);
    #endif

    return(1);
} /* __PHYSFS_platformSeek */


PHYSFS_sint64 __PHYSFS_platformTell(void *opaque)
{
    int fd = *((int *) opaque);
    PHYSFS_sint64 retval;

    #ifdef PHYSFS_HAVE_LLSEEK
      loff_t retoffset;
      int rc = llseek(fd, 0, &retoffset, SEEK_CUR);
      BAIL_IF_MACRO(rc == -1, strerror(errno), -1);
      retval = (PHYSFS_sint64) retoffset;
    #else
      retval = (PHYSFS_sint64) lseek(fd, 0, SEEK_CUR);
      BAIL_IF_MACRO(retval == -1, strerror(errno), -1);
    #endif

    return(retval);
} /* __PHYSFS_platformTell */


PHYSFS_sint64 __PHYSFS_platformFileLength(void *opaque)
{
    int fd = *((int *) opaque);
    struct stat statbuf;
    BAIL_IF_MACRO(fstat(fd, &statbuf) == -1, strerror(errno), -1);
    return((PHYSFS_sint64) statbuf.st_size);
} /* __PHYSFS_platformFileLength */


int __PHYSFS_platformEOF(void *opaque)
{
    PHYSFS_sint64 pos = __PHYSFS_platformTell(opaque);
    PHYSFS_sint64 len = __PHYSFS_platformFileLength(opaque);
    return(pos >= len);
} /* __PHYSFS_platformEOF */


int __PHYSFS_platformFlush(void *opaque)
{
    int fd = *((int *) opaque);
    BAIL_IF_MACRO(fsync(fd) == -1, strerror(errno), 0);
    return(1);
} /* __PHYSFS_platformFlush */


int __PHYSFS_platformClose(void *opaque)
{
    int fd = *((int *) opaque);
    BAIL_IF_MACRO(close(fd) == -1, strerror(errno), 0);
    allocator.Free(opaque);
    return(1);
} /* __PHYSFS_platformClose */


int __PHYSFS_platformDelete(const char *path)
{
    BAIL_IF_MACRO(remove(path) == -1, strerror(errno), 0);
    return(1);
} /* __PHYSFS_platformDelete */


PHYSFS_sint64 __PHYSFS_platformGetLastModTime(const char *fname)
{
    struct stat statbuf;
    BAIL_IF_MACRO(stat(fname, &statbuf) < 0, strerror(errno), -1);
    return statbuf.st_mtime;
} /* __PHYSFS_platformGetLastModTime */

#endif  /* PHYSFS_PLATFORM_POSIX */


int __PHYSFS_platformInit(void)
{
    return(1);  /* always succeed. */
} /* __PHYSFS_platformInit */


int __PHYSFS_platformDeinit(void)
{
    return(1);  /* always succeed. */
} /* __PHYSFS_platformDeinit */


/* Stub version for platforms without CD-ROM support. */
void __PHYSFS_platformDetectAvailableCDs(PHYSFS_StringCallback cb, void *data)
{
} /* __PHYSFS_platformDetectAvailableCDs */

/* this is in posix.c ... */
extern char *__PHYSFS_platformCopyEnvironmentVariable(const char *varname);


/*
 * See where program (bin) resides in the $PATH specified by (envr).
 *  returns a copy of the first element in envr that contains it, or NULL
 *  if it doesn't exist or there were other problems. PHYSFS_SetError() is
 *  called if we have a problem.
 *
 * (envr) will be scribbled over, and you are expected to allocator.Free() the
 *  return value when you're done with it.
 */
static char *findBinaryInPath(const char *bin, char *envr)
{
    size_t alloc_size = 0;
    char *exe = NULL;
    char *start = envr;
    char *ptr;

    BAIL_IF_MACRO(bin == NULL, ERR_INVALID_ARGUMENT, NULL);
    BAIL_IF_MACRO(envr == NULL, ERR_INVALID_ARGUMENT, NULL);

    do
    {
        size_t size;
        ptr = strchr(start, ':');  /* find next $PATH separator. */
        if (ptr)
            *ptr = '\0';

        size = strlen(start) + strlen(bin) + 2;
        if (size > alloc_size)
        {
            char *x = (char *) allocator.Realloc(exe, size);
            if (x == NULL)
            {
                if (exe != NULL)
                    allocator.Free(exe);
                BAIL_MACRO(ERR_OUT_OF_MEMORY, NULL);
            } /* if */

            alloc_size = size;
            exe = x;
        } /* if */

        /* build full binary path... */
        strcpy(exe, start);
        if ((exe[0] == '\0') || (exe[strlen(exe) - 1] != '/'))
            strcat(exe, "/");
        strcat(exe, bin);

        if (access(exe, X_OK) == 0)  /* Exists as executable? We're done. */
        {
            strcpy(exe, start);  /* i'm lazy. piss off. */
            return(exe);
        } /* if */

        start = ptr + 1;  /* start points to beginning of next element. */
    } while (ptr != NULL);

    if (exe != NULL)
        allocator.Free(exe);

    return(NULL);  /* doesn't exist in path. */
} /* findBinaryInPath */


char *__PHYSFS_platformCalcBaseDir(const char *argv0)
{
    char *retval = NULL;
    char *envr = NULL;
    struct stat stbuf;

    /* fast path: default behaviour can handle this. */
    if ( (argv0 != NULL) && (strchr(argv0, '/') != NULL) )
        return(NULL);  /* higher level will parse out real path from argv0. */

    if ((retval == NULL) && (argv0 != NULL))
    {
        /* If there's no dirsep on argv0, then look through $PATH for it. */
        envr = __PHYSFS_platformCopyEnvironmentVariable("PATH");
        BAIL_IF_MACRO(!envr, NULL, NULL);
        retval = findBinaryInPath(argv0, envr);
        allocator.Free(envr);
    } /* if */

    return(retval);
} /* __PHYSFS_platformCalcBaseDir */


char *__PHYSFS_platformRealPath(const char *path)
{
    char resolved_path[MAXPATHLEN];
    char *retval = NULL;

    errno = 0;
    BAIL_IF_MACRO(!realpath(path, resolved_path), strerror(errno), NULL);
    retval = (char *) allocator.Malloc(strlen(resolved_path) + 1);
    BAIL_IF_MACRO(retval == NULL, ERR_OUT_OF_MEMORY, NULL);
    strcpy(retval, resolved_path);

    return(retval);
} /* __PHYSFS_platformRealPath */


char *__PHYSFS_platformCurrentDir(void)
{
    /*
     * This can't just do platformRealPath("."), since that would eventually
     *  just end up calling back into here.
     */

    int allocSize = 0;
    char *retval = NULL;
    char *ptr;

    do
    {
        allocSize += 100;
        ptr = (char *) allocator.Realloc(retval, allocSize);
        if (ptr == NULL)
        {
            if (retval != NULL)
                allocator.Free(retval);
            BAIL_MACRO(ERR_OUT_OF_MEMORY, NULL);
        } /* if */

        retval = ptr;
        ptr = getcwd(retval, allocSize);
    } while (ptr == NULL && errno == ERANGE);

    if (ptr == NULL && errno)
    {
            /*
             * getcwd() failed for some reason, for example current
             * directory not existing.
             */
        if (retval != NULL)
            allocator.Free(retval);
        BAIL_MACRO(ERR_NO_SUCH_FILE, NULL);
    } /* if */

    return(retval);
} /* __PHYSFS_platformCurrentDir */


int __PHYSFS_platformSetDefaultAllocator(PHYSFS_Allocator *a)
{
    return(0);  /* just use malloc() and friends. */
} /* __PHYSFS_platformSetDefaultAllocator */


// No pthread support for PSP here
PHYSFS_uint64 __PHYSFS_platformGetThreadID(void) { return(0x0001); }
void *__PHYSFS_platformCreateMutex(void) { return((void *) 0x0001); }
void __PHYSFS_platformDestroyMutex(void *mutex) {}
int __PHYSFS_platformGrabMutex(void *mutex) { return(1); }
void __PHYSFS_platformReleaseMutex(void *mutex) {}

/* end of psp.c ... */