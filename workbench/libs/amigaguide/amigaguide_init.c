/*
    (C) 2000 AROS - The Amiga Research OS
    $Id$

    Desc: AmigaGuide initialization code.
    Lang: English.
*/

#define AROS_ALMOST_COMPATIBLE
#include <stddef.h>
#include <exec/types.h>
#include <exec/libraries.h>
#include <aros/libcall.h>
#include <aros/debug.h>

#include <proto/exec.h>

#include "amigaguide_intern.h"
#include "libdefs.h"


#undef SysBase

/* Customize libheader.c */
#define LC_LIBBASESIZE  sizeof(LIBBASETYPE)

#define LC_NO_OPENLIB
#define LC_NO_CLOSELIB

#include <libcore/libheader.c>



#define AmigaGuideBase ((LIBBASETYPEPTR) lh)


ULONG SAVEDS STDARGS LC_BUILDNAME(L_InitLib) (LC_LIBHEADERTYPEPTR lh)
{
    D(bug("Inside initfunc of amigaguide.library\n"));


    return TRUE;
}

/*
ULONG SAVEDS STDARGS LC_BUILDNAME(L_OpenLib) (LC_LIBHEADERTYPEPTR lh)
{
}

void SAVEDS STDARGS LC_BUILDNAME(L_CloseLib) (LC_LIBHEADERTYPEPTR lh)
{
}
*/

void SAVEDS STDARGS LC_BUILDNAME(L_ExpungeLib) (LC_LIBHEADERTYPEPTR lh)
{

}
