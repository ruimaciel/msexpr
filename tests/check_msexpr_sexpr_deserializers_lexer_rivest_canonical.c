#include <stdlib.h>
#include <stdio.h>
#include <check.h>

#include <src/deserializer/input.h>
#include <src/deserializer/rivest_canonical_lexer.h>


START_TEST(test_GIVEN_string_of_verbatim_strings_WHEN_call_lexer_once_THEN_return_first_token)
{
    /* Given */
    char * buffer = "1:a2:bc:3:def4:ghij:5:klmn:10:opqrstuvwx2:yz\0\0\0\0";
    FILE * stream = fmemopen(buffer,48,"r");
    struct input_t input = {0};

    /* When */
    struct token_t token;
    token = rivest_canonical_lexer(stream, &input);

    /* Then */
    ck_assert_int_eq(token.token, TOKEN_RIVEST_LPVERBATIM_STRING);
    ck_assert_ptr_ne(token.payload, NULL);

    struct token_string_t *s = (struct token_string_t *)token.payload;
    ck_assert_int_eq(s->length, 1);
    ck_assert_ptr_ne(s->text, NULL);
    ck_assert_int_eq(s->text[0], 'a');
}
END_TEST

START_TEST(test_GIVEN_string_of_verbatim_strings_WHEN_call_lexer_twice_THEN_return_second_token)
{
    /* Given */
    char * buffer = "1:a2:bc:3:def4:ghij:5:klmn:10:opqrstuvwx2:yz\0\0\0\0";
    FILE * stream = fmemopen(buffer,48,"r");
    struct input_t input = {0};

    /* When */
    struct token_t token;
    token = rivest_canonical_lexer(stream, &input);
    token = rivest_canonical_lexer(stream, &input);

    /* Then */
    ck_assert_int_eq(token.token, TOKEN_RIVEST_LPVERBATIM_STRING);
    ck_assert_ptr_ne(token.payload, NULL);

    struct token_string_t *s = (struct token_string_t *)token.payload;
    ck_assert_int_eq(s->length, 2);
    ck_assert_int_eq(s->text[0], 'b');
    ck_assert_int_eq(s->text[1], 'c');
}
END_TEST

START_TEST(test_GIVEN_string_with_open_parenthesis_WHEN_call_lexer_once_THEN_return_open_parenthesis_token)
{
    /* Given */
    char * buffer = "(\0\0\0\0";
    FILE * stream = fmemopen(buffer,6,"r");
    struct input_t input = {0};

    /* When */
    struct token_t token;
    token = rivest_canonical_lexer(stream, &input);

    /* Then */
    ck_assert_int_eq(token.token, TOKEN_RIVEST_PARENTHESIS_OPEN);
}
END_TEST

START_TEST(test_GIVEN_string_with_close_parenthesis_WHEN_call_lexer_once_THEN_return_close_parenthesis_token)
{
    /* Given */
    char * buffer = ")\0\0\0\0";
    FILE * stream = fmemopen(buffer,6,"r");
    struct input_t input = {0};

    /* When */
    struct token_t token;
    token = rivest_canonical_lexer(stream, &input);

    /* Then */
    ck_assert_int_eq(token.token, TOKEN_RIVEST_PARENTHESIS_CLOSE);
}
END_TEST

START_TEST(test_GIVEN_string_with_open_and_close_parenthesis_WHEN_call_lexer_twice_THEN_return_open_and_close_parenthesis_tokens)
{
    /* Given */
    char * buffer = "()\0\0\0\0";
    FILE * stream = fmemopen(buffer,6,"r");
    struct input_t input = {0};

    /* When */
    struct token_t token1;
    token1 = rivest_canonical_lexer(stream, &input);

    struct token_t token2;
    token2 = rivest_canonical_lexer(stream, &input);

    /* Then */
    ck_assert_int_eq(token1.token, TOKEN_RIVEST_PARENTHESIS_OPEN);
    ck_assert_int_eq(token2.token, TOKEN_RIVEST_PARENTHESIS_CLOSE);
}
END_TEST

START_TEST(test_GIVEN_string_with_null_characters_WHEN_call_lexer_THEN_return_EOF_token)
{
    /* Given */
    char * buffer = "\0\0\0\0";
    FILE * stream = fmemopen(buffer,6,"r");
    struct input_t input = {0};

    /* When */
    struct token_t token;
    token = rivest_canonical_lexer(stream, &input);

    /* Then */
    ck_assert_int_eq(token.token, TOKEN_RIVEST_EOF);
}
END_TEST

START_TEST(test_GIVEN_string_with_nested_verbatim_string_WHEN_call_lexer_three_times_THEN_return_close_parenthesis)
{
    /* Given */
    char * buffer = "(2:ab)\0\0\0\0";
    FILE * stream = fmemopen(buffer,6,"r");
    struct input_t input = {0};

    /* When */
    struct token_t token1;
    token1 = rivest_canonical_lexer(stream, &input);

    struct token_t token2;
    token2 = rivest_canonical_lexer(stream, &input);

    struct token_t token3;
    token3 = rivest_canonical_lexer(stream, &input);

    /* Then */
    ck_assert_int_eq(token3.token, TOKEN_RIVEST_PARENTHESIS_CLOSE);
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

    tcase_add_test(tc_core, test_GIVEN_string_of_verbatim_strings_WHEN_call_lexer_once_THEN_return_first_token);
    tcase_add_test(tc_core, test_GIVEN_string_of_verbatim_strings_WHEN_call_lexer_twice_THEN_return_second_token);
    tcase_add_test(tc_core, test_GIVEN_string_with_open_parenthesis_WHEN_call_lexer_once_THEN_return_open_parenthesis_token);
    tcase_add_test(tc_core, test_GIVEN_string_with_close_parenthesis_WHEN_call_lexer_once_THEN_return_close_parenthesis_token);
    tcase_add_test(tc_core, test_GIVEN_string_with_open_and_close_parenthesis_WHEN_call_lexer_twice_THEN_return_open_and_close_parenthesis_tokens);
    tcase_add_test(tc_core, test_GIVEN_string_with_null_characters_WHEN_call_lexer_THEN_return_EOF_token);
    tcase_add_test(tc_core, test_GIVEN_string_with_nested_verbatim_string_WHEN_call_lexer_three_times_THEN_return_close_parenthesis);

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
