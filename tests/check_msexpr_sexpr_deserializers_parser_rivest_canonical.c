#include <stdlib.h>
#include <stdio.h>
#include <check.h>

#include <msexpr/deserializers.h>


START_TEST(test_GIVEN_verbatim_strings_WHEN_parse_string_THEN_return_OK)
{
    /* Given */
    char * buffer = "2:ab\0\0\0\0";
    FILE * stream = fmemopen(buffer,8,"r");

    /* When */
    struct msexpr_parse_result return_code = msexpr_parse_rivest_canonical(stream);

    /* Then */
    ck_assert_int_eq(return_code.result, MSEXPR_PARSER_OK);
}
END_TEST

START_TEST(test_GIVEN_parens_with_verbatim_strings_WHEN_parse_string_THEN_return_OK)
{
    /* Given */
    char * buffer = "(2:ab)\0\0\0\0";
    FILE * stream = fmemopen(buffer,8,"r");

    /* When */
    struct msexpr_parse_result return_code = msexpr_parse_rivest_canonical(stream);

    /* Then */
    ck_assert_int_eq(return_code.result, MSEXPR_PARSER_OK);
}
END_TEST

START_TEST(test_GIVEN_parens_with_two_verbatim_strings_WHEN_parse_string_THEN_return_OK)
{
    /* Given */
    char * buffer = "(2:ab3:cde)\0\0\0\0";
    FILE * stream = fmemopen(buffer,15,"r");

    /* When */
    struct msexpr_parse_result return_code = msexpr_parse_rivest_canonical(stream);

    /* Then */
    ck_assert_int_eq(return_code.result, MSEXPR_PARSER_OK);
}
END_TEST

START_TEST(test_GIVEN_parens_with_nested_parens_two_verbatim_strings_WHEN_parse_string_THEN_return_OK)
{
    /* Given */
    char * buffer = "((2:ab3:cde)4:fghi)\0\0\0\0";
    FILE * stream = fmemopen(buffer,22,"r");

    /* When */
    struct msexpr_parse_result return_code = msexpr_parse_rivest_canonical(stream);

    /* Then */
    ck_assert_int_eq(return_code.result, MSEXPR_PARSER_OK);
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

    s = suite_create("msexpr deserializer");

    /* Core test case */
    tc_core = tcase_create("rivest canonical lexer");

    tcase_add_test(tc_core, test_GIVEN_verbatim_strings_WHEN_parse_string_THEN_return_OK);
    tcase_add_test(tc_core, test_GIVEN_parens_with_verbatim_strings_WHEN_parse_string_THEN_return_OK);
    tcase_add_test(tc_core, test_GIVEN_parens_with_two_verbatim_strings_WHEN_parse_string_THEN_return_OK);
    tcase_add_test(tc_core, test_GIVEN_parens_with_nested_parens_two_verbatim_strings_WHEN_parse_string_THEN_return_OK);

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
