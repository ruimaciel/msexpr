#ifndef MPARSER_SEXPR_DESERIALIZER_LEXER_INPUT_H
#define MPARSER_SEXPR_DESERIALIZER_LEXER_INPUT_H

#include <stdio.h>

#include "token.h"

struct input_t {
    char *limit;
    char *cursor;
    char *marker;
    char *buffer;
    char *token;	/* points to the beginning of the current lexeme */
    size_t buffer_size;
    unsigned short padding;
    int has_reached_eof;
};

int input_fill(FILE *stream, struct input_t *input, const size_t required_size);

#endif
