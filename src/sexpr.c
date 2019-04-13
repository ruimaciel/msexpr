#include <msexpr/sexpr.h>

#include <stdio.h> /* for NULL */
#include <stdlib.h>
#include <assert.h>

#include "sexpr_ops.h"


int msexpr_init(struct msexpr * sexpr)
{
    assert(sexpr != NULL);

    sexpr->type = MSEXPR_TYPE_NULL;

    return 0;
}

void msexpr_free(struct msexpr ** sexpr)
{
    if(sexpr == NULL || *sexpr == NULL)
    {
        return;
    }

    switch( (*sexpr)->type )
    {
    case MSEXPR_TYPE_ATOM_TEXT:
        msexpr_free_atom_text( (struct msexpr_atom_text **)(sexpr));
        break;

    case MSEXPR_TYPE_ATOM_INTEGER:
        msexpr_free_atom_integer( (struct msexpr_atom_integer **)(sexpr));
        break;

    case MSEXPR_TYPE_ATOM_FLOAT:
        msexpr_free_atom_float( (struct msexpr_atom_float **)(sexpr));
        break;

    case MSEXPR_TYPE_CELL:
        msexpr_free_cell((struct msexpr_cell **)(sexpr));
        break;

    default:
        break;
    }
}

struct msexpr * msexpr_make_text(char * text, const size_t length)
{
    struct msexpr_atom_text *atom = (struct msexpr_atom_text *)malloc(sizeof(struct msexpr_atom_text));

    if(atom == NULL)
    {
        return NULL;
    }

    /* init the text atom */
    atom->sexpr.type = MSEXPR_TYPE_ATOM_TEXT;
    atom->text = text;
    atom->length = length;

    return (struct msexpr *)atom;
}

struct msexpr * msexpr_make_integer(const long int value)
{
    struct msexpr_atom_integer *atom = (struct msexpr_atom_integer *)malloc(sizeof(struct msexpr_atom_integer));

    if(atom == NULL)
    {
        return NULL;
    }

    /* init the integer atom */
    atom->sexpr.type = MSEXPR_TYPE_ATOM_INTEGER;
    atom->value = value;

    return (struct msexpr *)atom;
}

struct msexpr * msexpr_make_float(const double value)
{
    struct msexpr_atom_float *atom = (struct msexpr_atom_float *)malloc(sizeof(struct msexpr_atom_float));

    if(atom == NULL)
    {
        return NULL;
    }

    /* init the float atom */
    atom->sexpr.type = MSEXPR_TYPE_ATOM_FLOAT;
    atom->value = value;

    return (struct msexpr *)atom;
}

struct msexpr * msexpr_cons(struct msexpr *car, struct msexpr *cdr)
{
    struct msexpr_cell *cell = (struct msexpr_cell *)malloc(sizeof(struct msexpr_cell));

    if(cell == NULL)
    {
        return NULL;
    }

    cell->sexpr.type = MSEXPR_TYPE_CELL;
    cell->car = car;
    cell->cdr = cdr;

    return (struct msexpr *)cell;
}
