#include <stdlib.h>
#include <string.h>
#include <check.h>

#include <msexpr/sexpr.h>

START_TEST(test_sexpr_GIVEN_integer_atom_WHEN_new_integer_atom_THEN_output_valid_integer_atom)
{
    /* Given */
    struct msexpr * atom = NULL;

    /* When */
    atom = msexpr_make_integer(42);

    /* Then */
    ck_assert_int_eq(atom->type, MSEXPR_TYPE_ATOM_INTEGER);
}
END_TEST

START_TEST(test_sexpr_GIVEN_float_atom_WHEN_new_float_atom_THEN_output_valid_float_atom)
{
    /* Given */
    struct msexpr * atom = NULL;

    /* When */
    atom = msexpr_make_float(42.4);

    /* Then */
    ck_assert_int_eq(atom->type, MSEXPR_TYPE_ATOM_FLOAT);
}
END_TEST

//START_TEST(test_sexpr_GIVEN_WHEN_THEN_)
//{
//	/* Given */
//	/* When */
//	/* Then */
//}
//END_TEST

Suite * msexpr_suite(void)
{
    Suite *s;
    TCase *tc_core;

    s = suite_create("msexpr");

    /* Core test case */
    tc_core = tcase_create("atoms");

    tcase_add_test(tc_core, test_sexpr_GIVEN_integer_atom_WHEN_new_integer_atom_THEN_output_valid_integer_atom);
    tcase_add_test(tc_core, test_sexpr_GIVEN_float_atom_WHEN_new_float_atom_THEN_output_valid_float_atom);

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
