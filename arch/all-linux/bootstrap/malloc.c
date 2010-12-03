/*
 * AROS-thread-safe versions of libc memory allocation routines
 * This does not work with Android's bionic.
 */

#ifndef __ANDROID__

#include <proto/exec.h>
#include <sys/types.h>

static int memnest;
#define MEMLOCK if (SysBase != NULL) Forbid();
#define MEMUNLOCK if (SysBase != NULL) Permit();

extern struct ExecBase *SysBase;
extern void * __libc_malloc(size_t);
extern void __libc_free(void *);
extern void * __libc_calloc(size_t, size_t);
extern void * __libc_realloc(void * mem, size_t newsize);

void * malloc(size_t size)
{
    void *res;

    MEMLOCK
    memnest++;
    res = __libc_malloc(size);
    memnest--;
    MEMUNLOCK

    return res;
}

void free(void * addr)
{
    MEMLOCK
    memnest++;
    __libc_free(addr);
    memnest--;
    MEMUNLOCK
}

void * calloc(size_t n, size_t size)
{
    void *res;

    MEMLOCK
    memnest++;
    res = __libc_calloc(n, size);
    memnest--;
    MEMUNLOCK
    
    return res;
}

#endif
