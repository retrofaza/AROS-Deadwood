/*
    Copyright (C) 1995-2021, The AROS Development Team. All rights reserved.
*/
/*
 * This program generates the libcall.h macroset for gcc
 */

#include <stdio.h>
#include <string.h>

#define GENCALL_MAX     (13 + 1)        /* Max number of arguments */

#define FLAG_NR         (1 << 2)

static inline const char *nr(int flags)
{
    if (flags & FLAG_NR)
        return "NR";
    else
        return "";
}

#if 0
void aros_lc(int id, const char *suffix)
{
    int i;

    printf("#define AROS_LC%d%s(t,n,", id, suffix);
    for (i = 0; i < id; i++)
        printf("a%d,", i + 1);
    printf("bt,bn,o,s) \\\n"
           "({ \\\n"
           "    bt __bn = (bt)bn; \\\n"
           "    ((__attribute__((regparm(1))) t (*)(bt"
    );
    for (i = 0; i < id; i++)
    {
        printf(", __AROS_LDA(a%d)", i+1);
    }
    printf("))__AROS_GETVECADDR(__bn,o))(__bn");
    for (i = 0; i < id; i++)
    {
        printf(", __AROS_LCA(a%d)", i+1);
    }
    printf("); \\\n"
           "})\n"
    );
}
#endif

static void aros_call(int id, int flags)
{
    int i;
    printf("#define __AROS_NEWCALL%d%s(t,n,", id, nr(flags));
    for (i = 0; i < id; i++)
        printf("a%d,", i + 1);
    printf("bt,bn) \\\n"
           "({ \\\n"
           "    t __ret; \\\n"
           "    register bt __bn asm(\"r12\"); \\\n"
           /* Need to first assign to variable otherwise gcc may miscompile */
           "    APTR __func = (APTR)n; \\\n"
           "    asm volatile(\"pushq %%%%r12\" : : :); \\\n"
           "    __bn = bn; \\\n"
           "    __ret = (( t (*)("
    );
    if (i == 0)
        printf("void");
    for (i = 0; i < id; i++)
    {
        if (i > 0)
            printf(", ");
        printf("__AROS_LDA(a%d)", i+1);
    }
    printf("))(APTR)__func)(");
    if (i == 0)
        printf("void");
    for (i = 0; i < id; i++)
    {
        if (i > 0)
            printf(", ");
        printf("__AROS_LCA(a%d)", i+1);
    }
    printf("); \\\n"
    "    asm volatile(\"popq %%%%r12\" : : :); \\\n"
    "    __ret; \\\n"
           "})\n"
    );
    printf("#define AROS_NEWCALL%d%s __AROS_NEWCALL%d%s\n", id, nr(flags), id, nr(flags));
}

static void aros_lvo_call(int id, int flags)
{
    int i;
    printf("#define __AROS_NEWLVO_CALL%d%s(t,", id, nr(flags));
    for (i = 0; i < id; i++)
        printf("a%d,", i + 1);
    printf("bt,bn,o,s) \\\n");
    printf("    __AROS_NEWCALL%d%s(t,__AROS_GETVECADDR(bn,o), \\\n", id, nr(flags));
    for (i = 0; i < id; i++)
        printf("        AROS_LCA(a%d), \\\n", i + 1);
    printf("        bt,bn)\n");
    printf("#define AROS_NEWLVO_CALL%d%s __AROS_NEWLVO_CALL%d%s\n", id, nr(flags), id, nr(flags));
}

static void aros_lh(int id, int is_ignored)
{
    int i;

    printf("#define __AROS_NEWLH%d%s(t,n,", id, is_ignored ? "I" : "");
    for (i = 0; i < id; i++)
        printf("a%d,", i + 1);
    printf("bt,bn,o,s) \\\n");
    printf("    t AROS_SLIB_ENTRY(n,s,o) (");
    if (i == 0)
        printf("void");
    for (i = 0; i < id; i++)
    {
        if (i > 0)
            printf(", ");
        printf("__AROS_LHA(a%d)", i + 1);
    }
    printf(") {");
    if (!is_ignored)
        printf(" \\\n    register bt __attribute__((unused)) bn asm(\"r12\");");
    printf("\n");
    printf("#define AROS_NEWLH%d%s __AROS_NEWLH%d%s\n", id, is_ignored ? "I" : "", id, is_ignored ? "I" : "");
}

