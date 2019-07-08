#include <msexpr/deserializers.h>

#include "deserializer/input.h"

#include "deserializer/rivest_canonical_lexer.h"
#include "sexpr_stack.h"

enum TOKEN_RIVEST_NT
{
    TOKEN_RIVEST_FIRST_NONTERMINAL = TOKEN_RIVEST_LAST,
    TOKEN_RIVEST_DOCUMENT,
    TOKEN_RIVEST_SEXPR,
    TOKEN_RIVEST_SEXPR_NEXT,
    TOKEN_RIVEST_LAST_NONTERMINAL
};

enum RIVEST_PARSER_STATE
{
    RIVEST_ST_ERROR = 0,
    RIVEST_ST_START_PARENTHESIS_OPEN,
    RIVEST_ST_START_LPVERBATIM_STRING,
    RIVEST_ST_SEXPR_OPEN,
    RIVEST_ST_SEXPR_LPVERBATIM_STRING,
    RIVEST_ST_SEXPR_NEXT_PARENTHESIS_OPEN,
    RIVEST_ST_SEXPR_NEXT_LPVERBATIM_STRING,
    RIVEST_ST_SEXPR_NEXT_PARENTHESIS_CLOSE,
    RIVEST_ST_TERMINAL_POP
};

struct msexpr_parse_result msexpr_parse_rivest_canonical(FILE *stream)
{
    struct msexpr * dom = NULL;
    struct msexpr_token_stack stack;
    struct msexpr_sexpr_stack cursor;
    struct input_t input = {0};
    struct token_t token = {0};

    /* set the parser table */
    short table[TOKEN_RIVEST_LAST_NONTERMINAL][TOKEN_RIVEST_LAST] = {{0}};

    /* set the parser tree */
    table[TOKEN_RIVEST_DOCUMENT][TOKEN_RIVEST_PARENTHESIS_OPEN] 	= RIVEST_ST_START_PARENTHESIS_OPEN;
    table[TOKEN_RIVEST_DOCUMENT][TOKEN_RIVEST_LPVERBATIM_STRING] 	= RIVEST_ST_START_LPVERBATIM_STRING;
    table[TOKEN_RIVEST_SEXPR][TOKEN_RIVEST_PARENTHESIS_OPEN] 		= RIVEST_ST_SEXPR_OPEN;
    table[TOKEN_RIVEST_SEXPR][TOKEN_RIVEST_LPVERBATIM_STRING] 		= RIVEST_ST_SEXPR_LPVERBATIM_STRING;
    table[TOKEN_RIVEST_SEXPR_NEXT][TOKEN_RIVEST_PARENTHESIS_OPEN] 	= RIVEST_ST_SEXPR_NEXT_PARENTHESIS_OPEN;
    table[TOKEN_RIVEST_SEXPR_NEXT][TOKEN_RIVEST_LPVERBATIM_STRING] 	= RIVEST_ST_SEXPR_NEXT_LPVERBATIM_STRING;
    table[TOKEN_RIVEST_SEXPR_NEXT][TOKEN_RIVEST_PARENTHESIS_CLOSE] 	= RIVEST_ST_SEXPR_NEXT_PARENTHESIS_CLOSE;
    table[TOKEN_RIVEST_PARENTHESIS_OPEN][TOKEN_RIVEST_PARENTHESIS_OPEN] 	= RIVEST_ST_TERMINAL_POP;
    table[TOKEN_RIVEST_PARENTHESIS_CLOSE][TOKEN_RIVEST_PARENTHESIS_CLOSE] 	= RIVEST_ST_TERMINAL_POP;
    table[TOKEN_RIVEST_LPVERBATIM_STRING][TOKEN_RIVEST_LPVERBATIM_STRING] 	= RIVEST_ST_TERMINAL_POP;
    table[TOKEN_RIVEST_EOF][TOKEN_RIVEST_EOF] 								= RIVEST_ST_TERMINAL_POP;

