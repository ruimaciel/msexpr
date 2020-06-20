#include <stdlib.h>
#include <string.h>
#include <check.h>

#include <msexpr/serializers.h>

START_TEST(test_sexpr_GIVEN_cons_cell_with_text_atom_and_null_WHEN_fprint_csexpr_THEN_retval_is_zero)
{
    /* Given */
    struct  msexpr *cell = (struct msexpr *)msexpr_cons(msexpr_make_text("foo",3), NULL);

    /* When */
    fprintf(stdout,"\n");
    int retval = msexpr_fprint_csexpr(stdout, cell);
    fprintf(stdout,"\n");

    /* Then */
    ck_assert_int_eq(retval, 0);
}
END_TEST

START_TEST(test_sexpr_GIVEN_cons_cell_with_integer_atom_and_null_WHEN_fprint_csexpr_THEN_retval_is_zero)
{
    /* Given */
    struct  msexpr *cell = (struct msexpr *)msexpr_cons(msexpr_make_integer(42), NULL);

    /* When */
    fprintf(stdout,"\n");
    int retval = msexpr_fprint_csexpr(stdout, cell);
    fprintf(stdout,"\n");

    /* Then */
    ck_assert_int_eq(retval, 0);
}
END_TEST

START_TEST(test_sexpr_GIVEN_cons_cell_with_float_atom_and_null_WHEN_fprint_csexpr_THEN_retval_is_zero)
{
    /* Given */
    struct  msexpr *cell = (struct msexpr *)msexpr_cons(msexpr_make_float(42), NULL);

    /* When */
    fprintf(stdout,"\n");
    int retval = msexpr_fprint_csexpr(stdout, cell);
    fprintf(stdout,"\n");

    /* Then */
    ck_assert_int_eq(retval, 0);
}
END_TEST

Suite * msexpr_suite(void)
{
    Suite *s;
    TCase *tc_core;

    s = suite_create("msexpr");

    /* Core test case */
    tc_core = tcase_create("sexpr ops");

    tcase_add_test(tc_core, test_sexpr_GIVEN_cons_cell_with_text_atom_and_null_WHEN_fprint_csexpr_THEN_retval_is_zero);
    tcase_add_test(tc_core, test_sexpr_GIVEN_cons_cell_with_integer_atom_and_null_WHEN_fprint_csexpr_THEN_retval_is_zero);
    tcase_add_test(tc_core, test_sexpr_GIVEN_cons_cell_with_float_atom_and_null_WHEN_fprint_csexpr_THEN_retval_is_zero);

    suite_add_tcase(s, tc_core);

    return s;
}

int main(void)
{
    int number_failed = 0;
    Suite *s;
    SRunner *sr;

    s = msexpr_suite();
    sr = srunner_create(s);

    /*
     * Adds nofork to enable debugging tests
     * http://check.sourceforge.net/doc/check%5Fhtml/check%5F4.html#No-Fork-Mode
     */
    srunner_set_fork_status (sr, CK_NOFORK);

    srunner_run_all(sr, CK_NORMAL);
    number_failed = srunner_ntests_failed(sr);
    srunner_free(sr);
    return (number_failed == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}
