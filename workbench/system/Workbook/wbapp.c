/*
    Copyright © 2011, The AROS Development Team. All rights reserved.
    $Id$

    Desc: Workbook Application Class
    Lang: english
*/

#define DEBUG 0
#include <aros/debug.h>

#include <string.h>
#include <limits.h>

#include <proto/dos.h>
#include <proto/exec.h>
#include <proto/intuition.h>
#include <proto/utility.h>
#include <proto/gadtools.h>
#include <proto/workbench.h>
#include <proto/graphics.h>

#include <intuition/classusr.h>
#include <libraries/gadtools.h>
#include <workbench/handler.h>

#include "workbook_intern.h"
#include "classes.h"

struct wbApp {
    struct MsgPort *WinPort;
    ULONG           WinMask;   /* Mask of our port(s) */
    struct MsgPort *AppPort;
    ULONG           AppMask;   /* Mask of our port(s) */
    Object         *Root;      /* Background 'root' window */
    struct MinList  Windows; /* Subwindows */
};

static void wbOpenDrawer(Class *cl, Object *obj, CONST_STRPTR path)
{
    struct WorkbookBase *wb = (APTR)cl->cl_UserData;
    struct wbApp *my = INST_DATA(cl, obj);
    Object *win;

    win = NewObject(WBWindow, NULL, 
                        WBWA_UserPort, my->WinPort,
                        WBWA_Path, path,
                        TAG_END);

    if (win)
    	DoMethod(obj, OM_ADDMEMBER, win);
}

// OM_NEW
static IPTR WBAppNew(Class *cl, Object *obj, struct opSet *ops)
{
    struct WorkbookBase *wb = (APTR)cl->cl_UserData;
    struct wbApp *my;
    IPTR rc;

    rc = DoSuperMethodA(cl, obj, (Msg)ops);
    if (rc == 0)
    	return 0;

    my = INST_DATA(cl, rc);

    NEWLIST(&my->Windows);

    /* Create our Workbench message port */
    my->AppPort = CreatePort(NULL, 0);

    if (my->AppPort == NULL) {
    	DoSuperMethod(cl, (Object *)rc, OM_DISPOSE);
    	return 0;
    }

    /* Create our Window message port */
    my->WinPort = CreatePort(NULL, 0);

    if (my->WinPort == NULL) {
    	DeleteMsgPort(my->AppPort);
    	DoSuperMethod(cl, (Object *)rc, OM_DISPOSE);
    	return 0;
    }

    my->AppMask |= (1UL << my->AppPort->mp_SigBit);
    my->WinMask |= (1UL << my->WinPort->mp_SigBit);

    /* Create our root window */
    my->Root = NewObject(WBWindow, NULL,
                         WBWA_Path, NULL,
                         WBWA_UserPort, my->WinPort,
                         TAG_END);

    if (my->Root == NULL) {
    	DeleteMsgPort(my->WinPort);
    	DeleteMsgPort(my->AppPort);
    	DoSuperMethod(cl, (Object *)rc, OM_DISPOSE);
    	return 0;
    }

    return rc;
}

// OM_DISPOSE
static IPTR WBAppDispose(Class *cl, Object *obj, Msg msg)
{
    struct WorkbookBase *wb = (APTR)cl->cl_UserData;
    struct wbApp *my = INST_DATA(cl, obj);
    Object *tstate = (Object *)my->Windows.mlh_Head;
    Object *tmp;

    /* Get rid of the subwindows */
    while ((tmp = NextObject(&tstate))) {
    	DoMethod(tmp, OM_REMOVE);
    	DisposeObject(obj);
    }

    /* Get rid of the main window */
    DisposeObject(my->Root);

    DeleteMsgPort(my->AppPort);
    DeleteMsgPort(my->WinPort);

    return DoSuperMethodA(cl, obj, msg);
}

// OM_ADDMEMBER
static IPTR WBAppAddMember(Class *cl, Object *obj, struct opMember *opm)
{
    struct wbApp *my = INST_DATA(cl, obj);

    return DoMethod(opm->opam_Object, OM_ADDTAIL, &my->Windows);
}

// OM_REMMEMBER
static IPTR WBAppRemMember(Class *cl, Object *obj, struct opMember *opm)
{
    return DoMethod(opm->opam_Object, OM_REMOVE);
}


static Object *wbLookupWindow(Class *cl, Object *obj, struct Window *win)
{
    struct WorkbookBase *wb = (APTR)cl->cl_UserData;
    struct wbApp *my = INST_DATA(cl, obj);
    Object *ostate = (Object *)my->Windows.mlh_Head;
    Object *owin;

    while ((owin = NextObject(&ostate))) {
    	struct Window *match = NULL;

    	GetAttr(WBWA_Window, owin, (IPTR *)&match);
    	if (match == win)
    	    return owin;
    }

    return NULL;
}

static void wbNewSizeWindow(Class *cl, Object *obj, struct Window *win)
{
    Object *owin;

    if ((owin = wbLookupWindow(cl, obj, win))) {
    	DoMethod(owin, WBWM_NEWSIZE);
    }
}

static void wbCloseWindow(Class *cl, Object *obj, struct Window *win)
{
    struct WorkbookBase *wb = (APTR)cl->cl_UserData;
    Object *owin;

    if ((owin = wbLookupWindow(cl, obj, win))) {
    	DoMethod(obj, OM_REMMEMBER, owin);
    	DisposeObject(owin);
    }
}

