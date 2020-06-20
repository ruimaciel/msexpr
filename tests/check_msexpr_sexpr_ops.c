#include <stdlib.h>
#include <string.h>
#include <check.h>

#include <msexpr/sexpr.h>
#include <src/sexpr_ops.h>

START_TEST(test_sexpr_GIVEN_atom_text_WHEN_free_atom_text_THEN_atom_ptr_is_null)
{
	/* Given */
	struct  msexpr_atom_text *atom = (struct msexpr_atom_text *)msexpr_make_text(strdup("foo"), 3);

	/* When */
	msexpr_free_atom_text(&atom);

	/* Then */
	ck_assert_ptr_eq(atom, NULL);
}
END_TEST

START_TEST(test_sexpr_GIVEN_cell_with_null_car_and_cdr_WHEN_free_cell_THEN_cell_ptr_is_null)
{
	/* Given */
	struct  msexpr_cell *cell = (struct msexpr_cell *)msexpr_cons(NULL, NULL);

	/* When */
	msexpr_free_cell(&cell);

	/* Then */
	ck_assert_ptr_eq(cell, NULL);
}
END_TEST

Suite * msexpr_suite(void)
{
	Suite *s;
	TCase *tc_core;

	s = suite_create("msexpr");

	/* Core test case */
	tc_core = tcase_create("sexpr ops");

	tcase_add_test(tc_core, test_sexpr_GIVEN_atom_text_WHEN_free_atom_text_THEN_atom_ptr_is_null);
	tcase_add_test(tc_core, test_sexpr_GIVEN_cell_with_null_car_and_cdr_WHEN_free_cell_THEN_cell_ptr_is_null);

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
