#include "__arosc_privdata.h"

#include <proto/exec.h>
#include <proto/dos.h>
#include <dos/dosextens.h>
#include <sys/arosc.h>
#include <aros/symbolsets.h>
#include <stdlib.h>

#include "arosc_init.h"

#include "__ctype.h"
#include "etask.h"

#include <aros/debug.h>

/* Provide local aroscbase for the static version of arosc */
#ifdef AROSC_STATIC
static struct aroscbase __aroscbase;
static struct aroscbase *aroscbase;
#endif

struct arosc_userdata *__get_arosc_userdata(void)
{
#ifdef AROSC_SHARED
    struct aroscbase *aroscbase = __get_aroscbase();
#endif
#ifdef AROSC_STATIC
    static int __init = 0;
    
    if(!__init)
    {
        __init = 1;
        
        aroscbase = &__aroscbase;
        __ctype_init();
    }
#endif
#if !defined(AROSC_SHARED) && !defined(AROSC_STATIC)
    extern struct aroscbase *aroscbase;
#endif
    
    return &(aroscbase->acb_acud);
}
