#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <check.h>

#include <src/deserializer/input.h>

static char * buffer = "1:a2:bc:3:def4:ghij:5:klmn:10:opqrstuvwx2:yz\0\0\0\0";

START_TEST(test_sexpr_GIVEN_zero_init_input_struct_WHEN_input_fill_THEN_input_becomes_valid)
{
    /* Given */
    FILE * stream = fmemopen(buffer,48,"r");
    struct input_t input = {0};

    /* When */
    input_fill(stream,&input,10);

    /* Then */
    ck_assert_ptr_ne(input.buffer, NULL);
    ck_assert_ptr_ne(input.limit, NULL);
    ck_assert_ptr_eq(input.token, input.buffer);
    ck_assert_ptr_eq(input.cursor, input.buffer);
    ck_assert_int_eq(input.buffer[0], buffer[0]);
}
END_TEST

START_TEST(test_sexpr_GIVEN_cursor_has_moved_WHEN_input_fill_THEN_input_becomes_valid)
{
    /* Given */
    FILE * stream = fmemopen(buffer,48,"r");
    struct input_t input = {0};
    const size_t position = 3;
    input_fill(stream,&input, position);

    input.token = input.cursor = input.marker = input.buffer+position;

    /* When */
    input_fill(stream,&input,1);

    /* Then */
    ck_assert_int_eq(input.buffer[0], buffer[position]);
    ck_assert_ptr_eq(input.token, input.buffer);
    ck_assert_ptr_eq(input.cursor, input.buffer);
}
END_TEST

Suite * msexpr_suite(void)
{
    Suite *s;
    TCase *tc_core;

    s = suite_create("msexpr");

    /* Core test case */
    tc_core = tcase_create("deserializer input");

    tcase_add_test(tc_core, test_sexpr_GIVEN_zero_init_input_struct_WHEN_input_fill_THEN_input_becomes_valid);
    tcase_add_test(tc_core, test_sexpr_GIVEN_cursor_has_moved_WHEN_input_fill_THEN_input_becomes_valid);

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
