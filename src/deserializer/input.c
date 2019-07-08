#include "input.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stddef.h>
#include <assert.h>

int input_fill(FILE * stream, struct input_t *in, const size_t required_size)
{
    ptrdiff_t free = 0;

    assert(stream != NULL);
    assert(in != NULL);

    if (in->has_reached_eof)
    {
        return 1;
    }

    if(in->buffer == NULL)
    {
        /* initialize input */
        const size_t buffer_size = 16;	/* FIXIT don't use hard-coded values */
        in->padding = 4;
        unsigned char * new_buffer = malloc(buffer_size + in->padding);
        if(new_buffer == NULL)
        {
            return 4;	/* FIXIT use proper error handling */
        }

        in->buffer = new_buffer;
        in->buffer_size = buffer_size;
        /* set the pointers in a state that triggers a refill */
        in->token = in->cursor = in->marker = new_buffer + buffer_size;
        in->limit = new_buffer + buffer_size + in->padding;
        memset(in->buffer, 0, buffer_size + in->padding);
    }

    free = in->token - in->buffer;
    if (free < required_size)
    {
        return 2;
    }

    memmove(in->buffer, in->token, in->limit - in->token);
    in->limit -= free;
    in->cursor -= free;
    in->marker -= free;
    in->token -= free;

    in->limit += fread(in->limit-in->padding, 1, (size_t)free, stream);
    if (in->limit < in->buffer + in->buffer_size)
    {
        const size_t maxfill = in->buffer_size - (size_t)(in->limit - in->buffer);
        in->has_reached_eof = 1;
        memset(in->limit, 0, maxfill);
        in->limit += maxfill;
    }
    return 0;
}
