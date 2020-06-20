#include <stdlib.h>
#include <string.h>
#include <check.h>

#include <src/sexpr_stack.h>

START_TEST(test_sexpr_GIVEN_uninitialized_stack_WHEN_init_THEN_stack_current_size_is_zero)
{
	/* Given */
	struct msexpr_sexpr_stack stack;

	/* When */
	msexpr_sexpr_stack_init(&stack, 4);

	/* Then */
	ck_assert_ptr_eq(msexpr_sexpr_stack_top(&stack), NULL);
}
END_TEST

START_TEST(test_sexpr_GIVEN_initialized_stack_WHEN_top_THEN_return_null_ptr)
{
	/* Given */
	struct msexpr_sexpr_stack stack;

	/* When */
	msexpr_sexpr_stack_init(&stack, 4);

	/* Then */
	ck_assert_ptr_eq(msexpr_sexpr_stack_top(&stack), NULL);
}
END_TEST

START_TEST(test_sexpr_GIVEN_initialized_stack_WHEN_push_element_THEN_top_returns_same_element)
{
	/* Given */
	struct msexpr_sexpr_stack stack;
	msexpr_sexpr_stack_init(&stack, 4);

	struct msexpr * text = msexpr_make_text(strdup("foo"), 3);

	/* When */
	msexpr_sexpr_stack_push(&stack, text);

	/* Then */
	ck_assert_ptr_eq(msexpr_sexpr_stack_top(&stack), text);
}
END_TEST

START_TEST(test_sexpr_GIVEN_initialized_stack_WHEN_push_twice_and_popped_once_THEN_pop_returns_second_push)
{
	/* Given */
	struct msexpr_sexpr_stack stack;
	msexpr_sexpr_stack_init(&stack, 4);

	/* When */
	struct msexpr * text1 = msexpr_make_text(strdup("foo"), 3);
	msexpr_sexpr_stack_push(&stack, text1);

	struct msexpr * text2 = msexpr_make_text(strdup("bar"), 3);
	msexpr_sexpr_stack_push(&stack, text2);

	struct msexpr * popped_element = msexpr_sexpr_stack_pop(&stack);

	/* Then */
	ck_assert_ptr_eq(popped_element, text2);
}
END_TEST

START_TEST(test_sexpr_GIVEN_initialized_stack_WHEN_push_twice_and_popped_once_THEN_top_returns_first_element)
{
	/* Given */
	struct msexpr_sexpr_stack stack;
	msexpr_sexpr_stack_init(&stack, 4);

	/* When */
	struct msexpr * text1 = msexpr_make_text(strdup("foo"), 3);
	msexpr_sexpr_stack_push(&stack, text1);

	struct msexpr * text2 = msexpr_make_text(strdup("bar"), 3);
	msexpr_sexpr_stack_push(&stack, text2);

	msexpr_sexpr_stack_pop(&stack);

	/* Then */
	ck_assert_ptr_eq(msexpr_sexpr_stack_top(&stack), text1);
}
END_TEST

START_TEST(test_sexpr_GIVEN_initialized_stack_WHEN_push_twice_and_popped_twice_THEN_top_returns_null)
{
	/* Given */
	struct msexpr_sexpr_stack stack;
	msexpr_sexpr_stack_init(&stack, 4);

	/* When */
	struct msexpr * text1 = msexpr_make_text(strdup("foo"), 3);
	msexpr_sexpr_stack_push(&stack, text1);

	struct msexpr * text2 = msexpr_make_text(strdup("bar"), 3);
	msexpr_sexpr_stack_push(&stack, text2);

	msexpr_sexpr_stack_pop(&stack);
	msexpr_sexpr_stack_pop(&stack);

	/* Then */
	ck_assert_ptr_eq(msexpr_sexpr_stack_top(&stack), NULL);
}
END_TEST

START_TEST(test_sexpr_GIVEN_initialized_stack_WHEN_push_twice_and_popped_twice_THEN_current_stack_size_is_zero)
{
	/* Given */
	struct msexpr_sexpr_stack stack;
	msexpr_sexpr_stack_init(&stack, 4);

	/* When */
	struct msexpr * text1 = msexpr_make_text(strdup("foo"), 3);
	msexpr_sexpr_stack_push(&stack, text1);

	struct msexpr * text2 = msexpr_make_text(strdup("bar"), 3);
	msexpr_sexpr_stack_push(&stack, text2);

	msexpr_sexpr_stack_pop(&stack);
	msexpr_sexpr_stack_pop(&stack);

	/* Then */
	ck_assert_int_eq(stack.current_size, 0);
}
END_TEST

Suite * msexpr_suite(void)
{
	Suite *s;
	TCase *tc_core;

	s = suite_create("msexpr");

	/* Core test case */
	tc_core = tcase_create("sexpr stack");

	tcase_add_test(tc_core, test_sexpr_GIVEN_uninitialized_stack_WHEN_init_THEN_stack_current_size_is_zero);
	tcase_add_test(tc_core, test_sexpr_GIVEN_initialized_stack_WHEN_top_THEN_return_null_ptr);
	tcase_add_test(tc_core, test_sexpr_GIVEN_initialized_stack_WHEN_push_element_THEN_top_returns_same_element);
	tcase_add_test(tc_core, test_sexpr_GIVEN_initialized_stack_WHEN_push_twice_and_popped_once_THEN_top_returns_first_element);
	tcase_add_test(tc_core, test_sexpr_GIVEN_initialized_stack_WHEN_push_twice_and_popped_once_THEN_pop_returns_second_push);
	tcase_add_test(tc_core, test_sexpr_GIVEN_initialized_stack_WHEN_push_twice_and_popped_twice_THEN_top_returns_null);
	tcase_add_test(tc_core, test_sexpr_GIVEN_initialized_stack_WHEN_push_twice_and_popped_twice_THEN_current_stack_size_is_zero);

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
