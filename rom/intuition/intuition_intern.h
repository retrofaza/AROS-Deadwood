#ifndef INTUITION_INTERN_H
#define INTUITION_INTERN_H
/*
    (C) 1995-96 AROS - The Amiga Replacement OS
    $Id$

    Desc: Intuitions internal structure
    Lang: english
*/
#ifndef AROS_LIBCALL_H
#   include <aros/libcall.h>
#endif
#ifndef EXEC_EXECBASE_H
#   include <exec/execbase.h>
#endif
#ifndef EXEC_SEMAPHORES_H
#   include <exec/semaphores.h>
#endif
#ifndef GRAPHICS_GFXBASE_H
#   include <graphics/gfxbase.h>
#endif
#ifndef INTUITION_INTUITIONBASE_H
#   include <intuition/intuitionbase.h>
#endif
#include "intuition_debug.h"

struct IntIntuitionBase
{
    struct IntuitionBase IBase;

    /* Put local shit here, invisible for the user */
    struct GfxBase	   * GfxBase;
    struct ExecBase	   * SysBase;
    struct UtilityBase	   * UtilBase;
    struct MinList	     ClassList;
    struct Screen	   * WorkBench;
    struct SignalSemaphore * SigSem;

    APTR		     DriverData; /* Pointer which the driver may use */

/*    struct MinList	   PublicScreenList;
    struct Screen      * DefaultPublicScreen; */
};

struct IntScreen
{
    struct Screen Screen;

    /* Private fields */
    struct DrawInfo DInfo;
    UWORD  Pens[NUMDRIPENS];
};

extern struct IntuitionBase * IntuitionBase;

#define GetPubIBase(ib)   ((struct IntuitionBase *)ib)
#define GetPrivIBase(ib)  ((struct IntIntuitionBase *)ib)

#ifdef GfxBase
#undef GfxBase
#endif
#define GfxBase     (GetPrivIBase(IntuitionBase)->GfxBase)
#ifdef SysBase
#undef SysBase
#endif
#define SysBase     (GetPrivIBase(IntuitionBase)->SysBase)
#ifdef UtilityBase
#undef UtilityBase
#endif
#define UtilityBase (GetPrivIBase(IntuitionBase)->UtilBase)

#define PublicClassList ((struct List *)&(GetPrivIBase(IntuitionBase)->ClassList))

/* Window-Flags */
#define EWFLG_DELAYCLOSE	0x00000001L /* Delay CloseWindow() */
#define EWFLG_CLOSEWINDOW	0x00000002L /* Call CloseWindow() */

/* Needed for close() */
#define expunge() \
    AROS_LC0(BPTR, expunge, struct IntuitionBase *, IntuitionBase, 3, Intuition)

#endif /* INTUITION_INTERN_H */
