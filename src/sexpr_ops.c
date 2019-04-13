#include "sexpr_ops.h"

#include <assert.h>

#include <msexpr/sexpr.h>

void msexpr_free_atom_text(struct msexpr_atom_text **atom)
{
    assert(atom != NULL);
    if(*atom == NULL)
    {
        return;
    }

    if((*atom)->text != NULL)
    {
        free((*atom)->text);
        (*atom)->text = NULL;
    }

    free(*atom);
    *atom = NULL;
}


void msexpr_free_atom_integer(struct msexpr_atom_integer **atom)
{
    assert(atom != NULL);
    if(*atom == NULL)
    {
        return;
    }

    free(*atom);
    *atom = NULL;
}

void msexpr_free_atom_float(struct msexpr_atom_float **atom)
{
    assert(atom != NULL);
    if(*atom == NULL)
    {
        return;
    }

    free(*atom);
    *atom = NULL;
}

void msexpr_free_cell(struct msexpr_cell **cell)
{
    if(cell == NULL || *cell == NULL)
    {
        return;
    }

    msexpr_free( &(*cell)->car );
    msexpr_free( &(*cell)->cdr );

    free(*cell);
    *cell = NULL;
}
