/*
    Copyright (C) 2023, The AROS Development Team. All rights reserved.
*/

#include <aros/kernel.h>
#include <aros/libcall.h>

#include "kernel_base.h"
#include "kernel_intern.h"

AROS_LH0(unsigned int, KrnGetCPUNumber,
         struct KernelBase *, KernelBase, 41, Kernel)
{
    AROS_LIBFUNC_INIT

    int _mhartid;
    __asm__ volatile("csrr %0, mhartid" : "=r"(_mhartid));
    return _mhartid;

    AROS_LIBFUNC_EXIT
}
