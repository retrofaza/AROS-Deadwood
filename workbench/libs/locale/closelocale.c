/*
    Copyright (C) 1995-1998 AROS - The Amiga Research OS
    $Id$

    Desc: CloseLocale() - Close a locale structure.
    Lang: english
*/
#include <exec/types.h>
#include <proto/exec.h>
#include "locale_intern.h"

extern struct Locale defLocale;

/*****************************************************************************

    NAME */
#include <proto/locale.h>

	AROS_LH1(void, CloseLocale,

/*  SYNOPSIS */
	AROS_LHA(struct Locale *, locale, A0),

/*  LOCATION */
	struct LocaleBase *, LocaleBase, 7, Locale)

/*  FUNCTION
	Finish accessing a Locale.

    INPUTS
	locale  -   An opened locale. Note that NULL is a valid
		    parameter here, and will simply be ignored.

    RESULT
	The locale is released back to the system.

    NOTES

    EXAMPLE

    BUGS

    SEE ALSO
	OpenLocale()

    INTERNALS

    HISTORY
	27-11-96    digulla automatically created from
			    locale_lib.fd and clib/locale_protos.h

*****************************************************************************/
{
    AROS_LIBFUNC_INIT
    AROS_LIBBASE_EXT_DECL(struct Library *,LocaleBase)

    /* Best make sure we actually have something freeable. */
    if(locale && (locale != &defLocale))
    {
	/* Make sure we don't have any race conditions */
	ObtainSemaphore(&IntLB(LocaleBase)->lb_LocaleLock);
	if(--IntL(locale)->il_Count == 0)
	{
	    /* Free the locale structure */
	    FreeMem(locale, sizeof(struct IntLocale));
	}
	ReleaseSemaphore(&IntLB(LocaleBase)->lb_LocaleLock);
    }

    AROS_LIBFUNC_EXIT
} /* CloseLocale */
