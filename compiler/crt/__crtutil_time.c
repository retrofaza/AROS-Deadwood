/*
    Copyright (C) 1995-2020, The AROS Development Team. All rights reserved.

    Return the current time in seconds.
*/

#include <proto/exec.h>

#define __NOBLIBBASE__

#include <proto/timer.h>

#include <string.h>

#include "__crtutil_intbase.h"

#include <aros/debug.h>

static int __init_timerbase(struct CrtUtilIntBase *CrtUtilBase);
#define TimerBase       CrtUtilBase->_TimerBase

/*****************************************************************************

    NAME */
#include <time.h>

        time_t time (

/*  SYNOPSIS */
        time_t * tloc)

/*  FUNCTION
       time() returns the time since 00:00:00 GMT, January 1, 1970,
       measured in seconds.

    INPUTS
        tloc - If this pointer is non-NULL, then the time is written into
                this variable as well.

    RESULT
        The number of seconds.

    NOTES

    EXAMPLE
        time_t tt1, tt2;

        // tt1 and tt2 are the same
        tt1 = time (&tt2);

        // This is valid, too
        tt1 = time (NULL);

    BUGS

    SEE ALSO
        ctime(), asctime(), localtime()

    INTERNALS

******************************************************************************/
{
    struct CrtUtilIntBase *CrtUtilBase = (struct CrtUtilIntBase *)__aros_getbase_CrtUtilBase();
    struct timeval tv;

    /* We get TimerBase here and not during LIBINIT because timer.device is not available
       when library is initialized.
    */
    if (TimerBase == NULL)
        __init_timerbase(CrtUtilBase);
    if (TimerBase == NULL)
        return (time_t)-1;

    GetSysTime(&tv);
    tv.tv_sec += 2922 * 1440 * 60;

    tv.tv_sec += __stdc_gmtoffset() * 60;

    if (tloc)
        *tloc = tv.tv_sec;
    return tv.tv_sec;
} /* time */


static int __init_timerbase(struct CrtUtilIntBase *CrtUtilBase)
{
    D(bug("%s()\n", __func__));

    memset( &CrtUtilBase->timeport, 0, sizeof( CrtUtilBase->timeport ) );
    CrtUtilBase->timeport.mp_Node.ln_Type   = NT_MSGPORT;
    CrtUtilBase->timeport.mp_Flags          = PA_IGNORE;
    CrtUtilBase->timeport.mp_SigTask        = FindTask(NULL);
    NEWLIST(&CrtUtilBase->timeport.mp_MsgList);

    CrtUtilBase->timereq.tr_node.io_Message.mn_Node.ln_Type    = NT_MESSAGE;
    CrtUtilBase->timereq.tr_node.io_Message.mn_Node.ln_Pri     = 0;
    CrtUtilBase->timereq.tr_node.io_Message.mn_Node.ln_Name    = NULL;
    CrtUtilBase->timereq.tr_node.io_Message.mn_ReplyPort       = &CrtUtilBase->timeport;
    CrtUtilBase->timereq.tr_node.io_Message.mn_Length          = sizeof (CrtUtilBase->timereq);

    if (OpenDevice(
            "timer.device", UNIT_VBLANK,
            (struct IORequest *)&CrtUtilBase->timereq, 0
        ) == 0
    )
    {
        TimerBase = (struct Device *)CrtUtilBase->timereq.tr_node.io_Device;
        D(bug("%s: TimerBase = 0x%p\n", __func__, TimerBase));
        return 1;
    }
    else
    {
        D(bug("%s: OpenDevice failed\n", __func__));
        return 0;
    }
}


static void __exit_timerbase(struct CrtUtilIntBase *CrtUtilBase)
{
    D(bug("%s()\n", __func__));

    if (TimerBase != NULL)
    {
        CloseDevice((struct IORequest *)&CrtUtilBase->timereq);
        TimerBase = NULL;
    }
}

ADD2EXPUNGELIB(__exit_timerbase, 0);