    /* init the cursor stack */
    msexpr_sexpr_stack_init(&cursor, 8);

    /* init the parser stack */
    msexpr_token_stack_init(&stack, 8);
    msexpr_token_stack_push(&stack, TOKEN_RIVEST_DOCUMENT);
    token = rivest_canonical_lexer(stream, &input);

    while(stack.current_size > 0) /* */
    /* while(token.token != TOKEN_RIVEST_EOF) /* */
    {
        const enum TOKEN_RIVEST_NT top = msexpr_token_stack_top(&stack);

        switch(table[top][token.token])
        {
        case RIVEST_ST_ERROR: /* error token */
        {
            struct msexpr_parse_result result = {MSEXPR_PARSER_UNKNOWN, NULL};
            return result;
        }

        case RIVEST_ST_START_PARENTHESIS_OPEN:
        {
            /* initialize the DOM */
            dom = msexpr_cons(NULL, NULL);
            msexpr_sexpr_stack_push(&cursor, dom);

            /* update parser stack */
            msexpr_token_stack_pop(&stack);
            msexpr_token_stack_push(&stack, TOKEN_RIVEST_EOF);
            msexpr_token_stack_push(&stack, TOKEN_RIVEST_PARENTHESIS_CLOSE);
            msexpr_token_stack_push(&stack, TOKEN_RIVEST_SEXPR_NEXT);
            msexpr_token_stack_push(&stack, TOKEN_RIVEST_SEXPR);
            msexpr_token_stack_push(&stack, TOKEN_RIVEST_PARENTHESIS_OPEN);
        }
            break;

        case RIVEST_ST_START_LPVERBATIM_STRING:
        {
            /* initialize the DOM */
            struct token_string_t * payload = token.payload;
            dom = msexpr_make_text(payload->text, payload->length);
            msexpr_sexpr_stack_push(&cursor, dom);

            /* update parser stack */
            msexpr_token_stack_pop(&stack);
            msexpr_token_stack_push(&stack, TOKEN_RIVEST_EOF);
            msexpr_token_stack_push(&stack, TOKEN_RIVEST_LPVERBATIM_STRING);
        }
            break;

        case RIVEST_ST_TERMINAL_POP:
        {
            msexpr_token_stack_pop(&stack);
            token = rivest_canonical_lexer(stream, &input);
        }
        break;

        case RIVEST_ST_SEXPR_OPEN:
        {
            /* TODO add tree node */
            struct msexpr_cell * cell = NULL;

            msexpr_token_stack_pop(&stack);
            msexpr_token_stack_push(&stack, TOKEN_RIVEST_PARENTHESIS_CLOSE);
            msexpr_token_stack_push(&stack, TOKEN_RIVEST_SEXPR_NEXT);
            msexpr_token_stack_push(&stack, TOKEN_RIVEST_SEXPR);
            msexpr_token_stack_push(&stack, TOKEN_RIVEST_PARENTHESIS_OPEN);

            /*TODO add cell to DOM */
            if(dom == NULL)
            {
                /* The DOM hasn't been initialized.  Let's initialize the DOM tree. */
                struct msexpr * root_cell = NULL;

                assert(cursor.current_size == 0);

                root_cell = msexpr_cons(NULL, NULL);
                dom = root_cell;

                msexpr_sexpr_stack_push(&cursor, root_cell);
            }
            else {
                struct msexpr_cell * current_cell = NULL;
                struct msexpr * new_cell = msexpr_cons(NULL, NULL);

                if( msexpr_sexpr_stack_top(&cursor)->type != MSEXPR_TYPE_CELL )
                {
                    /*TODO DOM is not valid */
                    struct msexpr_parse_result result = {MSEXPR_PARSER_UNKNOWN, NULL};
                    return result;
                }

                current_cell = (struct msexpr_cell *)msexpr_sexpr_stack_top(&cursor);

                if(current_cell->car == NULL)
                {
                    current_cell->car = new_cell;
                    msexpr_sexpr_stack_push(&cursor, new_cell);
                    continue;
                }

                if(current_cell->cdr != NULL)
                {
                    /* move cdr to a cell and append */
                    current_cell->cdr = msexpr_cons(current_cell->cdr, NULL);
                    msexpr_sexpr_stack_pop(&cursor);
                    msexpr_sexpr_stack_push(&cursor, current_cell->cdr);
                }

                current_cell->cdr = msexpr_cons(new_cell, NULL);
                msexpr_sexpr_stack_push(&cursor, new_cell);
            }

        }
            break;

        case RIVEST_ST_SEXPR_LPVERBATIM_STRING:
        {
            struct token_string_t * token_string = NULL;

            msexpr_token_stack_pop(&stack);
            msexpr_token_stack_push(&stack, TOKEN_RIVEST_LPVERBATIM_STRING);

            /*TODO add string to DOM */

            token_string = (struct token_string_t *)(&token);
            if(dom == NULL) {
                /* The DOM hasn't been initialized.  Let's initialize the DOM tree. */
                struct msexpr * text = NULL;

                assert(cursor.current_size == 0);

                text = msexpr_make_text(token_string->text, token_string->length);
                dom = text;

                msexpr_sexpr_stack_push(&cursor, text);
            }
            else {
                /* The DOM tree has been initialize. Thus it shall be updated by adding the new text string */
                struct msexpr * text = NULL;
                struct msexpr_cell * current_cell = NULL;

                assert(cursor.current_size > 0);

                text = msexpr_make_text(token_string->text, token_string->length);

                if( msexpr_sexpr_stack_top(&cursor)->type != MSEXPR_TYPE_CELL )
                {
                    /*TODO DOM is not valid */
                    struct msexpr_parse_result result = {MSEXPR_PARSER_UNKNOWN, NULL};
                    return result;
                }

                current_cell = (struct msexpr_cell *)msexpr_sexpr_stack_top(&cursor);

                if(current_cell->car == NULL)
                {
                    current_cell->car = text;
                    continue;
                }

                if(current_cell->cdr != NULL)
                {
                    current_cell->cdr = msexpr_cons(current_cell->cdr, NULL);
                    msexpr_sexpr_stack_pop(&cursor);
                    msexpr_sexpr_stack_push(&cursor, current_cell->cdr);
                }

            }
        }
        break;

        case RIVEST_ST_SEXPR_NEXT_PARENTHESIS_OPEN:
        {
            msexpr_token_stack_pop(&stack);
            msexpr_token_stack_push(&stack, TOKEN_RIVEST_PARENTHESIS_CLOSE);
            msexpr_token_stack_push(&stack, TOKEN_RIVEST_SEXPR_NEXT);
            msexpr_token_stack_push(&stack, TOKEN_RIVEST_SEXPR);
            msexpr_token_stack_push(&stack, TOKEN_RIVEST_PARENTHESIS_OPEN);
        }
        break;

        case RIVEST_ST_SEXPR_NEXT_LPVERBATIM_STRING:
        {
            msexpr_token_stack_pop(&stack);
            msexpr_token_stack_push(&stack, TOKEN_RIVEST_SEXPR_NEXT);
            msexpr_token_stack_push(&stack, TOKEN_RIVEST_LPVERBATIM_STRING);
        }
        break;

        case RIVEST_ST_SEXPR_NEXT_PARENTHESIS_CLOSE:
        {
            msexpr_token_stack_pop(&stack);
        }
        break;

        default:	/* unsupported state */
        {
            struct msexpr_parse_result result = {MSEXPR_PARSER_UNKNOWN, NULL};
            return result;
        }

        }

    }

    {
        struct msexpr_parse_result result = {MSEXPR_PARSER_OK, dom};
        return result;
    }
}
