#ifndef MPARSER_SEXPR_H
#define MPARSER_SEXPR_H

#include <stdlib.h>


enum msexpr_type
{
	MSEXPR_TYPE_NULL = 0
	, MSEXPR_TYPE_CELL			/* a s-expression cell, which includes a car and a cdr field */
	, MSEXPR_TYPE_ATOM_TEXT		/* a text string atom*/
};

/*
 * stores attributes common to all atoms
 */
struct msexpr
{
	enum msexpr_type type;
};

struct msexpr_cell
{
	struct msexpr sexpr;

	struct msexpr *car;	/* the car field/left half */
	struct msexpr *cdr;	/* the cdr field/right half */
};

struct msexpr_atom_text
{
	struct msexpr sexpr;
	size_t length;
	char * text;
};

int msexpr_init(struct msexpr * sexpr);
void msexpr_free(struct msexpr ** sexpr);

struct msexpr * msexpr_make_text(char * text, const size_t length);
struct msexpr * msexpr_cons(struct msexpr *car, struct msexpr *cdr);

#endif