static void NewCLI(struct WorkbookBase *wb)
{
    BPTR cis;

    cis = Open("CON:20/20///Workbook Shell/AUTO", MODE_OLDFILE);
    if (cis)
    {
        struct TagItem tags[] =
            {
                { SYS_Asynch,      TRUE       }, /* 0 */
                { SYS_Background,  FALSE      }, /* 1 */
                { SYS_Input,       (IPTR)cis  }, /* 2 */
                { SYS_Output,      (IPTR)NULL }, /* 3 */
                { SYS_Error,       (IPTR)NULL }, /* 4 */
		{ TAG_END } };

	SystemTags("", tags);
    }
}

static BOOL wbMenuPick(Class *cl, Object *obj, struct Window *win, UWORD menuNumber)
{
    struct WorkbookBase *wb = (APTR)cl->cl_UserData;
    Object *owin;
    struct MenuItem *item;
    BOOL quit = FALSE;

    owin = wbLookupWindow(cl, obj, win);

    D(bug("Menu: %x\n", menuNumber));
    while (menuNumber != MENUNULL) {
    	item = ItemAddress(win->MenuStrip, menuNumber);

    	if (MENUNUM(menuNumber) == 0) {
    	    D(bug("Menu Command: %c\n", item->Command));
    	    switch (item->Command) {
    	    case 'Q':
    	    	quit = TRUE;
    	    	break;
    	    case 'W':
    	    	NewCLI(wb);
    	    	break;
    	    default:
    	    	break;
    	    }
    	} else {
    	    D(bug("Menu passed to: %p\n", owin));
    	    if (owin)
    	    	DoMethod(owin, WBWM_MENUPICK, item, menuNumber);
    	}

    	menuNumber = item->NextSelect;
    }

    return quit;
}

// WBAM_WORKBENCH - Register and handle all workbench events
static IPTR WBAppWorkbench(Class *cl, Object *obj, Msg msg)
{
    struct WorkbookBase *wb = (APTR)cl->cl_UserData;
    struct wbApp *my = INST_DATA(cl, obj);
    BOOL done = FALSE;

    if (RegisterWorkbench(my->AppPort)) {
    	while (!done) {
    	    ULONG mask;

    	    mask = Wait(my->AppMask | my->WinMask);

    	    if (mask & my->AppMask) {
    	    	struct WBHandlerMessage *wbhm;
    	    	wbhm = (APTR)GetMsg(my->AppPort);

    	    	switch (wbhm->wbhm_Type) {
    	    	case WBHM_TYPE_SHOW:
    	    	    /* Show all windows */
    	    	    break;
    	    	case WBHM_TYPE_HIDE:
    	    	    /* Hide all windows */
    	    	    break;
    	    	case WBHM_TYPE_OPEN:
    	    	    /* Open a drawer */
    	    	    wbOpenDrawer(cl, obj, wbhm->wbhm_Data.Open.Name);
    	    	    break;
    	    	case WBHM_TYPE_UPDATE:
    	    	    /* Refresh an open window/object */
    	    	    break;
    	    	}
    	    	ReplyMsg((APTR)wbhm);
    	    }

    	    if (mask & my->WinMask) {
    	    	struct IntuiMessage *im;

    	    	im = GT_GetIMsg(my->WinPort);

    	    	D(bug("im=%p, Class=%d, Code=%d\n", im, im->Class, im->Code));
    	    	switch (im->Class) {
    	    	case IDCMP_CLOSEWINDOW:
    	    	    /* Dispose the window */
    	    	    wbCloseWindow(cl, obj, im->IDCMPWindow);
    	    	    break;
    	    	case IDCMP_NEWSIZE:
    	    	    /* call WBWM_NEWSIZE on the window */
    	    	    wbNewSizeWindow(cl, obj, im->IDCMPWindow);
    	    	    break;
    	    	case IDCMP_MENUPICK:
    	    	    done = wbMenuPick(cl, obj, im->IDCMPWindow, im->Code);
    	    	    break;
    	    	}

    	    	GT_ReplyIMsg(im);
    	    }
    	}

    	UnregisterWorkbench(my->AppPort);
    }

    return FALSE;
}

static IPTR dispatcher(Class *cl, Object *obj, Msg msg)
{
    IPTR rc = 0;

    switch (msg->MethodID) {
    case OM_NEW:       rc = WBAppNew(cl, obj, (APTR)msg); break;
    case OM_DISPOSE:   rc = WBAppDispose(cl, obj, (APTR)msg); break;
    case OM_ADDMEMBER: rc = WBAppAddMember(cl, obj, (APTR)msg); break;
    case OM_REMMEMBER: rc = WBAppRemMember(cl, obj, (APTR)msg); break;
    case WBAM_WORKBENCH:rc = WBAppWorkbench(cl, obj, (APTR)msg); break;
    default:           rc = DoSuperMethodA(cl, obj, msg); break;
    }

    return rc;
}

Class *WBApp_MakeClass(struct WorkbookBase *wb)
{
    Class *cl;

    cl = MakeClass( NULL, "rootclass", NULL,
                    sizeof(struct wbApp),
                    0);
    if (cl != NULL) {
    	cl->cl_Dispatcher.h_Entry = HookEntry;
    	cl->cl_Dispatcher.h_SubEntry = dispatcher;
    	cl->cl_Dispatcher.h_Data = NULL;
    	cl->cl_UserData = (IPTR)wb;
    }

    return cl;
}
