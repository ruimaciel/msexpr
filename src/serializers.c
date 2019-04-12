#include <msexpr/serializers.h>

int msexpr_fprint_csexpr(FILE *stream, struct msexpr *sexpr)
{
    if(sexpr == NULL)
    {
        fprintf(stream,"nil");
        return 0;
    }


    switch(sexpr->type)
    {

    case MSEXPR_TYPE_NULL:
    {
        fprintf(stream,"nil");
    }
        break;

    case MSEXPR_TYPE_ATOM_TEXT:
    {
        struct msexpr_atom_text* sexpr_text = (struct msexpr_atom_text*)sexpr;
        fprintf(stream,"%lu:%s", sexpr_text->length, sexpr_text->text);
    }
        break;

    case MSEXPR_TYPE_CELL:
    {
        fprintf(stream,"(");
        struct msexpr_cell * sexpr_cell = (struct msexpr_cell*)sexpr;
        msexpr_fprint_csexpr(stream, sexpr_cell->car);
        fprintf(stream," . ");
        msexpr_fprint_csexpr(stream, sexpr_cell->cdr);
        fprintf(stream,")");
        break;
    }

    }

    return 0;
}
