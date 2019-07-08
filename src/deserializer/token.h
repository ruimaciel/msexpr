#ifndef MPARSER_SEXPR_DESERIALIZER_TOKEN_H
#define MPARSER_SEXPR_DESERIALIZER_TOKEN_H

#include <stddef.h>


struct token_t
{
    int token;
    void * payload;
};


struct token_string_t
{
    size_t length;
    char * text;
};

struct msexpr_token_stack
{
    unsigned short * array;
    unsigned int current_size, max_size;
};

void msexpr_token_stack_init(struct msexpr_token_stack *stack, unsigned int initial_size);
int msexpr_token_stack_grow(struct msexpr_token_stack *stack);
int msexpr_token_stack_push(struct msexpr_token_stack *stack, unsigned short element);
unsigned short msexpr_token_stack_top(struct msexpr_token_stack *stack);
unsigned short msexpr_token_stack_pop(struct msexpr_token_stack *stack);

#endif
