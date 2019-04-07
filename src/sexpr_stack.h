#ifndef MPARSER_SEXPR_STACK_H
#define MPARSER_SEXPR_STACK_H

#include <stdlib.h>
#include <assert.h>

#include <msexpr/sexpr.h>

struct msexpr_sexpr_stack
{
	struct msexpr ** array;
	unsigned int current_size, max_size;
};

void msexpr_sexpr_stack_init(struct msexpr_sexpr_stack *stack, unsigned int initial_size);
int msexpr_sexpr_stack_grow(struct msexpr_sexpr_stack *stack);
int msexpr_sexpr_stack_push(struct msexpr_sexpr_stack *stack, struct msexpr *element);
struct msexpr * msexpr_sexpr_stack_top(struct msexpr_sexpr_stack *stack);
struct msexpr * msexpr_sexpr_stack_pop(struct msexpr_sexpr_stack *stack);

#endif
