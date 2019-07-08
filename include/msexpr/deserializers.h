#ifndef MPARSER_SEXPR_DESERIALIZERS_H
#define MPARSER_SEXPR_DESERIALIZERS_H

#include <stdio.h>
#include <msexpr/sexpr.h>

enum msexpr_parse_return_code
{
    MSEXPR_PARSER_OK = 0,
    MSEXPR_PARSER_UNKNOWN
};

struct msexpr_parse_result
{
    enum msexpr_parse_return_code result;
    struct msexpr *sexpr;
};

/*
 * Parser for the canonical s-expressions format described in
 * section 6.1 of:
 * http://people.csail.mit.edu/rivest/Sexp.txt
 */
struct msexpr_parse_result msexpr_parse_rivest_canonical(FILE *stream);

#endif
