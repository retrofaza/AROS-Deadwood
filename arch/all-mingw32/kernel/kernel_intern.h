#ifndef KERNEL_INTERN_H_
#define KERNEL_INTERN_H_

#include <aros/libcall.h>
#include <inttypes.h>
#include <exec/lists.h>
#include <exec/execbase.h>
#include <exec/memory.h>
#include <utility/tagitem.h>
#include <stdio.h>
#include <stdarg.h>

//#include "syscall.h"

#define STACK_SIZE 4096

#define EXCEPTIONS_NUM 2
#define INTERRUPTS_NUM 2

struct KernelBase {
    struct Node         kb_Node;
    void *              kb_MemPool;
    struct List         kb_Exceptions[EXCEPTIONS_NUM];
    struct List         kb_Interrupts[INTERRUPTS_NUM];
};

struct KernelBSS {
    void *addr;
    IPTR len;
};

enum intr_types {
    it_exception = 0xe0,
    it_interrupt = 0xf0
};

struct IntrNode {
    struct MinNode      in_Node;
    void                (*in_Handler)(void *, void *);
    void                *in_HandlerData;
    void                *in_HandlerData2;
    uint8_t             in_type;
    uint8_t             in_nr;
};

#ifdef bug
#undef bug
#endif
#ifdef D
#undef D
#endif
#define D(x) x

#ifdef __AROS__
struct KernelInterface {
    long (*core_init)(unsigned long TimerPeriod, struct ExecBase **SysBasePointer, APTR *KernelBasePointer);
    long (*core_intr_disable)(void);
    long (*core_intr_enable)(void);
    void (*core_syscall)(unsigned long n);
};

extern struct HostInterface *HostIFace;
extern struct KernelInterface KernelIFace;

IPTR krnGetTagData(Tag tagValue, intptr_t defaultVal, const struct TagItem *tagList);
struct TagItem *krnFindTagItem(Tag tagValue, const struct TagItem *tagList);
struct TagItem *krnNextTagItem(const struct TagItem **tagListPtr);

AROS_LD2(int, KrnBug,
         AROS_LDA(const char *, format, A0),
         AROS_LDA(va_list, args, A1),
         struct KernelBase *, KernelBase, 11, Kernel);

static inline void bug(const char *format, ...)
{
    va_list args;
    va_start(args, format);
    AROS_SLIB_ENTRY(KrnBug, Kernel)(format, args, NULL); /* Warning! It's a HACK (KernelBase == NULL)!!! */
    va_end(args);
}
#else
#define bug printf
extern unsigned char Ints_Enabled;
extern struct ExecBase **SysBasePtr;
extern struct KernelBase **KernelBasePtr;

void core_Dispatch(CONTEXT *regs);
void core_Switch(CONTEXT *regs);
void core_Schedule(CONTEXT *regs);
void core_ExitInterrupt(CONTEXT *regs);
void core_Cause(struct ExecBase *SysBase);
#endif

#endif /*KERNEL_INTERN_H_*/
