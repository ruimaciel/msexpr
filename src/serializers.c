#include <msexpr/serializers.h>

int msexpr_fprint_csexpr_text_netstring(FILE *stream, const char *text, size_t length)
{
    return fprintf(stream,"%lu:%s", length, text);
}

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
        msexpr_fprint_csexpr_text_netstring(stream, sexpr_text->text, sexpr_text->length);
    }
        break;

    case MSEXPR_TYPE_ATOM_INTEGER:
    {
        struct msexpr_atom_integer* sexpr_number = (struct msexpr_atom_integer*)sexpr;
        fprintf(stream,"%li", sexpr_number->value);
    }
        break;

    case MSEXPR_TYPE_ATOM_FLOAT:
    {
        struct msexpr_atom_float* sexpr_number = (struct msexpr_atom_float*)sexpr;
        fprintf(stream,"%f", sexpr_number->value);
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
