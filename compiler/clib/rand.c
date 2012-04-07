/*
    Copyright � 1995-2012, The AROS Development Team. All rights reserved.
    $Id$

    C99 functions rand() and srand().
*/

#include "__arosc_privdata.h"

#include <aros/symbolsets.h>

/*****************************************************************************

    NAME */
#include <stdlib.h>

	int rand (

/*  SYNOPSIS */
	void)

/*  FUNCTION
	A random number generator.

    INPUTS
	None.

    RESULT
	A pseudo-random integer between 0 and RAND_MAX.

    NOTES

    EXAMPLE

    BUGS

    SEE ALSO
        srand()

    INTERNALS

******************************************************************************/
{
    struct aroscbase *aroscbase = __GM_GetBase();

    aroscbase->acb_srand_seed = aroscbase->acb_srand_seed * 1103515245 + 12345;

    return aroscbase->acb_srand_seed % RAND_MAX;
} /* rand */


/*****************************************************************************

    NAME */
#include <stdlib.h>

	void srand (

/*  SYNOPSIS */
	unsigned int seed)

/*  FUNCTION
	Set the starting value for the random number generator rand()
        If a seed value is set to a value the same stream of pseudo-random
        numbers will be generated by rand() for the same seed value.

    INPUTS
	seed - New start value

    RESULT
	None.

    NOTES
        One seed value per arosc.library is kept which normally corresponds
        with per task.

    EXAMPLE

    BUGS

    SEE ALSO
        rand()

    INTERNALS

******************************************************************************/
{
    struct aroscbase *aroscbase = __GM_GetBase();

    aroscbase->acb_srand_seed = seed;
} /* srand */

static int __rand_seedinit(struct aroscbase *aroscbase)
{
    aroscbase->acb_srand_seed = 1;

    return 1;
}

ADD2OPENLIB(__rand_seedinit, 0);
