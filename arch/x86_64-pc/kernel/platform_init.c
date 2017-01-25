/*
    Copyright � 1995-2017, The AROS Development Team. All rights reserved.
    $Id$
*/

#define __KERNEL_NOLIBBASE__

#include <aros/multiboot.h>
#include <aros/symbolsets.h>
#include <asm/cpu.h>
#include <asm/io.h>
#include <exec/lists.h>
#include <proto/exec.h>

#include <inttypes.h>

#include "kernel_base.h"
#include "kernel_debug.h"
#include "kernel_intern.h"

#define D(x)
#define DAPIC(x)

/* 
    This file contains code that is run once Exec has been brought up - and is launched
    via the RomTag/Autoinit routines in Exec.
    
    Here we do the Platform configuration that requires a working "AROS" environment.
*/

static int Platform_Init(struct KernelBase *LIBBASE)
{
    struct PlatformData *pdata;
    int i;

    D(
        bug("[Kernel:x86_64] %s: Performing Post-Exec initialization\n", __func__);
        bug("[Kernel:x86_64] %s: KernelBase @ %p\n", __func__, LIBBASE);
    )

    NEWLIST(&LIBBASE->kb_ICList);
    LIBBASE->kb_ICTypeBase = KBL_INTERNAL + 1;

    D(bug("[Kernel:x86_64] %s: Interrupt Controller Base ID = %d\n", __func__, LIBBASE->kb_ICTypeBase));

    for (i = 0; i < HW_IRQ_COUNT; i++)
    {
        switch(i)
        {
            default:
                LIBBASE->kb_Interrupts[i].lh_Type = KBL_INTERNAL;
                break;
        }
    }

    D(bug("[Kernel:x86_64] %s: Interrupt Lists initialised\n", __func__));

    pdata = AllocMem(sizeof(struct PlatformData), MEMF_PUBLIC|MEMF_CLEAR);
    if (!pdata)
    	return FALSE;

    D(bug("[Kernel:x86_64] %s: Platform Data allocated @ 0x%p\n", __func__, pdata));

    LIBBASE->kb_PlatformData = pdata;

    return TRUE;
}

ADD2INITLIB(Platform_Init, 10)
