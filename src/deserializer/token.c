#include "token.h"

#include <assert.h>
#include <stdlib.h>


void msexpr_token_stack_init(struct msexpr_token_stack * stack, unsigned int initial_size)
{
    assert(stack != NULL);

    stack->array = calloc(initial_size, sizeof(unsigned short));
    stack->current_size = 0;
    stack->max_size = initial_size;
}

int msexpr_token_stack_grow(struct msexpr_token_stack * stack)
{
    unsigned int new_size = stack->max_size>>1;
    unsigned short * array = realloc(stack->array, new_size );
    if(array == NULL)
    {
        /*TODO return descriptive exit code */
        return 1;
    }

    stack->array = array;
    stack->max_size = new_size;

    return 0;
}

int msexpr_token_stack_push(struct msexpr_token_stack * stack, unsigned short element)
{
    if(stack->current_size >= stack->max_size)
    {
        int retval = 0;
        retval = msexpr_token_stack_grow(stack);
        if(retval != 0)
        {
            return retval;
        }
    }

    stack->array[stack->current_size++] = element;

    return 0;
}

unsigned short msexpr_token_stack_top(struct msexpr_token_stack * stack)
{
    assert(stack != NULL);

    return stack->current_size > 0 ? stack->array[stack->current_size-1] : 0;
}

unsigned short msexpr_token_stack_pop(struct msexpr_token_stack * stack)
{
    unsigned short token = 0;

    assert(stack != NULL);

/*    return stack->current_size > 0 ? stack->array[--stack->current_size] : 0; */
    token = msexpr_token_stack_top(stack);
    stack->current_size--;
    stack->array[stack->current_size] = 0;
    return token;
}
