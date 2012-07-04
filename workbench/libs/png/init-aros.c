#include <aros/symbolsets.h>
#include LC_LIBDEFS_FILE

#define DEBUG 1
#include <aros/debug.h>

IPTR aroscbase_offset;
IPTR zbase_offset;


static int InitFunc(LIBBASETYPEPTR LIBBASE)
{
    D(bug("Inside Init func of png.library\n"));

    aroscbase_offset = offsetof(LIBBASETYPE, _aroscbase);
    zbase_offset = offsetof(LIBBASETYPE, _zbase);

    return TRUE;
}

static int OpenFunc(LIBBASETYPEPTR LIBBASE)
{
    D(bug("Opening png.library\n"));

    LIBBASE->_aroscbase = OpenLibrary("arosc.library", 0);
    D(bug("[png.library::OpenLib] aroscbase=%p\n", LIBBASE->_aroscbase));

    LIBBASE->_zbase = OpenLibrary("z_au.library", 0);
    D(bug("[png.library::OpenLib] zbase=%p\n", LIBBASE->_zbase));

    return LIBBASE->_aroscbase != NULL && LIBBASE->_zbase != NULL;
}

static int CloseFunc(LIBBASETYPEPTR LIBBASE)
{
   D(bug("Closing png.library\n"));

   CloseLibrary(LIBBASE->_zbase);
   CloseLibrary(LIBBASE->_aroscbase);

   return TRUE;
}


ADD2INITLIB(InitFunc, 0);
ADD2OPENLIB(OpenFunc, 0);
ADD2CLOSELIB(CloseFunc, 0);
