/*
    Copyright � 2021-2023, The AROS Development Team. All rights reserved.
    $Id$
*/


#include <proto/exec.h>
#include <proto/dos.h>
#include <proto/single.h>

#include <CUnit/CUnitCI.h>

void test_open(void)
{
    SingleBase = OpenLibrary((STRPTR)"single.library",0);

    if (SingleBase)
    {
        CU_PASS("");
    }
    else
    {
        CU_FAIL("NULL != OpenLibrary( \"single.library\", 0 ))");
    }
}

void test_base(void)
{
    CU_SKIP_IF(SingleBase == NULL);
    if(SingleBase != NULL)
    {
        CU_ASSERT(NT_LIBRARY == SingleBase->lib_Node.ln_Type);
        CU_ASSERT(0 != SingleBase->lib_NegSize);
        CU_ASSERT(0 != SingleBase->lib_PosSize);
        CU_ASSERT(0 != SingleBase->lib_OpenCnt);
    }
}

void test_reg_call(void)
{
    CU_SKIP_IF(SingleBase == NULL);

    /* Set initial value */
    RegSetValue(6);

    /* Check fixed number of argument functions */
    const LONG e1 = 16;
    LONG r1 = RegAdd4(1, 2, 3, 4);
    CU_ASSERT_EQUAL(r1, e1);

    const LONG e2 = 27;
    LONG r2 = RegAdd6(1, 2, 3, 4, 5, 6);
    CU_ASSERT_EQUAL(r2, e2);

    const LONG e3 = 51;
    LONG r3 = RegAdd9(1, 2, 3, 4, 5, 6, 7, 8, 9);
    CU_ASSERT_EQUAL(r3, e3);
}

void test_stack_call(void)
{
    CU_SKIP_IF(SingleBase == NULL);

    /* Set initial value */
    StackSetValue(5);

    /* Check fixed number of argument functions */
    const int e1 = 15;
    int r1 = StackAdd4(1, 2, 3, 4);
    CU_ASSERT_EQUAL(r1, e1);

    const int e2 = 26;
    int r2 = StackAdd6(1, 2, 3, 4, 5, 6);
    CU_ASSERT_EQUAL(r2, e2);

    const int e3 = 50;
    int r3 = StackAdd9(1, 2, 3, 4, 5, 6, 7, 8, 9);
    CU_ASSERT_EQUAL(r3, e3);

    /* Check variable number of argument functions */
    const int e4 = 15;
    int r4 = StackAdd4OrMore(4, 1, 2, 3, 4);
    CU_ASSERT_EQUAL(r4, e4);

    const int e5 = 26;
    int r5 = StackAdd4OrMore(6, 1, 2, 3, 4, 5, 6);
    CU_ASSERT_EQUAL(r5, e5);

    const int e6 = 50;
    int r6 = StackAdd4OrMore(9, 1, 2, 3, 4, 5, 6, 7, 8, 9);
    CU_ASSERT_EQUAL(r6, e6);
}

void test_close(void)
{
    CU_SKIP_IF(SingleBase == NULL);
    if(SingleBase != NULL)
    {
        CloseLibrary((struct Library *)SingleBase);
        CU_PASS("");
    }
}

int main(int argc, char** argv)
{
    CU_CI_DEFINE_SUITE("Library_Single_Suite", NULL, NULL, NULL, NULL);
    CUNIT_CI_TEST(test_open);
    CUNIT_CI_TEST(test_base);
    CUNIT_CI_TEST(test_reg_call);
    CUNIT_CI_TEST(test_stack_call);
    CUNIT_CI_TEST(test_close);
    return CU_CI_RUN_SUITES();
}

