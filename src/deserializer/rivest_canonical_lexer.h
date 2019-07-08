#ifndef MPARSER_SEXPR_DESERIALIZER_LEXER_RIVEST_CANONICAL_H
#define MPARSER_SEXPR_DESERIALIZER_LEXER_RIVEST_CANONICAL_H

#include <stdio.h>

#include "input.h"

/*
 * Parser for the canonical s-expressions format described in
 * section 6.1 of:
 * http://people.csail.mit.edu/rivest/Sexp.txt
 */

enum TOKEN_RIVEST_T {
    TOKEN_RIVEST_ERROR = 0,
    TOKEN_RIVEST_PARENTHESIS_OPEN,
    TOKEN_RIVEST_PARENTHESIS_CLOSE,
    TOKEN_RIVEST_LPVERBATIM_STRING,	/* lenght-prefixed "verbatim" encoding string */
    TOKEN_RIVEST_EOF,
    TOKEN_RIVEST_LAST
};

struct token_t rivest_canonical_lexer(FILE* stream, struct input_t *input);

#endif
