#include <stdlib.h>
#include <string.h>
#include <check.h>

#include <msexpr/sexpr.h>

START_TEST(test_sexpr_GIVEN_text_atom_WHEN_new_text_atom_THEN_output_valid_text_atom)
{
	/* Given */
	struct msexpr * atom = NULL;

	/* When */
	atom = msexpr_make_text("foo", 3);

	/* Then */
	ck_assert_int_eq(atom->type, MSEXPR_TYPE_ATOM_TEXT);
}
END_TEST

Suite * msexpr_suite(void)
{
	Suite *s;
	TCase *tc_core;

	s = suite_create("msexpr");

	/* Core test case */
	tc_core = tcase_create("atom text");

	tcase_add_test(tc_core, test_sexpr_GIVEN_text_atom_WHEN_new_text_atom_THEN_output_valid_text_atom);

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