#if 0
static void aros_ld(int id, int is_ignored)
{
    int i;

    printf("#define AROS_LD%d%s(t,n,", id, is_ignored ? "I" : "");
    for (i = 0; i < id; i++)
        printf("a%d,", i + 1);
    printf("bt,bn,o,s) \\\n");
    if (!is_ignored)
    {
        printf("    __attribute__((regparm(1))) t AROS_SLIB_ENTRY(n,s,o) (bt bn");
        for (i = 0; i < id; i++)
        {
            printf(", __AROS_LDA(a%d)", i + 1);
        }
    }
    else
    {
        printf("    t AROS_SLIB_ENTRY(n,s,o) (");
        if (i == 0)
            printf("void");
        for (i = 0; i < id; i++)
        {
            if (i > 0)
                printf(", ");
            printf("__AROS_LDA(a%d)", i + 1);
        }
    }
    printf(");\n");
}
#endif

#if 0
const static char extra[] =
"\n"
"#define __AROS_QUADt(type,name,reg1,reg2) type\n"
"#define __AROS_QUADn(type,name,reg1,reg2) name\n"
"#define __AROS_QUADr(type,name,reg1,reg2) reg1##reg2\n"
"\n"
"#define AROS_LHQUAD1(t,n,a1,bt,bn,o,s) \\\n"
"    AROS_LH1(t,n, \\\n"
"             AROS_LHA(__AROS_QUADt(a1), __AROS_QUADn(a1), __AROS_QUADr(a1)), \\\n"
"             bt, bn, o, s \\\n"
"    )\n"
"#define AROS_LHQUAD2(t,n,a1,a2,bt,bn,o,s) \\\n"
"    AROS_LH2(t,n, \\\n"
"             AROS_LHA(__AROS_QUADt(a1), __AROS_QUADn(a1), __AROS_QUADr(a1)), \\\n"
"             AROS_LHA(__AROS_QUADt(a2), __AROS_QUADn(a2), __AROS_QUADr(a2)), \\\n"
"             bt, bn, o, s \\\n"
"    )\n"
"#define AROS_LH1QUAD1(t,n,a1,a2,bt,bn,o,s) \\\n"
"    AROS_LH2(t,n, \\\n"
"             AROS_LHA(a1), \\\n"
"             AROS_LHA(__AROS_QUADt(a2), __AROS_QUADn(a2), __AROS_QUADr(a2)), \\\n"
"             bt, bn, o, s \\\n"
"    )\n"
"#define AROS_LH2QUAD1(t,n,a1,a2,a3,bt,bn,o,s) \\\n"
"    AROS_LH3(t,n, \\\n"
"             AROS_LHA(a1), \\\n"
"             AROS_LHA(a2), \\\n"
"             AROS_LHA(__AROS_QUADt(a3), __AROS_QUADn(a3), __AROS_QUADr(a3)), \\\n"
"             bt, bn, o, s \\\n"
"    )\n"
"#define AROS_LH3QUAD1(t,n,a1,a2,a3,a4,bt,bn,o,s) \\\n"
"    AROS_LH4(t,n, \\\n"
"             AROS_LHA(a1), \\\n"
"             AROS_LHA(a2), \\\n"
"             AROS_LHA(a3), \\\n"
"             AROS_LHA(__AROS_QUADt(a4), __AROS_QUADn(a4), __AROS_QUADr(a4)), \\\n"
"             bt, bn, o, s \\\n"
"    )\n"
"\n"
"\n"
"#define AROS_LCQUAD1(t,n,a1,bt,bn,o,s) \\\n"
"    AROS_LC1(t,n, \\\n"
"             AROS_LCA(__AROS_QUADt(a1), __AROS_QUADn(a1), __AROS_QUADr(a1)), \\\n"
"             bt, bn, o, s \\\n"
"    )\n"
"#define AROS_LCQUAD2(t,n,a1,a2,bt,bn,o,s) \\\n"
"    AROS_LC2(t,n, \\\n"
"             AROS_LCA(__AROS_QUADt(a1), __AROS_QUADn(a1), __AROS_QUADr(a1)), \\\n"
"             AROS_LCA(__AROS_QUADt(a2), __AROS_QUADn(a2), __AROS_QUADr(a2)), \\\n"
"             bt, bn, o, s \\\n"
"    )\n"
"#define AROS_LC1QUAD1(t,n,a1,a2,bt,bn,o,s) \\\n"
"    AROS_LC2(t,n, \\\n"
"             AROS_LCA(a1), \\\n"
"             AROS_LCA(__AROS_QUADt(a2), __AROS_QUADn(a2), __AROS_QUADr(a2)), \\\n"
"             bt, bn, o, s \\\n"
"    )\n"
"#define AROS_LC2QUAD1(t,n,a1,a2,a3,bt,bn,o,s) \\\n"
"    AROS_LC3(t,n, \\\n"
"             AROS_LCA(a1), \\\n"
"             AROS_LCA(a2), \\\n"
"             AROS_LCA(__AROS_QUADt(a3), __AROS_QUADn(a3), __AROS_QUADr(a3)), \\\n"
"             bt, bn, o, s \\\n"
"    )\n"
"#define AROS_LC3QUAD1(t,n,a1,a2,a3,a4,bt,bn,o,s) \\\n"
"    AROS_LC4(t,n, \\\n"
"             AROS_LCA(a1), \\\n"
"             AROS_LCA(a2), \\\n"
"             AROS_LCA(a3), \\\n"
"             AROS_LCA(__AROS_QUADt(a4), __AROS_QUADn(a4), __AROS_QUADr(a4)), \\\n"
"             bt, bn, o, s \\\n"
"    )\n"
"\n"
"#define AROS_LDQUAD1(t,n,a1,bt,bn,o,s) \\\n"
"     AROS_LD1(t,n, \\\n"
"         AROS_LDA(__AROS_QUADt(a1), __AROS_QUADn(a1), __AROS_QUADr(a1)), \\\n"
"         bt, bn, o, s \\\n"
"     )\n"
"#define AROS_LDQUAD2(t,n,a1,a2,bt,bn,o,s) \\\n"
"     AROS_LD2(t,n, \\\n"
"         AROS_LDA(__AROS_QUADt(a1), __AROS_QUADn(a1), __AROS_QUADr(a1)), \\\n"
"         AROS_LDA(__AROS_QUADt(a2), __AROS_QUADn(a2), __AROS_QUADr(a2)), \\\n"
"         bt, bn, o, s \\\n"
"     )\n"
"#define AROS_LD1QUAD1(t,n,a1,a2,bt,bn,o,s) \\\n"
"     AROS_LD2(t,n, \\\n"
"         AROS_LDA(a1), \\\n"
"         AROS_LDA(__AROS_QUADt(a2), __AROS_QUADn(a2), __AROS_QUADr(a2)), \\\n"
"         bt, bn, o, s \\\n"
"     )\n"
"#define AROS_LD2QUAD1(t,n,a1,a2,a3,bt,bn,o,s) \\\n"
"     AROS_LD3(t,n, \\\n"
"         AROS_LDA(a1), \\\n"
"         AROS_LDA(a2), \\\n"
"         AROS_LDA(__AROS_QUADt(a3), __AROS_QUADn(a3), __AROS_QUADr(a3)), \\\n"
"         bt, bn, o, s \\\n"
"     )\n"
"#define AROS_LD3QUAD1(t,n,a1,a2,a3,a4,bt,bn,o,s) \\\n"
"     AROS_LD4(t,n, \\\n"
"         AROS_LDA(a1), \\\n"
"         AROS_LDA(a2), \\\n"
"         AROS_LDA(a3), \\\n"
"         AROS_LDA(__AROS_QUADt(a4), __AROS_QUADn(a4), __AROS_QUADr(a4)), \\\n"
"         bt, bn, o, s \\\n"
"     )\n"
"\n";
#endif

