#ifndef RUBY_PARSER_NODE_HH
#define RUBY_PARSER_NODE_HH

#include <memory>
#include <vector>

#include "token.hh"

namespace ruby_parser {
  enum class node_type {
    ENCODING_LITERAL      = 1,
    FILE_LITERAL          = 2,
    LINE_LITERAL          = 3,
    ALIAS                 = 4,
    AND_ASGN              = 5,
    ARG                   = 6,
    ARG_EXPR              = 7,
    ARGS                  = 8,
    ARRAY                 = 9,
    BACK_REF              = 10,
    BEGIN                 = 11,
    BLOCK                 = 12,
    BLOCK_PASS            = 13,
    BLOCKARG              = 14,
    BLOCKARG_EXPR         = 15,
    CASE                  = 16,
    CBASE                 = 17,
    CLASS                 = 18,
    CONST                 = 19,
    CVAR                  = 20,
    DEF                   = 21,
    DEFS                  = 22,
    DSTR                  = 23,
    DSYM                  = 24,
    ENSURE                = 25,
    EXRANGE               = 26,
    FALSE                 = 27,
    FOR                   = 28,
    GVAR                  = 29,
    HASH                  = 30,
    IDENT                 = 31,
    IF                    = 32,
    INT                   = 33,
    INRANGE               = 34,
    IVAR                  = 35,
    KWARG                 = 36,
    KWBEGIN               = 37,
    KWOPTARG              = 38,
    KWRESTARG             = 39,
    KWSPLAT               = 40,
    LAMBDA                = 41,
    MASGN                 = 42,
    MATCH_CURRENT_LINE    = 43,
    MATCH_WITH_LVASGN     = 44,
    MLHS                  = 45,
    MODULE                = 46,
    NIL                   = 47,
    NOT                   = 48,
    NTH_REF               = 49,
    OBJC_KWARG            = 50,
    OBJC_RESTARG          = 51,
    OP_ASGN               = 52,
    OPTARG                = 53,
    OR_ASGN               = 54,
    PAIR                  = 55,
    POSTEXE               = 56,
    PREEXE                = 57,
    PROTOTYPE             = 58,
    REGEXP                = 59,
    REGOPT                = 60,
    RESBODY               = 61,
    RESCUE                = 62,
    RESTARG               = 63,
    RESTARG_EXPR          = 64,
    SCLASS                = 65,
    SELF                  = 66,
    SEND                  = 67,
    SHADOWARG             = 68,
    SPLAT                 = 69,
    STR                   = 70,
    SYM                   = 71,
    TR_ARRAY              = 72,
    TR_CAST               = 73,
    TR_CPATH              = 74,
    TR_GENARGS            = 75,
    TR_GENDECL            = 76,
    TR_GENINST            = 77,
    TR_HASH               = 78,
    TR_IVARDECL           = 79,
    TR_NIL                = 80,
    TR_NILLABLE           = 81,
    TR_OR                 = 82,
    TR_PROC               = 83,
    TR_SPECIAL            = 84,
    TR_TUPLE              = 85,
    TRUE                  = 86,
    TYPED_ARG             = 87,
    UNDEF                 = 88,
    WHEN                  = 89,
    XSTR                  = 90,
    WHILE                 = 91,
    UNTIL                 = 92,
    AND                   = 93,
    OR                    = 94,
    SUPER                 = 95,
    YIELD                 = 96,
    RETURN                = 97,
    BREAK                 = 98,
    NEXT                  = 99,
    DEFINED               = 100,
    REDO                  = 101,
    RETRY                 = 102,
    ZSUPER                = 103,

    // internal pseudo-nodes
    // TODO - move these out of node.hh and into something specific to the
    // parser
    NODE_LIST             = -1,
    NODE_DELIMITED_LIST   = -2,
    NODE_DELIMITED_BLOCK  = -3,
    NODE_WITH_TOKEN       = -4,
  };
}

#endif
