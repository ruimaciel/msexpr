#ifndef MPARSER_SEXPR_OPS_H
#define MPARSER_SEXPR_OPS_H

#include <msexpr/sexpr.h>

void msexpr_free_atom_text(struct msexpr_atom_text **atom);
void msexpr_free_atom_integer(struct msexpr_atom_integer **atom);
void msexpr_free_cell(struct msexpr_cell ** cell);

#endif
