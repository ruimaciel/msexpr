#include "sexpr_stack.h"

void msexpr_sexpr_stack_init(struct msexpr_sexpr_stack * stack, unsigned int initial_size)
{
	assert(stack != NULL);

	stack->array = (struct msexpr **)malloc(initial_size*(sizeof (struct msexpr *)));
	stack->current_size = 0;
	stack->max_size = initial_size;
}

int msexpr_sexpr_stack_grow(struct msexpr_sexpr_stack * stack)
{
	unsigned int new_size = stack->max_size>>1;
	struct msexpr ** array = realloc(stack->array, new_size );
	if(array == NULL)
	{
		/*TODO return descriptive exit code */
		return 1;
	}

	stack->array = array;
	stack->max_size = new_size;

	return 0;
}

int msexpr_sexpr_stack_push(struct msexpr_sexpr_stack * stack, struct msexpr * element)
{
	if(stack->current_size >= stack->max_size)
	{
		int retval = 0;
		retval = msexpr_sexpr_stack_grow(stack);
		if(retval != 0)
		{
			return retval;
		}
	}

	stack->array[stack->current_size++] = element;

	return 0;
}

struct msexpr * msexpr_sexpr_stack_top(struct msexpr_sexpr_stack * stack)
{
	assert(stack != NULL);

	return stack->current_size > 0 ? stack->array[stack->current_size-1] : NULL;
}

struct msexpr *msexpr_sexpr_stack_pop(struct msexpr_sexpr_stack * stack)
{
	assert(stack != NULL);

	return stack->current_size > 0 ? stack->array[--stack->current_size] : NULL;
}