int main(int argc, char **argv)
{
    // int i;

    printf("/* AUTOGENERATED by arch/x86_64-all/include/gencall.c */\n");
    printf("\n");
    printf("#ifndef AROS_X86_64_LIBCALL_H\n");
    printf("#define AROS_X86_64_LIBCALL_H\n");
    printf("\n");

    aros_lh(1, 0);

    aros_call(1, 0);

    aros_lvo_call(1, 0);

    // printf("#define __AROS_CPU_SPECIFIC_LH\n\n");

    // for (i = 0; i < GENCALL_MAX; i++)
    //     aros_lh(i, 0);

    // for (i = 0; i < GENCALL_MAX; i++)
    //     aros_lh(i, 1);

    // printf("\n");
    // printf("#define __AROS_CPU_SPECIFIC_LC\n\n");

    // for (i = 0; i < GENCALL_MAX; i++)
    //     aros_lc(i, "");

    // for (i = 0; i < GENCALL_MAX; i++)
    //     aros_lc(i, "NR");

    // for (i = 0; i < GENCALL_MAX; i++)
    //     aros_call(i, "");

    // for (i = 0; i < GENCALL_MAX; i++)
    //     aros_call(i, "NR");

    // printf("\n");
    // printf("#define __AROS_CPU_SPECIFIC_LD\n\n");

    // for (i = 0; i < GENCALL_MAX; i++)
    //     aros_ld(i, 0);

    // for (i = 0; i < GENCALL_MAX; i++)
    //     aros_ld(i, 1);

    // printf("%s\n", extra);

    printf("#endif /* AROS_X86_64_LIBCALL_H */\n");
    return 0;
}
