#define __PHYSICSFS_INTERNAL__
#include "physfs_internal.h"

int __PHYSFS_platformInit(void)
{
    return(1);  /* always succeed. */
} /* __PHYSFS_platformInit */


int __PHYSFS_platformDeinit(void)
{
    return(1);  /* always succeed. */
} /* __PHYSFS_platformDeinit */

void __PHYSFS_platformDetectAvailableCDs(PHYSFS_StringCallback cb, void *data)
{
} /* __PHYSFS_platformDetectAvailableCDs */

/* this is in posix.c ... */
extern char *__PHYSFS_platformCopyEnvironmentVariable(const char *varname);
