#include <stdlib.h>
#include <string.h>
#include <check.h>

#include <msexpr/sexpr.h>

START_TEST(test_sexpr_init_sexpr)
{
	/* Given */
	struct msexpr sexpr;

	/* When */
	msexpr_init(&sexpr);

	/* Then */
	ck_assert_int_eq(sexpr.type, MSEXPR_TYPE_NULL);
}
END_TEST

START_TEST(test_sexpr_GIVEN_sexpr_and_text_atom_with_same_type_WHEN_comparing_text_atom_with_sexpr_THEN_values_are_equal)
{
	/* let's check for pointer alignment issues */

	/* Given */
	struct msexpr sexpr;
	struct msexpr_atom_text atom;

	/* When */
	sexpr.type = MSEXPR_TYPE_ATOM_TEXT;
	atom.sexpr.type = MSEXPR_TYPE_ATOM_TEXT;

	/* Then */
	ck_assert_int_eq(atom.sexpr.type, sexpr.type);
}
END_TEST

START_TEST(test_sexpr_GIVEN_null_car_cdr_WHEN_cons_THEN_return_cell)
{
	/* Given */
	struct msexpr * cell = NULL;

	/* When */
	cell = msexpr_cons(NULL, NULL);

	/* Then */
	ck_assert_int_eq(cell->type, MSEXPR_TYPE_CELL);
}
END_TEST

START_TEST(test_sexpr_GIVEN_pointer_to_input_atom_WHEN_cons_input_atom_as_car_THEN_car_cell_is_input_car_cell)
{
	/* Given */
	struct msexpr * input_atom = msexpr_make_text("foo",3);
	struct msexpr * sexpr = NULL;

	/* When */
	sexpr = msexpr_cons(input_atom, NULL);

	/* Then */
	ck_assert_int_eq(sexpr->type, MSEXPR_TYPE_CELL);
	ck_assert_ptr_eq(((struct msexpr_cell *)sexpr)->car, input_atom);
}
END_TEST

START_TEST(test_sexpr_GIVEN_pointer_to_input_atom_WHEN_cons_input_atom_as_cdr_THEN_cdr_cell_is_input_cdr_cell)
{
	/* Given */
	struct msexpr *input_atom = msexpr_make_text("foo", 3);
	struct msexpr * sexpr = NULL;

	/* When */
	sexpr = msexpr_cons(NULL, input_atom);

	/* Then */
	ck_assert_int_eq(sexpr->type, MSEXPR_TYPE_CELL);
	ck_assert_ptr_eq(((struct msexpr_cell *)sexpr)->cdr, input_atom);
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
	tc_core = tcase_create("sexpr");

	tcase_add_test(tc_core, test_sexpr_init_sexpr);
	tcase_add_test(tc_core, test_sexpr_GIVEN_sexpr_and_text_atom_with_same_type_WHEN_comparing_text_atom_with_sexpr_THEN_values_are_equal);
	tcase_add_test(tc_core, test_sexpr_GIVEN_null_car_cdr_WHEN_cons_THEN_return_cell);
	tcase_add_test(tc_core, test_sexpr_GIVEN_pointer_to_input_atom_WHEN_cons_input_atom_as_car_THEN_car_cell_is_input_car_cell);
	tcase_add_test(tc_core, test_sexpr_GIVEN_pointer_to_input_atom_WHEN_cons_input_atom_as_cdr_THEN_cdr_cell_is_input_cdr_cell);

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
