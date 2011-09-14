/* A Bison parser, made by GNU Bison 2.7.  */

/* Bison implementation for Yacc-like parsers in C
   
      Copyright (C) 1984, 1989-1990, 2000-2012 Free Software Foundation, Inc.
   
   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.
   
   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.
   
   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.  */

/* As a special exception, you may create a larger work that contains
   part or all of the Bison parser skeleton and distribute that work
   under terms of your choice, so long as that work isn't itself a
   parser generator using the skeleton or a modified version thereof
   as a parser skeleton.  Alternatively, if you modify or redistribute
   the parser skeleton itself, you may (at your option) remove this
   special exception, which will cause the skeleton and the resulting
   Bison output files to be licensed under the GNU General Public
   License without this special exception.
   
   This special exception was added by the Free Software Foundation in
   version 2.2 of Bison.  */

/* C LALR(1) parser skeleton written by Richard Stallman, by
   simplifying the original so-called "semantic" parser.  */

/* All symbols defined below should begin with yy or YY, to avoid
   infringing on user name space.  This should be done even for local
   variables, as they might otherwise be expanded by user macros.
   There are some unavoidable exceptions within include files to
   define necessary library symbols; they are noted "INFRINGES ON
   USER NAME SPACE" below.  */

/* Identify Bison output.  */
#define YYBISON 1

/* Bison version.  */
#define YYBISON_VERSION "2.7"

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 1

/* Push parsers.  */
#define YYPUSH 0

/* Pull parsers.  */
#define YYPULL 1


/* Substitute the variable and function names.  */
#define yyparse         xcp_yyparse
#define yylex           xcp_yylex
#define yyerror         xcp_yyerror
#define yylval          xcp_yylval
#define yychar          xcp_yychar
#define yydebug         xcp_yydebug
#define yynerrs         xcp_yynerrs
#define yylloc          xcp_yylloc

/* Copy the first part of user declarations.  */
/* Line 371 of yacc.c  */
#line 24 "/home/vdust/devel/xmms2/xmms2-all/src/clients/lib/xmmsclient/collparser/parser.y"

#include <stdio.h> /* snprintf */
#include <string.h> /* strdup, strlen */

#include "collparser_priv.h"
#include "parser_data_priv.h"
#include "collbuilder_priv.h"
#include "utils_priv.h"
#include "errors_priv.h"

/* uncomment this line to show all reductions. */
/* #define SHOW_REDUCTION 1 */
/* Line 371 of yacc.c  */
#line 42 "/home/vdust/devel/xmms2/xmms2-all/src/clients/lib/xmmsclient/collparser/parser.y"

#ifdef SHOW_REDUCTION
# define PRINT_R(from, to) printf ("%s <-- %s\n", to, from)
#else
# define PRINT_R(from, to) ((void)0)
#endif

static char *_prepare_match (const char *s, int alt);
void xcp_yyerror (YYLTYPE *locp, xmmsv_collparser_t *collparser, void *scanner, const char *msg);
int xcp_yylex (YYSTYPE *lval, YYLTYPE *lloc, void *scanner);

/* Line 371 of yacc.c  */
#line 102 "parser.tab.c"

# ifndef YY_NULL
#  if defined __cplusplus && 201103L <= __cplusplus
#   define YY_NULL nullptr
#  else
#   define YY_NULL 0
#  endif
# endif

/* Enabling verbose error messages.  */
#ifdef YYERROR_VERBOSE
# undef YYERROR_VERBOSE
# define YYERROR_VERBOSE 1
#else
# define YYERROR_VERBOSE 1
#endif

/* In a future release of Bison, this section will be replaced
   by #include "parser.tab.h".  */
#ifndef YY_XCP_YY_PARSER_TAB_H_INCLUDED
# define YY_XCP_YY_PARSER_TAB_H_INCLUDED
/* Enabling traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif
#if YYDEBUG
extern int xcp_yydebug;
#endif

/* Tokens.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
   /* Put the tokens into the symbol table, so that GDB and other debuggers
      know about them.  */
   enum yytokentype {
     TOKEN_EOF = 258,
     TOKEN_EOF_FILTER = 259,
     TOKEN_EOF_OPERATOR = 260,
     TOKEN_EOF_PROPERTY = 261,
     TOKEN_EOF_QUOTE = 262,
     TOKEN_EOF_SEQUENCE = 263,
     TOKEN_EOF_STRING = 264,
     TOKEN_EOF_UNIVERSE = 265,
     TOKEN_ERR = 266,
     TOKEN_FILTER_EQUAL = 267,
     TOKEN_FILTER_GREATER = 268,
     TOKEN_FILTER_GREATEREQUAL = 269,
     TOKEN_FILTER_HAS = 270,
     TOKEN_FILTER_LESS = 271,
     TOKEN_FILTER_LESSEQUAL = 272,
     TOKEN_FILTER_MATCH = 273,
     TOKEN_FILTER_MATCH_GLOB = 274,
     TOKEN_FILTER_TOKEN = 275,
     TOKEN_GROUP_CLOSE = 276,
     TOKEN_GROUP_OPEN = 277,
     TOKEN_ID = 278,
     TOKEN_ID_RANGE = 279,
     TOKEN_ID_SEQUENCE = 280,
     TOKEN_OPERATOR_AND = 281,
     TOKEN_OPERATOR_NOT = 282,
     TOKEN_OPERATOR_OR = 283,
     TOKEN_PROPERTY = 284,
     TOKEN_PROPERTY_EOF_QUOTE = 285,
     TOKEN_REFERENCE = 286,
     TOKEN_STRING = 287,
     TOKEN_SYMBOL_ID = 288,
     TOKEN_UNIVERSE = 289
   };
#endif


#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef union YYSTYPE
{
/* Line 387 of yacc.c  */
#line 54 "/home/vdust/devel/xmms2/xmms2-all/src/clients/lib/xmmsclient/collparser/parser.y"

	int ival;
	unsigned int group;
	char *str;
	struct {
		xmmsv_coll_type_t f;
		int alt;
	} filop;
	xcp_data_t *data;


/* Line 387 of yacc.c  */
#line 191 "parser.tab.c"
} YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
#endif

#if ! defined YYLTYPE && ! defined YYLTYPE_IS_DECLARED
typedef struct YYLTYPE
{
  int first_line;
  int first_column;
  int last_line;
  int last_column;
} YYLTYPE;
# define yyltype YYLTYPE /* obsolescent; will be withdrawn */
# define YYLTYPE_IS_DECLARED 1
# define YYLTYPE_IS_TRIVIAL 1
#endif


#ifdef YYPARSE_PARAM
#if defined __STDC__ || defined __cplusplus
int xcp_yyparse (void *YYPARSE_PARAM);
#else
int xcp_yyparse ();
#endif
#else /* ! YYPARSE_PARAM */
#if defined __STDC__ || defined __cplusplus
int xcp_yyparse (xmmsv_collparser_t *collparser, void *scanner);
#else
int xcp_yyparse ();
#endif
#endif /* ! YYPARSE_PARAM */

#endif /* !YY_XCP_YY_PARSER_TAB_H_INCLUDED  */

/* Copy the second part of user declarations.  */

/* Line 390 of yacc.c  */
#line 231 "parser.tab.c"

#ifdef short
# undef short
#endif

#ifdef YYTYPE_UINT8
typedef YYTYPE_UINT8 yytype_uint8;
#else
typedef unsigned char yytype_uint8;
#endif

#ifdef YYTYPE_INT8
typedef YYTYPE_INT8 yytype_int8;
#elif (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
typedef signed char yytype_int8;
#else
typedef short int yytype_int8;
#endif

#ifdef YYTYPE_UINT16
typedef YYTYPE_UINT16 yytype_uint16;
#else
typedef unsigned short int yytype_uint16;
#endif

#ifdef YYTYPE_INT16
typedef YYTYPE_INT16 yytype_int16;
#else
typedef short int yytype_int16;
#endif

#ifndef YYSIZE_T
# ifdef __SIZE_TYPE__
#  define YYSIZE_T __SIZE_TYPE__
# elif defined size_t
#  define YYSIZE_T size_t
# elif ! defined YYSIZE_T && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
#  include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  define YYSIZE_T size_t
# else
#  define YYSIZE_T unsigned int
# endif
#endif

#define YYSIZE_MAXIMUM ((YYSIZE_T) -1)

#ifndef YY_
# if defined YYENABLE_NLS && YYENABLE_NLS
#  if ENABLE_NLS
#   include <libintl.h> /* INFRINGES ON USER NAME SPACE */
#   define YY_(Msgid) dgettext ("bison-runtime", Msgid)
#  endif
# endif
# ifndef YY_
#  define YY_(Msgid) Msgid
# endif
#endif

/* Suppress unused-variable warnings by "using" E.  */
#if ! defined lint || defined __GNUC__
# define YYUSE(E) ((void) (E))
#else
# define YYUSE(E) /* empty */
#endif

/* Identity function, used to suppress warnings about constant conditions.  */
#ifndef lint
# define YYID(N) (N)
#else
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static int
YYID (int yyi)
#else
static int
YYID (yyi)
    int yyi;
#endif
{
  return yyi;
}
#endif

#if ! defined yyoverflow || YYERROR_VERBOSE

/* The parser invokes alloca or malloc; define the necessary symbols.  */

# ifdef YYSTACK_USE_ALLOCA
#  if YYSTACK_USE_ALLOCA
#   ifdef __GNUC__
#    define YYSTACK_ALLOC __builtin_alloca
#   elif defined __BUILTIN_VA_ARG_INCR
#    include <alloca.h> /* INFRINGES ON USER NAME SPACE */
#   elif defined _AIX
#    define YYSTACK_ALLOC __alloca
#   elif defined _MSC_VER
#    include <malloc.h> /* INFRINGES ON USER NAME SPACE */
#    define alloca _alloca
#   else
#    define YYSTACK_ALLOC alloca
#    if ! defined _ALLOCA_H && ! defined EXIT_SUCCESS && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
#     include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
      /* Use EXIT_SUCCESS as a witness for stdlib.h.  */
#     ifndef EXIT_SUCCESS
#      define EXIT_SUCCESS 0
#     endif
#    endif
#   endif
#  endif
# endif

# ifdef YYSTACK_ALLOC
   /* Pacify GCC's `empty if-body' warning.  */
#  define YYSTACK_FREE(Ptr) do { /* empty */; } while (YYID (0))
#  ifndef YYSTACK_ALLOC_MAXIMUM
    /* The OS might guarantee only one guard page at the bottom of the stack,
       and a page size can be as small as 4096 bytes.  So we cannot safely
       invoke alloca (N) if N exceeds 4096.  Use a slightly smaller number
       to allow for a few compiler-allocated temporary stack slots.  */
#   define YYSTACK_ALLOC_MAXIMUM 4032 /* reasonable circa 2006 */
#  endif
# else
#  define YYSTACK_ALLOC YYMALLOC
#  define YYSTACK_FREE YYFREE
#  ifndef YYSTACK_ALLOC_MAXIMUM
#   define YYSTACK_ALLOC_MAXIMUM YYSIZE_MAXIMUM
#  endif
#  if (defined __cplusplus && ! defined EXIT_SUCCESS \
       && ! ((defined YYMALLOC || defined malloc) \
	     && (defined YYFREE || defined free)))
#   include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#   ifndef EXIT_SUCCESS
#    define EXIT_SUCCESS 0
#   endif
#  endif
#  ifndef YYMALLOC
#   define YYMALLOC malloc
#   if ! defined malloc && ! defined EXIT_SUCCESS && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
void *malloc (YYSIZE_T); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
#  ifndef YYFREE
#   define YYFREE free
#   if ! defined free && ! defined EXIT_SUCCESS && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
void free (void *); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
# endif
#endif /* ! defined yyoverflow || YYERROR_VERBOSE */


#if (! defined yyoverflow \
     && (! defined __cplusplus \
	 || (defined YYLTYPE_IS_TRIVIAL && YYLTYPE_IS_TRIVIAL \
	     && defined YYSTYPE_IS_TRIVIAL && YYSTYPE_IS_TRIVIAL)))

/* A type that is properly aligned for any stack member.  */
union yyalloc
{
  yytype_int16 yyss_alloc;
  YYSTYPE yyvs_alloc;
  YYLTYPE yyls_alloc;
};

/* The size of the maximum gap between one aligned stack and the next.  */
# define YYSTACK_GAP_MAXIMUM (sizeof (union yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# define YYSTACK_BYTES(N) \
     ((N) * (sizeof (yytype_int16) + sizeof (YYSTYPE) + sizeof (YYLTYPE)) \
      + 2 * YYSTACK_GAP_MAXIMUM)

# define YYCOPY_NEEDED 1

/* Relocate STACK from its old location to the new one.  The
   local variables YYSIZE and YYSTACKSIZE give the old and new number of
   elements in the stack, and YYPTR gives the new location of the
   stack.  Advance YYPTR to a properly aligned location for the next
   stack.  */
# define YYSTACK_RELOCATE(Stack_alloc, Stack)				\
    do									\
      {									\
	YYSIZE_T yynewbytes;						\
	YYCOPY (&yyptr->Stack_alloc, Stack, yysize);			\
	Stack = &yyptr->Stack_alloc;					\
	yynewbytes = yystacksize * sizeof (*Stack) + YYSTACK_GAP_MAXIMUM; \
	yyptr += yynewbytes / sizeof (*yyptr);				\
      }									\
    while (YYID (0))

#endif

#if defined YYCOPY_NEEDED && YYCOPY_NEEDED
/* Copy COUNT objects from SRC to DST.  The source and destination do
   not overlap.  */
# ifndef YYCOPY
#  if defined __GNUC__ && 1 < __GNUC__
#   define YYCOPY(Dst, Src, Count) \
      __builtin_memcpy (Dst, Src, (Count) * sizeof (*(Src)))
#  else
#   define YYCOPY(Dst, Src, Count)              \
      do                                        \
        {                                       \
          YYSIZE_T yyi;                         \
          for (yyi = 0; yyi < (Count); yyi++)   \
            (Dst)[yyi] = (Src)[yyi];            \
        }                                       \
      while (YYID (0))
#  endif
# endif
#endif /* !YYCOPY_NEEDED */

/* YYFINAL -- State number of the termination state.  */
#define YYFINAL  46
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   211

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  35
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  17
/* YYNRULES -- Number of rules.  */
#define YYNRULES  58
/* YYNRULES -- Number of states.  */
#define YYNSTATES  72

/* YYTRANSLATE(YYLEX) -- Bison symbol number corresponding to YYLEX.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   289

#define YYTRANSLATE(YYX)						\
  ((unsigned int) (YYX) <= YYMAXUTOK ? yytranslate[YYX] : YYUNDEFTOK)

/* YYTRANSLATE[YYLEX] -- Bison symbol number corresponding to YYLEX.  */
static const yytype_uint8 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     1,     2,     3,     4,
       5,     6,     7,     8,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    22,    23,    24,
      25,    26,    27,    28,    29,    30,    31,    32,    33,    34
};

#if YYDEBUG
/* YYPRHS[YYN] -- Index of the first RHS symbol of rule number YYN in
   YYRHS.  */
static const yytype_uint8 yyprhs[] =
{
       0,     0,     3,     5,     8,    10,    12,    16,    18,    22,
      26,    29,    33,    35,    39,    43,    46,    48,    51,    54,
      57,    59,    62,    65,    68,    71,    73,    76,    78,    80,
      84,    86,    90,    92,    96,    98,   101,   104,   106,   108,
     110,   112,   115,   118,   120,   122,   124,   127,   131,   134,
     138,   141,   143,   145,   147,   149,   151,   153,   155
};

/* YYRHS -- A `-1'-separated list of the rules' RHS.  */
static const yytype_int8 yyrhs[] =
{
      36,     0,    -1,     3,    -1,    37,     3,    -1,    37,    -1,
      38,    -1,    38,    28,    39,    -1,    39,    -1,    38,    28,
       5,    -1,    38,    28,     3,    -1,    39,    40,    -1,    39,
      26,    40,    -1,    40,    -1,    39,    26,     5,    -1,    39,
      26,     3,    -1,    33,    42,    -1,    46,    -1,    45,    21,
      -1,    27,    40,    -1,    31,    41,    -1,    34,    -1,    34,
      10,    -1,    45,     3,    -1,    27,     5,    -1,    27,     3,
      -1,    32,    -1,    32,     9,    -1,     7,    -1,    43,    -1,
      42,    25,    43,    -1,    44,    -1,    44,    24,    44,    -1,
       8,    -1,    44,    24,     8,    -1,    23,    -1,    23,     8,
      -1,    22,    37,    -1,    22,    -1,    47,    -1,    50,    -1,
      41,    -1,    48,    49,    -1,    48,     4,    -1,    15,    -1,
      29,    -1,    30,    -1,    29,     6,    -1,    49,    51,    41,
      -1,    51,    41,    -1,    49,    51,     4,    -1,    51,     4,
      -1,    12,    -1,    19,    -1,    18,    -1,    20,    -1,    16,
      -1,    17,    -1,    13,    -1,    14,    -1
};

/* YYRLINE[YYN] -- source line where rule number YYN was defined.  */
static const yytype_uint16 yyrline[] =
{
       0,   123,   123,   147,   165,   182,   189,   224,   228,   246,
     272,   316,   362,   366,   384,   410,   446,   450,   459,   495,
     542,   549,   562,   594,   612,   638,   642,   648,   667,   671,
     685,   702,   719,   736,   756,   760,   767,   771,   778,   782,
     786,   831,   860,   880,   888,   901,   917,   944,   986,  1028,
    1046,  1066,  1071,  1076,  1081,  1086,  1091,  1096,  1101
};
#endif

#if YYDEBUG || YYERROR_VERBOSE || 1
/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "$end", "error", "$undefined", "TOKEN_EOF", "TOKEN_EOF_FILTER",
  "TOKEN_EOF_OPERATOR", "TOKEN_EOF_PROPERTY", "TOKEN_EOF_QUOTE",
  "TOKEN_EOF_SEQUENCE", "TOKEN_EOF_STRING", "TOKEN_EOF_UNIVERSE",
  "TOKEN_ERR", "TOKEN_FILTER_EQUAL", "TOKEN_FILTER_GREATER",
  "TOKEN_FILTER_GREATEREQUAL", "TOKEN_FILTER_HAS", "TOKEN_FILTER_LESS",
  "TOKEN_FILTER_LESSEQUAL", "TOKEN_FILTER_MATCH",
  "TOKEN_FILTER_MATCH_GLOB", "TOKEN_FILTER_TOKEN", "TOKEN_GROUP_CLOSE",
  "TOKEN_GROUP_OPEN", "TOKEN_ID", "TOKEN_ID_RANGE", "TOKEN_ID_SEQUENCE",
  "TOKEN_OPERATOR_AND", "TOKEN_OPERATOR_NOT", "TOKEN_OPERATOR_OR",
  "TOKEN_PROPERTY", "TOKEN_PROPERTY_EOF_QUOTE", "TOKEN_REFERENCE",
  "TOKEN_STRING", "TOKEN_SYMBOL_ID", "TOKEN_UNIVERSE", "$accept", "s",
  "operation", "or_op", "and_op", "expr", "string", "sequence", "seqrange",
  "id", "group", "filter", "unary_filter", "unary_filop", "property",
  "binary_filter", "binary_filop", YY_NULL
};
#endif

# ifdef YYPRINT
/* YYTOKNUM[YYLEX-NUM] -- Internal token number corresponding to
   token YYLEX-NUM.  */
static const yytype_uint16 yytoknum[] =
{
       0,   256,   257,   258,   259,   260,   261,   262,   263,   264,
     265,   266,   267,   268,   269,   270,   271,   272,   273,   274,
     275,   276,   277,   278,   279,   280,   281,   282,   283,   284,
     285,   286,   287,   288,   289
};
# endif

/* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint8 yyr1[] =
{
       0,    35,    36,    36,    36,    37,    38,    38,    38,    38,
      39,    39,    39,    39,    39,    40,    40,    40,    40,    40,
      40,    40,    40,    40,    40,    41,    41,    41,    42,    42,
      43,    43,    43,    43,    44,    44,    45,    45,    46,    46,
      46,    47,    47,    48,    49,    49,    49,    50,    50,    50,
      50,    51,    51,    51,    51,    51,    51,    51,    51
};

/* YYR2[YYN] -- Number of symbols composing right hand side of rule YYN.  */
static const yytype_uint8 yyr2[] =
{
       0,     2,     1,     2,     1,     1,     3,     1,     3,     3,
       2,     3,     1,     3,     3,     2,     1,     2,     2,     2,
       1,     2,     2,     2,     2,     1,     2,     1,     1,     3,
       1,     3,     1,     3,     1,     2,     2,     1,     1,     1,
       1,     2,     2,     1,     1,     1,     2,     3,     2,     3,
       2,     1,     1,     1,     1,     1,     1,     1,     1
};

/* YYDEFACT[STATE-NAME] -- Default reduction number in state STATE-NUM.
   Performed when YYTABLE doesn't specify something else to do.  Zero
   means the default is an error.  */
static const yytype_uint8 yydefact[] =
{
       0,     2,    27,    51,    57,    58,    43,    55,    56,    53,
      52,    54,    37,     0,    44,    45,     0,    25,     0,    20,
       0,     4,     5,     7,    12,    40,     0,    16,    38,     0,
       0,    39,     0,    36,    24,    23,    18,    46,    19,    26,
      32,    34,    15,    28,    30,    21,     1,     3,     0,     0,
      10,    22,    17,    42,    41,     0,    50,    48,    35,     0,
       0,     9,     8,     6,    14,    13,    11,    49,    47,    29,
      33,    31
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int8 yydefgoto[] =
{
      -1,    20,    21,    22,    23,    24,    25,    42,    43,    44,
      26,    27,    28,    29,    30,    31,    32
};

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
#define YYPACT_NINF -22
static const yytype_int16 yypact[] =
{
     112,   -22,   -22,   -22,   -22,   -22,   -22,   -22,   -22,   -22,
     -22,   -22,   168,    15,    -1,   -22,    52,    -7,    -4,    -3,
       6,     5,   -16,   140,   -22,   -22,     0,   -22,   -22,    87,
     191,   -22,     7,   -22,   -22,   -22,   -22,   -22,   -22,   -22,
     -22,    17,   -10,   -22,     2,   -22,   -22,   -22,    48,    80,
     -22,   -22,   -22,   -22,   -22,     9,   -22,   -22,   -22,    -4,
       1,   -22,   -22,   140,   -22,   -22,   -22,   -22,   -22,   -22,
     -22,   -22
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int8 yypgoto[] =
{
     -22,   -22,    11,   -22,    -5,   -13,   -15,   -22,   -21,    -8,
     -22,   -22,   -22,   -22,    25,   -22,    26
};

/* YYTABLE[YYPACT[STATE-NUM]].  What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule which
   number is the opposite.  If YYTABLE_NINF, syntax error.  */
#define YYTABLE_NINF -1
static const yytype_uint8 yytable[] =
{
      36,    38,    39,    51,    40,    37,    46,    45,    47,    70,
      50,    56,    48,    67,     2,    59,     2,    57,    34,    41,
      35,    52,     2,    33,    41,    58,    60,     3,     4,     5,
       6,     7,     8,     9,    10,    11,    66,    12,    69,    17,
      68,    17,    13,    63,    14,    15,    16,    17,    18,    19,
      50,    61,    71,    62,    54,     2,    55,     0,     0,     2,
       3,     4,     5,     6,     7,     8,     9,    10,    11,     0,
      12,     0,     0,     0,     0,    13,     0,    14,    15,    16,
      17,    18,    19,    64,    17,    65,     0,     2,     0,     0,
       0,    53,     3,     4,     5,     6,     7,     8,     9,    10,
      11,     0,    12,     0,     0,     0,     0,    13,     0,    14,
      15,    16,    17,    18,    19,     1,    14,    15,     0,     2,
       0,     0,     0,     0,     3,     4,     5,     6,     7,     8,
       9,    10,    11,     0,    12,     0,     0,     0,     0,    13,
       0,    14,    15,    16,    17,    18,    19,     2,     0,     0,
       0,     0,     3,     4,     5,     6,     7,     8,     9,    10,
      11,     0,    12,     0,     0,     0,    49,    13,     0,    14,
      15,    16,    17,    18,    19,     2,     0,     0,     0,     0,
       3,     4,     5,     6,     7,     8,     9,    10,    11,     0,
      12,     0,     0,     0,     0,    13,     0,    14,    15,    16,
      17,    18,    19,     3,     4,     5,     0,     7,     8,     9,
      10,    11
};

#define yypact_value_is_default(Yystate) \
  (!!((Yystate) == (-22)))

#define yytable_value_is_error(Yytable_value) \
  YYID (0)

static const yytype_int8 yycheck[] =
{
      13,    16,     9,     3,     8,     6,     0,    10,     3,     8,
      23,     4,    28,     4,     7,    25,     7,    32,     3,    23,
       5,    21,     7,    12,    23,     8,    24,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    49,    22,    59,    32,
      55,    32,    27,    48,    29,    30,    31,    32,    33,    34,
      63,     3,    60,     5,    29,     7,    30,    -1,    -1,     7,
      12,    13,    14,    15,    16,    17,    18,    19,    20,    -1,
      22,    -1,    -1,    -1,    -1,    27,    -1,    29,    30,    31,
      32,    33,    34,     3,    32,     5,    -1,     7,    -1,    -1,
      -1,     4,    12,    13,    14,    15,    16,    17,    18,    19,
      20,    -1,    22,    -1,    -1,    -1,    -1,    27,    -1,    29,
      30,    31,    32,    33,    34,     3,    29,    30,    -1,     7,
      -1,    -1,    -1,    -1,    12,    13,    14,    15,    16,    17,
      18,    19,    20,    -1,    22,    -1,    -1,    -1,    -1,    27,
      -1,    29,    30,    31,    32,    33,    34,     7,    -1,    -1,
      -1,    -1,    12,    13,    14,    15,    16,    17,    18,    19,
      20,    -1,    22,    -1,    -1,    -1,    26,    27,    -1,    29,
      30,    31,    32,    33,    34,     7,    -1,    -1,    -1,    -1,
      12,    13,    14,    15,    16,    17,    18,    19,    20,    -1,
      22,    -1,    -1,    -1,    -1,    27,    -1,    29,    30,    31,
      32,    33,    34,    12,    13,    14,    -1,    16,    17,    18,
      19,    20
};

/* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
   symbol of state STATE-NUM.  */
static const yytype_uint8 yystos[] =
{
       0,     3,     7,    12,    13,    14,    15,    16,    17,    18,
      19,    20,    22,    27,    29,    30,    31,    32,    33,    34,
      36,    37,    38,    39,    40,    41,    45,    46,    47,    48,
      49,    50,    51,    37,     3,     5,    40,     6,    41,     9,
       8,    23,    42,    43,    44,    10,     0,     3,    28,    26,
      40,     3,    21,     4,    49,    51,     4,    41,     8,    25,
      24,     3,     5,    39,     3,     5,    40,     4,    41,    43,
       8,    44
};

#define yyerrok		(yyerrstatus = 0)
#define yyclearin	(yychar = YYEMPTY)
#define YYEMPTY		(-2)
#define YYEOF		0

#define YYACCEPT	goto yyacceptlab
#define YYABORT		goto yyabortlab
#define YYERROR		goto yyerrorlab


/* Like YYERROR except do call yyerror.  This remains here temporarily
   to ease the transition to the new meaning of YYERROR, for GCC.
   Once GCC version 2 has supplanted version 1, this can go.  However,
   YYFAIL appears to be in use.  Nevertheless, it is formally deprecated
   in Bison 2.4.2's NEWS entry, where a plan to phase it out is
   discussed.  */

#define YYFAIL		goto yyerrlab
#if defined YYFAIL
  /* This is here to suppress warnings from the GCC cpp's
     -Wunused-macros.  Normally we don't worry about that warning, but
     some users do, and we want to make it easy for users to remove
     YYFAIL uses, which will produce warnings from Bison 2.5.  */
#endif

#define YYRECOVERING()  (!!yyerrstatus)

#define YYBACKUP(Token, Value)                                  \
do                                                              \
  if (yychar == YYEMPTY)                                        \
    {                                                           \
      yychar = (Token);                                         \
      yylval = (Value);                                         \
      YYPOPSTACK (yylen);                                       \
      yystate = *yyssp;                                         \
      goto yybackup;                                            \
    }                                                           \
  else                                                          \
    {                                                           \
      yyerror (&yylloc, collparser, scanner, YY_("syntax error: cannot back up")); \
      YYERROR;							\
    }								\
while (YYID (0))

/* Error token number */
#define YYTERROR	1
#define YYERRCODE	256


/* YYLLOC_DEFAULT -- Set CURRENT to span from RHS[1] to RHS[N].
   If N is 0, then set CURRENT to the empty location which ends
   the previous symbol: RHS[0] (always defined).  */

#ifndef YYLLOC_DEFAULT
# define YYLLOC_DEFAULT(Current, Rhs, N)                                \
    do                                                                  \
      if (YYID (N))                                                     \
        {                                                               \
          (Current).first_line   = YYRHSLOC (Rhs, 1).first_line;        \
          (Current).first_column = YYRHSLOC (Rhs, 1).first_column;      \
          (Current).last_line    = YYRHSLOC (Rhs, N).last_line;         \
          (Current).last_column  = YYRHSLOC (Rhs, N).last_column;       \
        }                                                               \
      else                                                              \
        {                                                               \
          (Current).first_line   = (Current).last_line   =              \
            YYRHSLOC (Rhs, 0).last_line;                                \
          (Current).first_column = (Current).last_column =              \
            YYRHSLOC (Rhs, 0).last_column;                              \
        }                                                               \
    while (YYID (0))
#endif

#define YYRHSLOC(Rhs, K) ((Rhs)[K])


/* YY_LOCATION_PRINT -- Print the location on the stream.
   This macro was not mandated originally: define only if we know
   we won't break user code: when these are the locations we know.  */

#ifndef YY_LOCATION_PRINT
# if defined YYLTYPE_IS_TRIVIAL && YYLTYPE_IS_TRIVIAL

/* Print *YYLOCP on YYO.  Private, do not rely on its existence. */

__attribute__((__unused__))
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static unsigned
yy_location_print_ (FILE *yyo, YYLTYPE const * const yylocp)
#else
static unsigned
yy_location_print_ (yyo, yylocp)
    FILE *yyo;
    YYLTYPE const * const yylocp;
#endif
{
  unsigned res = 0;
  int end_col = 0 != yylocp->last_column ? yylocp->last_column - 1 : 0;
  if (0 <= yylocp->first_line)
    {
      res += fprintf (yyo, "%d", yylocp->first_line);
      if (0 <= yylocp->first_column)
        res += fprintf (yyo, ".%d", yylocp->first_column);
    }
  if (0 <= yylocp->last_line)
    {
      if (yylocp->first_line < yylocp->last_line)
        {
          res += fprintf (yyo, "-%d", yylocp->last_line);
          if (0 <= end_col)
            res += fprintf (yyo, ".%d", end_col);
        }
      else if (0 <= end_col && yylocp->first_column < end_col)
        res += fprintf (yyo, "-%d", end_col);
    }
  return res;
 }

#  define YY_LOCATION_PRINT(File, Loc)          \
  yy_location_print_ (File, &(Loc))

# else
#  define YY_LOCATION_PRINT(File, Loc) ((void) 0)
# endif
#endif


/* YYLEX -- calling `yylex' with the right arguments.  */
#ifdef YYLEX_PARAM
# define YYLEX yylex (&yylval, &yylloc, YYLEX_PARAM)
#else
# define YYLEX yylex (&yylval, &yylloc, scanner)
#endif

/* Enable debugging if requested.  */
#if YYDEBUG

# ifndef YYFPRINTF
#  include <stdio.h> /* INFRINGES ON USER NAME SPACE */
#  define YYFPRINTF fprintf
# endif

# define YYDPRINTF(Args)			\
do {						\
  if (yydebug)					\
    YYFPRINTF Args;				\
} while (YYID (0))

# define YY_SYMBOL_PRINT(Title, Type, Value, Location)			  \
do {									  \
  if (yydebug)								  \
    {									  \
      YYFPRINTF (stderr, "%s ", Title);					  \
      yy_symbol_print (stderr,						  \
		  Type, Value, Location, collparser, scanner); \
      YYFPRINTF (stderr, "\n");						  \
    }									  \
} while (YYID (0))


/*--------------------------------.
| Print this symbol on YYOUTPUT.  |
`--------------------------------*/

/*ARGSUSED*/
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_symbol_value_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep, YYLTYPE const * const yylocationp, xmmsv_collparser_t *collparser, void *scanner)
#else
static void
yy_symbol_value_print (yyoutput, yytype, yyvaluep, yylocationp, collparser, scanner)
    FILE *yyoutput;
    int yytype;
    YYSTYPE const * const yyvaluep;
    YYLTYPE const * const yylocationp;
    xmmsv_collparser_t *collparser;
    void *scanner;
#endif
{
  FILE *yyo = yyoutput;
  YYUSE (yyo);
  if (!yyvaluep)
    return;
  YYUSE (yylocationp);
  YYUSE (collparser);
  YYUSE (scanner);
# ifdef YYPRINT
  if (yytype < YYNTOKENS)
    YYPRINT (yyoutput, yytoknum[yytype], *yyvaluep);
# else
  YYUSE (yyoutput);
# endif
  switch (yytype)
    {
      default:
        break;
    }
}


/*--------------------------------.
| Print this symbol on YYOUTPUT.  |
`--------------------------------*/

#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_symbol_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep, YYLTYPE const * const yylocationp, xmmsv_collparser_t *collparser, void *scanner)
#else
static void
yy_symbol_print (yyoutput, yytype, yyvaluep, yylocationp, collparser, scanner)
    FILE *yyoutput;
    int yytype;
    YYSTYPE const * const yyvaluep;
    YYLTYPE const * const yylocationp;
    xmmsv_collparser_t *collparser;
    void *scanner;
#endif
{
  if (yytype < YYNTOKENS)
    YYFPRINTF (yyoutput, "token %s (", yytname[yytype]);
  else
    YYFPRINTF (yyoutput, "nterm %s (", yytname[yytype]);

  YY_LOCATION_PRINT (yyoutput, *yylocationp);
  YYFPRINTF (yyoutput, ": ");
  yy_symbol_value_print (yyoutput, yytype, yyvaluep, yylocationp, collparser, scanner);
  YYFPRINTF (yyoutput, ")");
}

/*------------------------------------------------------------------.
| yy_stack_print -- Print the state stack from its BOTTOM up to its |
| TOP (included).                                                   |
`------------------------------------------------------------------*/

#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_stack_print (yytype_int16 *yybottom, yytype_int16 *yytop)
#else
static void
yy_stack_print (yybottom, yytop)
    yytype_int16 *yybottom;
    yytype_int16 *yytop;
#endif
{
  YYFPRINTF (stderr, "Stack now");
  for (; yybottom <= yytop; yybottom++)
    {
      int yybot = *yybottom;
      YYFPRINTF (stderr, " %d", yybot);
    }
  YYFPRINTF (stderr, "\n");
}

# define YY_STACK_PRINT(Bottom, Top)				\
do {								\
  if (yydebug)							\
    yy_stack_print ((Bottom), (Top));				\
} while (YYID (0))


/*------------------------------------------------.
| Report that the YYRULE is going to be reduced.  |
`------------------------------------------------*/

#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_reduce_print (YYSTYPE *yyvsp, YYLTYPE *yylsp, int yyrule, xmmsv_collparser_t *collparser, void *scanner)
#else
static void
yy_reduce_print (yyvsp, yylsp, yyrule, collparser, scanner)
    YYSTYPE *yyvsp;
    YYLTYPE *yylsp;
    int yyrule;
    xmmsv_collparser_t *collparser;
    void *scanner;
#endif
{
  int yynrhs = yyr2[yyrule];
  int yyi;
  unsigned long int yylno = yyrline[yyrule];
  YYFPRINTF (stderr, "Reducing stack by rule %d (line %lu):\n",
	     yyrule - 1, yylno);
  /* The symbols being reduced.  */
  for (yyi = 0; yyi < yynrhs; yyi++)
    {
      YYFPRINTF (stderr, "   $%d = ", yyi + 1);
      yy_symbol_print (stderr, yyrhs[yyprhs[yyrule] + yyi],
		       &(yyvsp[(yyi + 1) - (yynrhs)])
		       , &(yylsp[(yyi + 1) - (yynrhs)])		       , collparser, scanner);
      YYFPRINTF (stderr, "\n");
    }
}

# define YY_REDUCE_PRINT(Rule)		\
do {					\
  if (yydebug)				\
    yy_reduce_print (yyvsp, yylsp, Rule, collparser, scanner); \
} while (YYID (0))

/* Nonzero means print parse trace.  It is left uninitialized so that
   multiple parsers can coexist.  */
int yydebug;
#else /* !YYDEBUG */
# define YYDPRINTF(Args)
# define YY_SYMBOL_PRINT(Title, Type, Value, Location)
# define YY_STACK_PRINT(Bottom, Top)
# define YY_REDUCE_PRINT(Rule)
#endif /* !YYDEBUG */


/* YYINITDEPTH -- initial size of the parser's stacks.  */
#ifndef	YYINITDEPTH
# define YYINITDEPTH 200
#endif

/* YYMAXDEPTH -- maximum size the stacks can grow to (effective only
   if the built-in stack extension method is used).

   Do not make this value too large; the results are undefined if
   YYSTACK_ALLOC_MAXIMUM < YYSTACK_BYTES (YYMAXDEPTH)
   evaluated with infinite-precision integer arithmetic.  */

#ifndef YYMAXDEPTH
# define YYMAXDEPTH 10000
#endif


#if YYERROR_VERBOSE

# ifndef yystrlen
#  if defined __GLIBC__ && defined _STRING_H
#   define yystrlen strlen
#  else
/* Return the length of YYSTR.  */
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static YYSIZE_T
yystrlen (const char *yystr)
#else
static YYSIZE_T
yystrlen (yystr)
    const char *yystr;
#endif
{
  YYSIZE_T yylen;
  for (yylen = 0; yystr[yylen]; yylen++)
    continue;
  return yylen;
}
#  endif
# endif

# ifndef yystpcpy
#  if defined __GLIBC__ && defined _STRING_H && defined _GNU_SOURCE
#   define yystpcpy stpcpy
#  else
/* Copy YYSRC to YYDEST, returning the address of the terminating '\0' in
   YYDEST.  */
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static char *
yystpcpy (char *yydest, const char *yysrc)
#else
static char *
yystpcpy (yydest, yysrc)
    char *yydest;
    const char *yysrc;
#endif
{
  char *yyd = yydest;
  const char *yys = yysrc;

  while ((*yyd++ = *yys++) != '\0')
    continue;

  return yyd - 1;
}
#  endif
# endif

# ifndef yytnamerr
/* Copy to YYRES the contents of YYSTR after stripping away unnecessary
   quotes and backslashes, so that it's suitable for yyerror.  The
   heuristic is that double-quoting is unnecessary unless the string
   contains an apostrophe, a comma, or backslash (other than
   backslash-backslash).  YYSTR is taken from yytname.  If YYRES is
   null, do not copy; instead, return the length of what the result
   would have been.  */
static YYSIZE_T
yytnamerr (char *yyres, const char *yystr)
{
  if (*yystr == '"')
    {
      YYSIZE_T yyn = 0;
      char const *yyp = yystr;

      for (;;)
	switch (*++yyp)
	  {
	  case '\'':
	  case ',':
	    goto do_not_strip_quotes;

	  case '\\':
	    if (*++yyp != '\\')
	      goto do_not_strip_quotes;
	    /* Fall through.  */
	  default:
	    if (yyres)
	      yyres[yyn] = *yyp;
	    yyn++;
	    break;

	  case '"':
	    if (yyres)
	      yyres[yyn] = '\0';
	    return yyn;
	  }
    do_not_strip_quotes: ;
    }

  if (! yyres)
    return yystrlen (yystr);

  return yystpcpy (yyres, yystr) - yyres;
}
# endif

/* Copy into *YYMSG, which is of size *YYMSG_ALLOC, an error message
   about the unexpected token YYTOKEN for the state stack whose top is
   YYSSP.

   Return 0 if *YYMSG was successfully written.  Return 1 if *YYMSG is
   not large enough to hold the message.  In that case, also set
   *YYMSG_ALLOC to the required number of bytes.  Return 2 if the
   required number of bytes is too large to store.  */
static int
yysyntax_error (YYSIZE_T *yymsg_alloc, char **yymsg,
                yytype_int16 *yyssp, int yytoken)
{
  YYSIZE_T yysize0 = yytnamerr (YY_NULL, yytname[yytoken]);
  YYSIZE_T yysize = yysize0;
  enum { YYERROR_VERBOSE_ARGS_MAXIMUM = 5 };
  /* Internationalized format string. */
  const char *yyformat = YY_NULL;
  /* Arguments of yyformat. */
  char const *yyarg[YYERROR_VERBOSE_ARGS_MAXIMUM];
  /* Number of reported tokens (one for the "unexpected", one per
     "expected"). */
  int yycount = 0;

  /* There are many possibilities here to consider:
     - Assume YYFAIL is not used.  It's too flawed to consider.  See
       <http://lists.gnu.org/archive/html/bison-patches/2009-12/msg00024.html>
       for details.  YYERROR is fine as it does not invoke this
       function.
     - If this state is a consistent state with a default action, then
       the only way this function was invoked is if the default action
       is an error action.  In that case, don't check for expected
       tokens because there are none.
     - The only way there can be no lookahead present (in yychar) is if
       this state is a consistent state with a default action.  Thus,
       detecting the absence of a lookahead is sufficient to determine
       that there is no unexpected or expected token to report.  In that
       case, just report a simple "syntax error".
     - Don't assume there isn't a lookahead just because this state is a
       consistent state with a default action.  There might have been a
       previous inconsistent state, consistent state with a non-default
       action, or user semantic action that manipulated yychar.
     - Of course, the expected token list depends on states to have
       correct lookahead information, and it depends on the parser not
       to perform extra reductions after fetching a lookahead from the
       scanner and before detecting a syntax error.  Thus, state merging
       (from LALR or IELR) and default reductions corrupt the expected
       token list.  However, the list is correct for canonical LR with
       one exception: it will still contain any token that will not be
       accepted due to an error action in a later state.
  */
  if (yytoken != YYEMPTY)
    {
      int yyn = yypact[*yyssp];
      yyarg[yycount++] = yytname[yytoken];
      if (!yypact_value_is_default (yyn))
        {
          /* Start YYX at -YYN if negative to avoid negative indexes in
             YYCHECK.  In other words, skip the first -YYN actions for
             this state because they are default actions.  */
          int yyxbegin = yyn < 0 ? -yyn : 0;
          /* Stay within bounds of both yycheck and yytname.  */
          int yychecklim = YYLAST - yyn + 1;
          int yyxend = yychecklim < YYNTOKENS ? yychecklim : YYNTOKENS;
          int yyx;

          for (yyx = yyxbegin; yyx < yyxend; ++yyx)
            if (yycheck[yyx + yyn] == yyx && yyx != YYTERROR
                && !yytable_value_is_error (yytable[yyx + yyn]))
              {
                if (yycount == YYERROR_VERBOSE_ARGS_MAXIMUM)
                  {
                    yycount = 1;
                    yysize = yysize0;
                    break;
                  }
                yyarg[yycount++] = yytname[yyx];
                {
                  YYSIZE_T yysize1 = yysize + yytnamerr (YY_NULL, yytname[yyx]);
                  if (! (yysize <= yysize1
                         && yysize1 <= YYSTACK_ALLOC_MAXIMUM))
                    return 2;
                  yysize = yysize1;
                }
              }
        }
    }

  switch (yycount)
    {
# define YYCASE_(N, S)                      \
      case N:                               \
        yyformat = S;                       \
      break
      YYCASE_(0, YY_("syntax error"));
      YYCASE_(1, YY_("syntax error, unexpected %s"));
      YYCASE_(2, YY_("syntax error, unexpected %s, expecting %s"));
      YYCASE_(3, YY_("syntax error, unexpected %s, expecting %s or %s"));
      YYCASE_(4, YY_("syntax error, unexpected %s, expecting %s or %s or %s"));
      YYCASE_(5, YY_("syntax error, unexpected %s, expecting %s or %s or %s or %s"));
# undef YYCASE_
    }

  {
    YYSIZE_T yysize1 = yysize + yystrlen (yyformat);
    if (! (yysize <= yysize1 && yysize1 <= YYSTACK_ALLOC_MAXIMUM))
      return 2;
    yysize = yysize1;
  }

  if (*yymsg_alloc < yysize)
    {
      *yymsg_alloc = 2 * yysize;
      if (! (yysize <= *yymsg_alloc
             && *yymsg_alloc <= YYSTACK_ALLOC_MAXIMUM))
        *yymsg_alloc = YYSTACK_ALLOC_MAXIMUM;
      return 1;
    }

  /* Avoid sprintf, as that infringes on the user's name space.
     Don't have undefined behavior even if the translation
     produced a string with the wrong number of "%s"s.  */
  {
    char *yyp = *yymsg;
    int yyi = 0;
    while ((*yyp = *yyformat) != '\0')
      if (*yyp == '%' && yyformat[1] == 's' && yyi < yycount)
        {
          yyp += yytnamerr (yyp, yyarg[yyi++]);
          yyformat += 2;
        }
      else
        {
          yyp++;
          yyformat++;
        }
  }
  return 0;
}
#endif /* YYERROR_VERBOSE */

/*-----------------------------------------------.
| Release the memory associated to this symbol.  |
`-----------------------------------------------*/

/*ARGSUSED*/
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yydestruct (const char *yymsg, int yytype, YYSTYPE *yyvaluep, YYLTYPE *yylocationp, xmmsv_collparser_t *collparser, void *scanner)
#else
static void
yydestruct (yymsg, yytype, yyvaluep, yylocationp, collparser, scanner)
    const char *yymsg;
    int yytype;
    YYSTYPE *yyvaluep;
    YYLTYPE *yylocationp;
    xmmsv_collparser_t *collparser;
    void *scanner;
#endif
{
  YYUSE (yyvaluep);
  YYUSE (yylocationp);
  YYUSE (collparser);
  YYUSE (scanner);

  if (!yymsg)
    yymsg = "Deleting";
  YY_SYMBOL_PRINT (yymsg, yytype, yyvaluep, yylocationp);

  switch (yytype)
    {
      case 7: /* TOKEN_EOF_QUOTE */
/* Line 1398 of yacc.c  */
#line 117 "/home/vdust/devel/xmms2/xmms2-all/src/clients/lib/xmmsclient/collparser/parser.y"
        { free (((*yyvaluep).str)); };
/* Line 1398 of yacc.c  */
#line 1336 "parser.tab.c"
        break;
      case 29: /* TOKEN_PROPERTY */
/* Line 1398 of yacc.c  */
#line 117 "/home/vdust/devel/xmms2/xmms2-all/src/clients/lib/xmmsclient/collparser/parser.y"
        { free (((*yyvaluep).str)); };
/* Line 1398 of yacc.c  */
#line 1343 "parser.tab.c"
        break;
      case 30: /* TOKEN_PROPERTY_EOF_QUOTE */
/* Line 1398 of yacc.c  */
#line 117 "/home/vdust/devel/xmms2/xmms2-all/src/clients/lib/xmmsclient/collparser/parser.y"
        { free (((*yyvaluep).str)); };
/* Line 1398 of yacc.c  */
#line 1350 "parser.tab.c"
        break;
      case 32: /* TOKEN_STRING */
/* Line 1398 of yacc.c  */
#line 117 "/home/vdust/devel/xmms2/xmms2-all/src/clients/lib/xmmsclient/collparser/parser.y"
        { free (((*yyvaluep).str)); };
/* Line 1398 of yacc.c  */
#line 1357 "parser.tab.c"
        break;
      case 37: /* operation */
/* Line 1398 of yacc.c  */
#line 118 "/home/vdust/devel/xmms2/xmms2-all/src/clients/lib/xmmsclient/collparser/parser.y"
        { xcp_data_free (((*yyvaluep).data)); };
/* Line 1398 of yacc.c  */
#line 1364 "parser.tab.c"
        break;
      case 38: /* or_op */
/* Line 1398 of yacc.c  */
#line 118 "/home/vdust/devel/xmms2/xmms2-all/src/clients/lib/xmmsclient/collparser/parser.y"
        { xcp_data_free (((*yyvaluep).data)); };
/* Line 1398 of yacc.c  */
#line 1371 "parser.tab.c"
        break;
      case 39: /* and_op */
/* Line 1398 of yacc.c  */
#line 118 "/home/vdust/devel/xmms2/xmms2-all/src/clients/lib/xmmsclient/collparser/parser.y"
        { xcp_data_free (((*yyvaluep).data)); };
/* Line 1398 of yacc.c  */
#line 1378 "parser.tab.c"
        break;
      case 40: /* expr */
/* Line 1398 of yacc.c  */
#line 118 "/home/vdust/devel/xmms2/xmms2-all/src/clients/lib/xmmsclient/collparser/parser.y"
        { xcp_data_free (((*yyvaluep).data)); };
/* Line 1398 of yacc.c  */
#line 1385 "parser.tab.c"
        break;
      case 41: /* string */
/* Line 1398 of yacc.c  */
#line 118 "/home/vdust/devel/xmms2/xmms2-all/src/clients/lib/xmmsclient/collparser/parser.y"
        { xcp_data_free (((*yyvaluep).data)); };
/* Line 1398 of yacc.c  */
#line 1392 "parser.tab.c"
        break;
      case 42: /* sequence */
/* Line 1398 of yacc.c  */
#line 118 "/home/vdust/devel/xmms2/xmms2-all/src/clients/lib/xmmsclient/collparser/parser.y"
        { xcp_data_free (((*yyvaluep).data)); };
/* Line 1398 of yacc.c  */
#line 1399 "parser.tab.c"
        break;
      case 43: /* seqrange */
/* Line 1398 of yacc.c  */
#line 118 "/home/vdust/devel/xmms2/xmms2-all/src/clients/lib/xmmsclient/collparser/parser.y"
        { xcp_data_free (((*yyvaluep).data)); };
/* Line 1398 of yacc.c  */
#line 1406 "parser.tab.c"
        break;
      case 45: /* group */
/* Line 1398 of yacc.c  */
#line 118 "/home/vdust/devel/xmms2/xmms2-all/src/clients/lib/xmmsclient/collparser/parser.y"
        { xcp_data_free (((*yyvaluep).data)); };
/* Line 1398 of yacc.c  */
#line 1413 "parser.tab.c"
        break;
      case 46: /* filter */
/* Line 1398 of yacc.c  */
#line 118 "/home/vdust/devel/xmms2/xmms2-all/src/clients/lib/xmmsclient/collparser/parser.y"
        { xcp_data_free (((*yyvaluep).data)); };
/* Line 1398 of yacc.c  */
#line 1420 "parser.tab.c"
        break;
      case 47: /* unary_filter */
/* Line 1398 of yacc.c  */
#line 118 "/home/vdust/devel/xmms2/xmms2-all/src/clients/lib/xmmsclient/collparser/parser.y"
        { xcp_data_free (((*yyvaluep).data)); };
/* Line 1398 of yacc.c  */
#line 1427 "parser.tab.c"
        break;
      case 49: /* property */
/* Line 1398 of yacc.c  */
#line 118 "/home/vdust/devel/xmms2/xmms2-all/src/clients/lib/xmmsclient/collparser/parser.y"
        { xcp_data_free (((*yyvaluep).data)); };
/* Line 1398 of yacc.c  */
#line 1434 "parser.tab.c"
        break;
      case 50: /* binary_filter */
/* Line 1398 of yacc.c  */
#line 118 "/home/vdust/devel/xmms2/xmms2-all/src/clients/lib/xmmsclient/collparser/parser.y"
        { xcp_data_free (((*yyvaluep).data)); };
/* Line 1398 of yacc.c  */
#line 1441 "parser.tab.c"
        break;

      default:
        break;
    }
}




/*----------.
| yyparse.  |
`----------*/

#ifdef YYPARSE_PARAM
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
int
yyparse (void *YYPARSE_PARAM)
#else
int
yyparse (YYPARSE_PARAM)
    void *YYPARSE_PARAM;
#endif
#else /* ! YYPARSE_PARAM */
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
int
yyparse (xmmsv_collparser_t *collparser, void *scanner)
#else
int
yyparse (collparser, scanner)
    xmmsv_collparser_t *collparser;
    void *scanner;
#endif
#endif
{
/* The lookahead symbol.  */
int yychar;


#if defined __GNUC__ && 407 <= __GNUC__ * 100 + __GNUC_MINOR__
/* Suppress an incorrect diagnostic about yylval being uninitialized.  */
# define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN \
    _Pragma ("GCC diagnostic push") \
    _Pragma ("GCC diagnostic ignored \"-Wuninitialized\"")\
    _Pragma ("GCC diagnostic ignored \"-Wmaybe-uninitialized\"")
# define YY_IGNORE_MAYBE_UNINITIALIZED_END \
    _Pragma ("GCC diagnostic pop")
#else
/* Default value used for initialization, for pacifying older GCCs
   or non-GCC compilers.  */
static YYSTYPE yyval_default;
# define YY_INITIAL_VALUE(Value) = Value
#endif
static YYLTYPE yyloc_default
# if defined YYLTYPE_IS_TRIVIAL && YYLTYPE_IS_TRIVIAL
  = { 1, 1, 1, 1 }
# endif
;
#ifndef YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_END
#endif
#ifndef YY_INITIAL_VALUE
# define YY_INITIAL_VALUE(Value) /* Nothing. */
#endif

/* The semantic value of the lookahead symbol.  */
YYSTYPE yylval YY_INITIAL_VALUE(yyval_default);

/* Location data for the lookahead symbol.  */
YYLTYPE yylloc = yyloc_default;


    /* Number of syntax errors so far.  */
    int yynerrs;

    int yystate;
    /* Number of tokens to shift before error messages enabled.  */
    int yyerrstatus;

    /* The stacks and their tools:
       `yyss': related to states.
       `yyvs': related to semantic values.
       `yyls': related to locations.

       Refer to the stacks through separate pointers, to allow yyoverflow
       to reallocate them elsewhere.  */

    /* The state stack.  */
    yytype_int16 yyssa[YYINITDEPTH];
    yytype_int16 *yyss;
    yytype_int16 *yyssp;

    /* The semantic value stack.  */
    YYSTYPE yyvsa[YYINITDEPTH];
    YYSTYPE *yyvs;
    YYSTYPE *yyvsp;

    /* The location stack.  */
    YYLTYPE yylsa[YYINITDEPTH];
    YYLTYPE *yyls;
    YYLTYPE *yylsp;

    /* The locations where the error started and ended.  */
    YYLTYPE yyerror_range[3];

    YYSIZE_T yystacksize;

  int yyn;
  int yyresult;
  /* Lookahead token as an internal (translated) token number.  */
  int yytoken = 0;
  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE yyval;
  YYLTYPE yyloc;

#if YYERROR_VERBOSE
  /* Buffer for error messages, and its allocated size.  */
  char yymsgbuf[128];
  char *yymsg = yymsgbuf;
  YYSIZE_T yymsg_alloc = sizeof yymsgbuf;
#endif

#define YYPOPSTACK(N)   (yyvsp -= (N), yyssp -= (N), yylsp -= (N))

  /* The number of symbols on the RHS of the reduced rule.
     Keep to zero when no symbol should be popped.  */
  int yylen = 0;

  yyssp = yyss = yyssa;
  yyvsp = yyvs = yyvsa;
  yylsp = yyls = yylsa;
  yystacksize = YYINITDEPTH;

  YYDPRINTF ((stderr, "Starting parse\n"));

  yystate = 0;
  yyerrstatus = 0;
  yynerrs = 0;
  yychar = YYEMPTY; /* Cause a token to be read.  */
  yylsp[0] = yylloc;
  goto yysetstate;

/*------------------------------------------------------------.
| yynewstate -- Push a new state, which is found in yystate.  |
`------------------------------------------------------------*/
 yynewstate:
  /* In all cases, when you get here, the value and location stacks
     have just been pushed.  So pushing a state here evens the stacks.  */
  yyssp++;

 yysetstate:
  *yyssp = yystate;

  if (yyss + yystacksize - 1 <= yyssp)
    {
      /* Get the current used size of the three stacks, in elements.  */
      YYSIZE_T yysize = yyssp - yyss + 1;

#ifdef yyoverflow
      {
	/* Give user a chance to reallocate the stack.  Use copies of
	   these so that the &'s don't force the real ones into
	   memory.  */
	YYSTYPE *yyvs1 = yyvs;
	yytype_int16 *yyss1 = yyss;
	YYLTYPE *yyls1 = yyls;

	/* Each stack pointer address is followed by the size of the
	   data in use in that stack, in bytes.  This used to be a
	   conditional around just the two extra args, but that might
	   be undefined if yyoverflow is a macro.  */
	yyoverflow (YY_("memory exhausted"),
		    &yyss1, yysize * sizeof (*yyssp),
		    &yyvs1, yysize * sizeof (*yyvsp),
		    &yyls1, yysize * sizeof (*yylsp),
		    &yystacksize);

	yyls = yyls1;
	yyss = yyss1;
	yyvs = yyvs1;
      }
#else /* no yyoverflow */
# ifndef YYSTACK_RELOCATE
      goto yyexhaustedlab;
# else
      /* Extend the stack our own way.  */
      if (YYMAXDEPTH <= yystacksize)
	goto yyexhaustedlab;
      yystacksize *= 2;
      if (YYMAXDEPTH < yystacksize)
	yystacksize = YYMAXDEPTH;

      {
	yytype_int16 *yyss1 = yyss;
	union yyalloc *yyptr =
	  (union yyalloc *) YYSTACK_ALLOC (YYSTACK_BYTES (yystacksize));
	if (! yyptr)
	  goto yyexhaustedlab;
	YYSTACK_RELOCATE (yyss_alloc, yyss);
	YYSTACK_RELOCATE (yyvs_alloc, yyvs);
	YYSTACK_RELOCATE (yyls_alloc, yyls);
#  undef YYSTACK_RELOCATE
	if (yyss1 != yyssa)
	  YYSTACK_FREE (yyss1);
      }
# endif
#endif /* no yyoverflow */

      yyssp = yyss + yysize - 1;
      yyvsp = yyvs + yysize - 1;
      yylsp = yyls + yysize - 1;

      YYDPRINTF ((stderr, "Stack size increased to %lu\n",
		  (unsigned long int) yystacksize));

      if (yyss + yystacksize - 1 <= yyssp)
	YYABORT;
    }

  YYDPRINTF ((stderr, "Entering state %d\n", yystate));

  if (yystate == YYFINAL)
    YYACCEPT;

  goto yybackup;

/*-----------.
| yybackup.  |
`-----------*/
yybackup:

  /* Do appropriate processing given the current state.  Read a
     lookahead token if we need one and don't already have one.  */

  /* First try to decide what to do without reference to lookahead token.  */
  yyn = yypact[yystate];
  if (yypact_value_is_default (yyn))
    goto yydefault;

  /* Not known => get a lookahead token if don't already have one.  */

  /* YYCHAR is either YYEMPTY or YYEOF or a valid lookahead symbol.  */
  if (yychar == YYEMPTY)
    {
      YYDPRINTF ((stderr, "Reading a token: "));
      yychar = YYLEX;
    }

  if (yychar <= YYEOF)
    {
      yychar = yytoken = YYEOF;
      YYDPRINTF ((stderr, "Now at end of input.\n"));
    }
  else
    {
      yytoken = YYTRANSLATE (yychar);
      YY_SYMBOL_PRINT ("Next token is", yytoken, &yylval, &yylloc);
    }

  /* If the proper action on seeing token YYTOKEN is to reduce or to
     detect an error, take that action.  */
  yyn += yytoken;
  if (yyn < 0 || YYLAST < yyn || yycheck[yyn] != yytoken)
    goto yydefault;
  yyn = yytable[yyn];
  if (yyn <= 0)
    {
      if (yytable_value_is_error (yyn))
        goto yyerrlab;
      yyn = -yyn;
      goto yyreduce;
    }

  /* Count tokens shifted since error; after three, turn off error
     status.  */
  if (yyerrstatus)
    yyerrstatus--;

  /* Shift the lookahead token.  */
  YY_SYMBOL_PRINT ("Shifting", yytoken, &yylval, &yylloc);

  /* Discard the shifted token.  */
  yychar = YYEMPTY;

  yystate = yyn;
  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END
  *++yylsp = yylloc;
  goto yynewstate;


/*-----------------------------------------------------------.
| yydefault -- do the default action for the current state.  |
`-----------------------------------------------------------*/
yydefault:
  yyn = yydefact[yystate];
  if (yyn == 0)
    goto yyerrlab;
  goto yyreduce;


/*-----------------------------.
| yyreduce -- Do a reduction.  |
`-----------------------------*/
yyreduce:
  /* yyn is the number of a rule to reduce with.  */
  yylen = yyr2[yyn];

  /* If YYLEN is nonzero, implement the default value of the action:
     `$$ = $1'.

     Otherwise, the following line sets YYVAL to garbage.
     This behavior is undocumented and Bison
     users should not rely upon it.  Assigning to YYVAL
     unconditionally makes the parser a bit smaller, and it avoids a
     GCC warning that YYVAL may be used uninitialized.  */
  yyval = yyvsp[1-yylen];

  /* Default location.  */
  YYLLOC_DEFAULT (yyloc, (yylsp - yylen), yylen);
  YY_REDUCE_PRINT (yyn);
  switch (yyn)
    {
        case 2:
/* Line 1792 of yacc.c  */
#line 123 "/home/vdust/devel/xmms2/xmms2-all/src/clients/lib/xmmsclient/collparser/parser.y"
    {
		PRINT_R ("TOKEN_EOF", "s");
		switch (collparser->mode) {
		case XMMS_COLLPARSER_MODE_COMPLETE:
			if (1) {
				xcp_data_t *d = xcp_data_new_eof ();
				xcp_data_set_completion (d,
				                         XMMS_COLLPARSER_COMPLETE_UNARY_OPERATOR |
				                         XMMS_COLLPARSER_COMPLETE_UNIVERSE |
				                         XMMS_COLLPARSER_COMPLETE_FIELD |
				                         XMMS_COLLPARSER_COMPLETE_FILTER |
				                         XMMS_COLLPARSER_COMPLETE_VALUE |
				                         XMMS_COLLPARSER_COMPLETE_SEQUENCE |
				                         XMMS_COLLPARSER_COMPLETE_OPEN_GROUP);
				collparser->result = xcp_data_build_result (d, collparser->mode);
				xcp_data_free (d);
			}
			break;
		case XMMS_COLLPARSER_MODE_COMPILE:
			xcp_yyerror (&(yylsp[(1) - (1)]), collparser, scanner, XCP_ERROR_PATTERN_EMPTY);
		default:
			YYERROR;
		}
	}
    break;

  case 3:
/* Line 1792 of yacc.c  */
#line 147 "/home/vdust/devel/xmms2/xmms2-all/src/clients/lib/xmmsclient/collparser/parser.y"
    {
		PRINT_R ("operation TOKEN_EOF", "s");
		if (collparser->mode == XMMS_COLLPARSER_MODE_COMPLETE) {
			if (!HAS_COMPLETION ((yyvsp[(1) - (2)].data))) {
				xcp_data_set_completion ((yyvsp[(1) - (2)].data),
				                         XMMS_COLLPARSER_COMPLETE_UNARY_OPERATOR |
				                         XMMS_COLLPARSER_COMPLETE_BINARY_OPERATOR |
				                         XMMS_COLLPARSER_COMPLETE_UNIVERSE |
				                         XMMS_COLLPARSER_COMPLETE_FIELD |
				                         XMMS_COLLPARSER_COMPLETE_FILTER |
				                         XMMS_COLLPARSER_COMPLETE_VALUE |
				                         XMMS_COLLPARSER_COMPLETE_SEQUENCE |
				                         XMMS_COLLPARSER_COMPLETE_OPEN_GROUP);
			}
		}
		collparser->result = xcp_data_build_result ((yyvsp[(1) - (2)].data), collparser->mode);
		xcp_data_free ((yyvsp[(1) - (2)].data));
	}
    break;

  case 4:
/* Line 1792 of yacc.c  */
#line 165 "/home/vdust/devel/xmms2/xmms2-all/src/clients/lib/xmmsclient/collparser/parser.y"
    { /* Can happen only when an expression is incomplete. */
		PRINT_R ("operation", "s");
		switch (collparser->mode) {
		case XMMS_COLLPARSER_MODE_COMPLETE:
			collparser->result = xcp_data_build_result ((yyvsp[(1) - (1)].data), collparser->mode);
			break;
		case XMMS_COLLPARSER_MODE_COMPILE:
			collparser->result = xmmsv_ref ((yyvsp[(1) - (1)].data)->error);
			xcp_yyerror (&(yylsp[(1) - (1)]), collparser, scanner, XCP_ERROR_INCOMPLETE);
		default:
			YYERROR;
		}
		xcp_data_free ((yyvsp[(1) - (1)].data));
	}
    break;

  case 5:
/* Line 1792 of yacc.c  */
#line 182 "/home/vdust/devel/xmms2/xmms2-all/src/clients/lib/xmmsclient/collparser/parser.y"
    {
		PRINT_R ("or_op", "operation");
		(yyval.data) = (yyvsp[(1) - (1)].data);
	}
    break;

  case 6:
/* Line 1792 of yacc.c  */
#line 189 "/home/vdust/devel/xmms2/xmms2-all/src/clients/lib/xmmsclient/collparser/parser.y"
    {
		PRINT_R ("or_op TOKEN_OPERATOR_OR and_op", "or_op");
		switch (collparser->mode) {
		case XMMS_COLLPARSER_MODE_COMPLETE:
			if (HAS_EOF ((yyvsp[(3) - (3)].data)) || !APPLY_OPERATOR ((yyvsp[(3) - (3)].data))) {
				(yyval.data) = (yyvsp[(3) - (3)].data);
			} else {
				xmmsv_coll_t *c;
				c = xcp_build_union ((yyvsp[(1) - (3)].data)->collection, (yyvsp[(3) - (3)].data)->collection);
				if (c) {
					(yyval.data) = xcp_data_new_coll (c);
					xmmsv_unref (c);
				} else {
					(yyval.data) = xcp_data_new ();
				}
				xcp_data_free ((yyvsp[(3) - (3)].data));
			}
			break;
		case XMMS_COLLPARSER_MODE_COMPILE:
			if (HAS_COLLECTION ((yyvsp[(3) - (3)].data))) {
				xmmsv_coll_t *c;
				c = xcp_build_union ((yyvsp[(1) - (3)].data)->collection, (yyvsp[(3) - (3)].data)->collection);
				if (c) {
					(yyval.data) = xcp_data_new_coll (c);
					xmmsv_unref (c);
					xcp_data_free ((yyvsp[(3) - (3)].data));
					break;
				}
			}
			/* Should never reached this point.*/
		default:
			YYERROR;
		}
		xcp_data_free ((yyvsp[(1) - (3)].data));
	}
    break;

  case 7:
/* Line 1792 of yacc.c  */
#line 224 "/home/vdust/devel/xmms2/xmms2-all/src/clients/lib/xmmsclient/collparser/parser.y"
    {
		PRINT_R ("and_op", "or_op");
		(yyval.data) = (yyvsp[(1) - (1)].data);
	}
    break;

  case 8:
/* Line 1792 of yacc.c  */
#line 228 "/home/vdust/devel/xmms2/xmms2-all/src/clients/lib/xmmsclient/collparser/parser.y"
    {
		PRINT_R ("or_op TOKEN_OPERATOR_OR TOKEN_EOF_OPERATOR", "or_op");
		switch (collparser->mode) {
		case XMMS_COLLPARSER_MODE_COMPLETE:
			(yyval.data) = xcp_data_new_eof ();
			xcp_data_set_completion ((yyval.data),
			                         XMMS_COLLPARSER_COMPLETE_BINARY_OPERATOR);
			xcp_data_set_complete_infos ((yyval.data), &(yylsp[(2) - (3)]), NULL, -1, "OR");
			(yyval.data)->cant_close = 1;
			break;
		case XMMS_COLLPARSER_MODE_COMPILE:
			collparser->result = xmmsv_new_error ("Expression expected.");
			xcp_yyerror (&(yylsp[(3) - (3)]), collparser, scanner, XCP_ERROR_INCOMPLETE);
		default:
			YYERROR;
		}
		xcp_data_free ((yyvsp[(1) - (3)].data));
	}
    break;

  case 9:
/* Line 1792 of yacc.c  */
#line 246 "/home/vdust/devel/xmms2/xmms2-all/src/clients/lib/xmmsclient/collparser/parser.y"
    {
		PRINT_R ("or_op TOKEN_OPERATOR_OR TOKEN_EOF", "or_op");
		switch (collparser->mode) {
		case XMMS_COLLPARSER_MODE_COMPLETE:
			(yyval.data) = xcp_data_new_eof ();
			xcp_data_set_completion ((yyval.data),
			                         XMMS_COLLPARSER_COMPLETE_UNARY_OPERATOR |
			                         XMMS_COLLPARSER_COMPLETE_UNIVERSE |
			                         XMMS_COLLPARSER_COMPLETE_FIELD |
			                         XMMS_COLLPARSER_COMPLETE_FILTER |
			                         XMMS_COLLPARSER_COMPLETE_VALUE |
			                         XMMS_COLLPARSER_COMPLETE_SEQUENCE |
			                         XMMS_COLLPARSER_COMPLETE_OPEN_GROUP);
			(yyval.data)->cant_close = 1;
			break;
		case XMMS_COLLPARSER_MODE_COMPILE:
			collparser->result = xmmsv_new_error ("Expression expected.");
			xcp_yyerror (&(yylsp[(3) - (3)]), collparser, scanner, XCP_ERROR_INCOMPLETE);
		default:
			YYERROR;
		}
		xcp_data_free ((yyvsp[(1) - (3)].data));
	}
    break;

  case 10:
/* Line 1792 of yacc.c  */
#line 272 "/home/vdust/devel/xmms2/xmms2-all/src/clients/lib/xmmsclient/collparser/parser.y"
    {
		PRINT_R ("and_op expr", "and_op");
		switch (collparser->mode) {
		case XMMS_COLLPARSER_MODE_COMPLETE:
			if (!APPLY_OPERATOR ((yyvsp[(2) - (2)].data))) {
				(yyval.data) = (yyvsp[(2) - (2)].data);
				if (NEED_CONTEXT ((yyvsp[(2) - (2)].data))) {
					/* Set context. */
					xmmsv_ref ((yyvsp[(1) - (2)].data)->collection);
					(yyval.data)->collection = (yyvsp[(1) - (2)].data)->collection;
				}
			} else {
				xmmsv_coll_t *c;
				c = xcp_build_intersection ((yyvsp[(1) - (2)].data)->collection, (yyvsp[(2) - (2)].data)->collection);
				if (c) {
					(yyval.data) = xcp_data_new_coll (c);
					xmmsv_unref (c);
				} else {
					(yyval.data) = xcp_data_new ();
				}
				(yyval.data)->eof = (yyvsp[(2) - (2)].data)->eof;
				(yyval.data)->quote_eof = (yyvsp[(2) - (2)].data)->eof;
				(yyval.data)->cant_close = (yyvsp[(2) - (2)].data)->cant_close;
				xcp_data_merge_completion ((yyvsp[(2) - (2)].data), (yyval.data));
				xcp_data_copy_complete_infos ((yyvsp[(2) - (2)].data), (yyval.data));
				xcp_data_free ((yyvsp[(2) - (2)].data));
			}
			break;
		case XMMS_COLLPARSER_MODE_COMPILE:
			if (HAS_COLLECTION ((yyvsp[(2) - (2)].data))) {
				xmmsv_coll_t *c;
				c = xcp_build_intersection ((yyvsp[(1) - (2)].data)->collection, (yyvsp[(2) - (2)].data)->collection);
				if (c) {
					(yyval.data) = xcp_data_new_coll (c);
					xmmsv_unref (c);
					xcp_data_free ((yyvsp[(2) - (2)].data));
					break;
				}
			}
		default:
			YYERROR;
		}
		xcp_data_free ((yyvsp[(1) - (2)].data));
	}
    break;

  case 11:
/* Line 1792 of yacc.c  */
#line 316 "/home/vdust/devel/xmms2/xmms2-all/src/clients/lib/xmmsclient/collparser/parser.y"
    {
		PRINT_R ("and_op TOKEN_OPERATOR_AND expr", "and_op");
		switch (collparser->mode) {
		case XMMS_COLLPARSER_MODE_COMPLETE:
			if (!APPLY_OPERATOR ((yyvsp[(3) - (3)].data))) {
				(yyval.data) = (yyvsp[(3) - (3)].data);
				if (NEED_CONTEXT ((yyvsp[(3) - (3)].data))) {
					/* Set context. */
					xmmsv_ref ((yyvsp[(1) - (3)].data)->collection);
					(yyval.data)->collection = (yyvsp[(1) - (3)].data)->collection;
				}
			} else {
				xmmsv_coll_t *c;
				c = xcp_build_intersection ((yyvsp[(1) - (3)].data)->collection, (yyvsp[(3) - (3)].data)->collection);
				if (c) {
					(yyval.data) = xcp_data_new_coll (c);
					xmmsv_unref (c);
				} else {
					(yyval.data) = xcp_data_new ();
				}
				(yyval.data)->eof = (yyvsp[(3) - (3)].data)->eof;
				(yyval.data)->quote_eof = (yyvsp[(3) - (3)].data)->eof;
				(yyval.data)->cant_close = (yyvsp[(3) - (3)].data)->cant_close;
				xcp_data_merge_completion ((yyvsp[(3) - (3)].data), (yyval.data));
				xcp_data_copy_complete_infos ((yyvsp[(3) - (3)].data), (yyval.data));
				xcp_data_free ((yyvsp[(3) - (3)].data));
			}
			break;
		case XMMS_COLLPARSER_MODE_COMPILE:
			if (HAS_COLLECTION ((yyvsp[(3) - (3)].data))) {
				xmmsv_coll_t *c;
				c = xcp_build_intersection ((yyvsp[(1) - (3)].data)->collection, (yyvsp[(3) - (3)].data)->collection);
				if (c) {
					(yyval.data) = xcp_data_new_coll (c);
					xmmsv_unref (c);
				} else {
					(yyval.data) = xcp_data_new ();
				}
				xcp_data_free ((yyvsp[(3) - (3)].data));
				break;
			}
		default:
			YYERROR;
		}
		xcp_data_free ((yyvsp[(1) - (3)].data));
	}
    break;

  case 12:
/* Line 1792 of yacc.c  */
#line 362 "/home/vdust/devel/xmms2/xmms2-all/src/clients/lib/xmmsclient/collparser/parser.y"
    {
		PRINT_R ("expr", "and_op");
		(yyval.data) = (yyvsp[(1) - (1)].data);
	}
    break;

  case 13:
/* Line 1792 of yacc.c  */
#line 366 "/home/vdust/devel/xmms2/xmms2-all/src/clients/lib/xmmsclient/collparser/parser.y"
    {
		PRINT_R ("and_op TOKEN_OPERATOR_AND TOKEN_EOF_OPERATOR", "and_op");
		switch (collparser->mode) {
		case XMMS_COLLPARSER_MODE_COMPLETE:
			(yyval.data) = (yyvsp[(1) - (3)].data);
			(yyval.data)->eof = 1;
			(yyval.data)->cant_close = 1;
			xcp_data_set_completion ((yyval.data),
			                         XMMS_COLLPARSER_COMPLETE_BINARY_OPERATOR);
			xcp_data_set_complete_infos ((yyval.data), &(yylsp[(2) - (3)]), NULL, -1, "AND");
			break;
		case XMMS_COLLPARSER_MODE_COMPILE:
			collparser->result = xmmsv_new_error ("Expression expected.");
			xcp_yyerror (&(yylsp[(3) - (3)]), collparser, scanner, XCP_ERROR_INCOMPLETE);
		default:
			YYERROR;
		}
	}
    break;

  case 14:
/* Line 1792 of yacc.c  */
#line 384 "/home/vdust/devel/xmms2/xmms2-all/src/clients/lib/xmmsclient/collparser/parser.y"
    {
		PRINT_R ("and_op TOKEN_OPERATOR_AND TOKEN_EOF", "and_op");
		switch (collparser->mode) {
		case XMMS_COLLPARSER_MODE_COMPLETE:
			(yyval.data) = (yyvsp[(1) - (3)].data);
			(yyval.data)->eof = 1;
			(yyval.data)->cant_close = 1;
			xcp_data_set_completion ((yyval.data),
			                         XMMS_COLLPARSER_COMPLETE_UNARY_OPERATOR |
			                         XMMS_COLLPARSER_COMPLETE_UNIVERSE |
			                         XMMS_COLLPARSER_COMPLETE_FIELD |
			                         XMMS_COLLPARSER_COMPLETE_FILTER |
			                         XMMS_COLLPARSER_COMPLETE_VALUE |
			                         XMMS_COLLPARSER_COMPLETE_SEQUENCE |
			                         XMMS_COLLPARSER_COMPLETE_OPEN_GROUP);
			break;
		case XMMS_COLLPARSER_MODE_COMPILE:
			collparser->result = xmmsv_new_error ("Expression expected.");
			xcp_yyerror (&(yylsp[(3) - (3)]), collparser, scanner, XCP_ERROR_INCOMPLETE);
		default:
			YYERROR;
		}
	}
    break;

  case 15:
/* Line 1792 of yacc.c  */
#line 410 "/home/vdust/devel/xmms2/xmms2-all/src/clients/lib/xmmsclient/collparser/parser.y"
    {
		PRINT_R ("TOKEN_SYMBOL_ID sequence", "expr");
		switch (collparser->mode) {
		case XMMS_COLLPARSER_MODE_COMPLETE:
			if (HAS_SEQUENCE ((yyvsp[(2) - (2)].data)) && !HAS_EOF ((yyvsp[(2) - (2)].data))) {
				xmmsv_coll_t *c;
				c = xcp_build_idlist ((yyvsp[(2) - (2)].data)->sequence);
				if (c) {
					(yyval.data) = xcp_data_new_coll (c);
					xmmsv_unref (c);
				} else {
					(yyval.data) = xcp_data_new ();
				}
				(yyval.data)->eof = (yyvsp[(2) - (2)].data)->eof;
				xcp_data_merge_completion ((yyvsp[(2) - (2)].data), (yyval.data));
				xcp_data_copy_complete_infos ((yyvsp[(2) - (2)].data), (yyval.data));
				xcp_data_free ((yyvsp[(2) - (2)].data));
			} else { /* If no sequence, must have completion infos. */
				(yyval.data) = (yyvsp[(2) - (2)].data);
			}
			break;
		case XMMS_COLLPARSER_MODE_COMPILE:
			if (HAS_SEQUENCE ((yyvsp[(2) - (2)].data))) {
				xmmsv_coll_t *c;
				c = xcp_build_idlist ((yyvsp[(2) - (2)].data)->sequence);
				if (c) {
					(yyval.data) = xcp_data_new_coll (c);
					xmmsv_unref (c);
					xcp_data_free ((yyvsp[(2) - (2)].data));
					break;
				}
			}
		default:
			YYERROR;
		}
	}
    break;

  case 16:
/* Line 1792 of yacc.c  */
#line 446 "/home/vdust/devel/xmms2/xmms2-all/src/clients/lib/xmmsclient/collparser/parser.y"
    {
		PRINT_R ("filter", "expr");
		(yyval.data) = (yyvsp[(1) - (1)].data);
	}
    break;

  case 17:
/* Line 1792 of yacc.c  */
#line 450 "/home/vdust/devel/xmms2/xmms2-all/src/clients/lib/xmmsclient/collparser/parser.y"
    {
		PRINT_R ("group TOKEN_GROUP_CLOSE", "expr");
		if (!HAS_COLLECTION((yyvsp[(1) - (2)].data))) {
			xcp_yyerror (&(yylsp[(2) - (2)]), collparser, scanner, XCP_ERROR_GROUP_EMPTY);
			YYERROR; /* "()" not allowed. */
		}
		(yyvsp[(1) - (2)].data)->stop_context = 1;
		(yyval.data) = (yyvsp[(1) - (2)].data);
	}
    break;

  case 18:
/* Line 1792 of yacc.c  */
#line 459 "/home/vdust/devel/xmms2/xmms2-all/src/clients/lib/xmmsclient/collparser/parser.y"
    {
		PRINT_R ("TOKEN_OPERATOR_NOT expr", "expr");
		switch (collparser->mode) {
		case XMMS_COLLPARSER_MODE_COMPLETE:
			if (HAS_EOF ((yyvsp[(2) - (2)].data)) || !APPLY_OPERATOR ((yyvsp[(2) - (2)].data))) {
				(yyval.data) = (yyvsp[(2) - (2)].data);
				(yyval.data)->stop_context = 1;
			} else {
				xmmsv_coll_t *c;
				c = xcp_build_complement ((yyvsp[(2) - (2)].data)->collection);
				if (c) {
					(yyval.data) = xcp_data_new_coll (c);
					xmmsv_unref (c);
				} else {
					(yyval.data) = xcp_data_new ();
				}
				xcp_data_merge_completion ((yyvsp[(2) - (2)].data), (yyval.data));
				xcp_data_copy_complete_infos ((yyvsp[(2) - (2)].data), (yyval.data));
				xcp_data_free ((yyvsp[(2) - (2)].data));
			}
			break;
		case XMMS_COLLPARSER_MODE_COMPILE:
			if (HAS_COLLECTION ((yyvsp[(2) - (2)].data))) {
				xmmsv_coll_t *c;
				c = xcp_build_complement ((yyvsp[(2) - (2)].data)->collection);
				if (c) {
					(yyval.data) = xcp_data_new_coll (c);
					xmmsv_unref (c);
					xcp_data_free ((yyvsp[(2) - (2)].data));
					break;
				}
			}
		default:
			YYERROR;
		}
	}
    break;

  case 19:
/* Line 1792 of yacc.c  */
#line 495 "/home/vdust/devel/xmms2/xmms2-all/src/clients/lib/xmmsclient/collparser/parser.y"
    {
		PRINT_R ("TOKEN_REFERENCE string", "expr");
		switch (collparser->mode) {
		case XMMS_COLLPARSER_MODE_COMPLETE:
			if (HAS_STRING ((yyvsp[(2) - (2)].data)) && !HAS_EOF ((yyvsp[(2) - (2)].data))) {
				xmmsv_coll_t *c;
				c = xcp_build_reference ((yyvsp[(2) - (2)].data)->string);
				(yyval.data) = xcp_data_new_coll (c);
				xmmsv_unref (c);
			} else if (!HAS_EOF ((yyvsp[(2) - (2)].data))) {
				collparser->result = xmmsv_new_error ("Collection name expected.");
				xcp_yyerror (&(yylsp[(2) - (2)]), collparser, scanner, XCP_ERROR_INCOMPLETE);
				YYERROR;
			} else {
				(yyval.data) = xcp_data_new ();
			}
			if (HAS_EOF ((yyvsp[(2) - (2)].data))) {
				(yyval.data)->eof = 1;
				/* Need to propagate for CLOSE_GROUP hint. */
				(yyval.data)->quote_eof = (yyvsp[(2) - (2)].data)->quote_eof;
				(yyval.data)->cant_close = (yyvsp[(2) - (2)].data)->cant_close || !HAS_STRING ((yyvsp[(2) - (2)].data));
				xcp_data_set_completion ((yyval.data),
				                         XMMS_COLLPARSER_COMPLETE_REFERENCE);
				if (HAS_STRING ((yyvsp[(2) - (2)].data))) {
					xcp_data_set_complete_infos ((yyval.data), &(yylsp[(2) - (2)]), NULL, -1, (yyvsp[(2) - (2)].data)->string);
				} else {
					xcp_data_set_complete_infos ((yyval.data), &(yylsp[(2) - (2)]), NULL, -1, "");
				}
			}
			break;
		case XMMS_COLLPARSER_MODE_COMPILE:
			if (HAS_STRING ((yyvsp[(2) - (2)].data))) {
				xmmsv_coll_t *c;
				c = xcp_build_reference ((yyvsp[(2) - (2)].data)->string);
				(yyval.data) = xcp_data_new_coll (c);
				xmmsv_unref (c);
				break;
			} else {
				(yyval.data) = xcp_data_new ();
				collparser->result = xmmsv_new_error ("Collection name expected.");
				xcp_yyerror (&(yylsp[(2) - (2)]), collparser, scanner, XCP_ERROR_INCOMPLETE);
			}
		default:
			YYERROR;
		}
		xcp_data_free ((yyvsp[(2) - (2)].data));
	}
    break;

  case 20:
/* Line 1792 of yacc.c  */
#line 542 "/home/vdust/devel/xmms2/xmms2-all/src/clients/lib/xmmsclient/collparser/parser.y"
    {
		PRINT_R ("TOKEN_UNIVERSE", "expr");
		xmmsv_coll_t *c;
		c = xcp_build_universe ();
		(yyval.data) = xcp_data_new_coll (c);
		xmmsv_unref (c);
	}
    break;

  case 21:
/* Line 1792 of yacc.c  */
#line 549 "/home/vdust/devel/xmms2/xmms2-all/src/clients/lib/xmmsclient/collparser/parser.y"
    {
		PRINT_R ("TOKEN_UNIVERSE TOKEN_EOF_UNIVERSE", "expr");
		xmmsv_coll_t *c;
		c = xcp_build_universe ();
		(yyval.data) = xcp_data_new_coll (c);
		xmmsv_unref (c);
		if (collparser->mode == XMMS_COLLPARSER_MODE_COMPLETE) {
			xcp_data_set_completion ((yyval.data),
			                         XMMS_COLLPARSER_COMPLETE_UNIVERSE |
									 XMMS_COLLPARSER_COMPLETE_VALUE);
			xcp_data_set_complete_infos ((yyval.data), &(yylsp[(1) - (2)]), NULL, -1, "*");
		}
	}
    break;

  case 22:
/* Line 1792 of yacc.c  */
#line 562 "/home/vdust/devel/xmms2/xmms2-all/src/clients/lib/xmmsclient/collparser/parser.y"
    {
		PRINT_R ("group TOKEN_EOF", "expr");
		switch (collparser->mode) {
		case XMMS_COLLPARSER_MODE_COMPLETE:
			(yyval.data) = (yyvsp[(1) - (2)].data);
			(yyval.data)->eof = 1;
			if (!HAS_COMPLETION ((yyval.data))) {
				xcp_data_set_completion ((yyval.data),
				                         XMMS_COLLPARSER_COMPLETE_UNARY_OPERATOR |
				                         XMMS_COLLPARSER_COMPLETE_OPEN_GROUP |
				                         XMMS_COLLPARSER_COMPLETE_UNIVERSE |
				                         XMMS_COLLPARSER_COMPLETE_FIELD |
				                         XMMS_COLLPARSER_COMPLETE_FILTER |
				                         XMMS_COLLPARSER_COMPLETE_VALUE |
				                         XMMS_COLLPARSER_COMPLETE_SEQUENCE);
				if (HAS_COLLECTION ((yyval.data))) {
					xcp_data_add_completion ((yyval.data), XMMS_COLLPARSER_COMPLETE_BINARY_OPERATOR);
				} else {
					(yyval.data)->cant_close = 1;
				}
			}
			if (!(yyval.data)->cant_close) {
				xcp_data_add_completion ((yyval.data), XMMS_COLLPARSER_COMPLETE_CLOSE_GROUP);
			}
			break;
		case XMMS_COLLPARSER_MODE_COMPILE:
			collparser->result = xmmsv_new_error ("')' expected.");
			xcp_yyerror (&(yylsp[(2) - (2)]), collparser, scanner, XCP_ERROR_INCOMPLETE);
		default:
			YYERROR;
		}
	}
    break;

  case 23:
/* Line 1792 of yacc.c  */
#line 594 "/home/vdust/devel/xmms2/xmms2-all/src/clients/lib/xmmsclient/collparser/parser.y"
    {
		PRINT_R ("TOKEN_OPERATOR_NOT TOKEN_EOF_OPERATOR", "expr");
		switch (collparser->mode) {
		case XMMS_COLLPARSER_MODE_COMPLETE:
			(yyval.data) = xcp_data_new_eof ();
			xcp_data_set_completion ((yyval.data),
			                         XMMS_COLLPARSER_COMPLETE_UNARY_OPERATOR);
			xcp_data_set_complete_infos ((yyval.data), &(yylsp[(1) - (2)]), NULL, -1, "NOT");
			(yyval.data)->stop_context = 1;
			(yyval.data)->cant_close = 1;
			break;
		case XMMS_COLLPARSER_MODE_COMPILE:
			collparser->result = xmmsv_new_error ("Expression expected.");
			xcp_yyerror (&(yylsp[(2) - (2)]), collparser, scanner, XCP_ERROR_INCOMPLETE);
		default:
			YYERROR;
		}
	}
    break;

  case 24:
/* Line 1792 of yacc.c  */
#line 612 "/home/vdust/devel/xmms2/xmms2-all/src/clients/lib/xmmsclient/collparser/parser.y"
    {
		PRINT_R ("TOKEN_OPERATOR_NOT TOKEN_EOF", "expr");
		switch (collparser->mode) {
		case XMMS_COLLPARSER_MODE_COMPLETE:
			(yyval.data) = xcp_data_new_eof ();
			xcp_data_set_completion ((yyval.data),
			                         XMMS_COLLPARSER_COMPLETE_UNARY_OPERATOR |
			                         XMMS_COLLPARSER_COMPLETE_UNIVERSE |
			                         XMMS_COLLPARSER_COMPLETE_FIELD |
			                         XMMS_COLLPARSER_COMPLETE_FILTER |
			                         XMMS_COLLPARSER_COMPLETE_VALUE |
			                         XMMS_COLLPARSER_COMPLETE_SEQUENCE |
			                         XMMS_COLLPARSER_COMPLETE_OPEN_GROUP);
			(yyval.data)->stop_context = 1;
			(yyval.data)->cant_close = 1;
			break;
		case XMMS_COLLPARSER_MODE_COMPILE:
			collparser->result = xmmsv_new_error ("Expression expected.");
			xcp_yyerror (&(yylsp[(2) - (2)]), collparser, scanner, XCP_ERROR_INCOMPLETE);
		default:
			YYERROR;
		}
	}
    break;

  case 25:
/* Line 1792 of yacc.c  */
#line 638 "/home/vdust/devel/xmms2/xmms2-all/src/clients/lib/xmmsclient/collparser/parser.y"
    {
		PRINT_R ("TOKEN_STRING", "string");
		(yyval.data) = xcp_data_new_string ((yyvsp[(1) - (1)].str)); /* data struct take over string ownership. */
	}
    break;

  case 26:
/* Line 1792 of yacc.c  */
#line 642 "/home/vdust/devel/xmms2/xmms2-all/src/clients/lib/xmmsclient/collparser/parser.y"
    {
		PRINT_R ("TOKEN_STRING TOKEN_EOF_STRING", "string");
		(yyval.data) = xcp_data_new_string ((yyvsp[(1) - (2)].str));
		/* Ignored in compile mode. */
		(yyval.data)->eof = 1;
	}
    break;

  case 27:
/* Line 1792 of yacc.c  */
#line 648 "/home/vdust/devel/xmms2/xmms2-all/src/clients/lib/xmmsclient/collparser/parser.y"
    {
		PRINT_R ("TOKEN_EOF_QUOTE", "string");
		switch (collparser->mode) {
		case XMMS_COLLPARSER_MODE_COMPLETE:
			(yyval.data) = xcp_data_new_string ((yyvsp[(1) - (1)].str));
			(yyval.data)->eof = 1;
			(yyval.data)->quote_eof = 1;
			(yyval.data)->cant_close = 1;
			break;
		case XMMS_COLLPARSER_MODE_COMPILE:
			collparser->result = xmmsv_new_error ("Unclosed quote.");
			xcp_yyerror (&(yylsp[(1) - (1)]), collparser, scanner, XCP_ERROR_INCOMPLETE);
		default:
			YYERROR;
		}
	}
    break;

  case 28:
/* Line 1792 of yacc.c  */
#line 667 "/home/vdust/devel/xmms2/xmms2-all/src/clients/lib/xmmsclient/collparser/parser.y"
    {
		PRINT_R ("seqrange", "sequence");
		(yyval.data) = (yyvsp[(1) - (1)].data);
	}
    break;

  case 29:
/* Line 1792 of yacc.c  */
#line 671 "/home/vdust/devel/xmms2/xmms2-all/src/clients/lib/xmmsclient/collparser/parser.y"
    {
		PRINT_R ("sequence TOKEN_ID_SEQUENCE seqrange", "sequence");
		if (HAS_EOF ((yyvsp[(3) - (3)].data))) { /* Happens only in completion mode. */
			xcp_data_free ((yyvsp[(1) - (3)].data));
			(yyval.data) = (yyvsp[(3) - (3)].data);
		} else {
			(yyval.data) = (yyvsp[(1) - (3)].data);
			xcp_data_merge_sequence ((yyvsp[(3) - (3)].data), (yyvsp[(1) - (3)].data));
			xcp_data_free ((yyvsp[(3) - (3)].data));
		}
	}
    break;

  case 30:
/* Line 1792 of yacc.c  */
#line 685 "/home/vdust/devel/xmms2/xmms2-all/src/clients/lib/xmmsclient/collparser/parser.y"
    {
		PRINT_R ("id", "seqrange");
		int i = (yyvsp[(1) - (1)].ival) < 0 ? -(yyvsp[(1) - (1)].ival) - 1 : (yyvsp[(1) - (1)].ival);
		(yyval.data) = xcp_data_new_sequence (i, i);
		if (collparser->mode == XMMS_COLLPARSER_MODE_COMPLETE && (yyvsp[(1) - (1)].ival) < 0) {
			int n;
			char *buf;
			(yyval.data)->eof = 1;
			xcp_data_set_completion ((yyval.data),
			                         XMMS_COLLPARSER_COMPLETE_ID);
			n = ((yylsp[(1) - (1)])).last_column - ((yylsp[(1) - (1)])).first_column + 1;
			buf = malloc (n);
			snprintf(buf, n, "%d", i);
			xcp_data_set_complete_infos ((yyval.data), &(yylsp[(1) - (1)]), NULL, -1, buf);
			free(buf);
		}
	}
    break;

  case 31:
/* Line 1792 of yacc.c  */
#line 702 "/home/vdust/devel/xmms2/xmms2-all/src/clients/lib/xmmsclient/collparser/parser.y"
    {
		PRINT_R ("id TOKEN_ID_RANGE id", "seqrange");
		int i = (yyvsp[(3) - (3)].ival) < 0 ? -(yyvsp[(3) - (3)].ival) - 1 : (yyvsp[(3) - (3)].ival);
		(yyval.data) = xcp_data_new_sequence ((yyvsp[(1) - (3)].ival), i);
		if (collparser->mode == XMMS_COLLPARSER_MODE_COMPLETE && (yyvsp[(3) - (3)].ival) < 0) {
			int n;
			char *buf;
			(yyval.data)->eof = 1;
			xcp_data_set_completion ((yyval.data),
			                         XMMS_COLLPARSER_COMPLETE_ID);
			n = ((yylsp[(3) - (3)])).last_column - ((yylsp[(3) - (3)])).first_column + 1;
			buf = malloc (n);
			snprintf(buf, n, "%d", i);
			xcp_data_set_complete_infos ((yyval.data), &(yylsp[(3) - (3)]), NULL, -1, buf);
			free(buf);
		}
	}
    break;

  case 32:
/* Line 1792 of yacc.c  */
#line 719 "/home/vdust/devel/xmms2/xmms2-all/src/clients/lib/xmmsclient/collparser/parser.y"
    {
		PRINT_R ("TOKEN_EOF_SEQUENCE", "seqrange");
		switch (collparser->mode) {
		case XMMS_COLLPARSER_MODE_COMPLETE:
			(yyval.data) = xcp_data_new_eof ();
			(yyval.data)->cant_close = 1;
			xcp_data_set_completion ((yyval.data),
			                         XMMS_COLLPARSER_COMPLETE_ID);
			xcp_data_set_complete_infos ((yyval.data), &(yylsp[(1) - (1)]), NULL, -1, "");
			break;
		case XMMS_COLLPARSER_MODE_COMPILE:
			collparser->result = xmmsv_new_error ("Id expected.");
			xcp_yyerror (&(yylsp[(1) - (1)]), collparser, scanner, XCP_ERROR_INCOMPLETE);
		default:
			YYERROR;
		}
	}
    break;

  case 33:
/* Line 1792 of yacc.c  */
#line 736 "/home/vdust/devel/xmms2/xmms2-all/src/clients/lib/xmmsclient/collparser/parser.y"
    {
		PRINT_R ("id TOKEN_ID_RANGE TOKEN_EOF_SEQUENCE", "seqrange");
		switch (collparser->mode) {
		case XMMS_COLLPARSER_MODE_COMPLETE:
			(yyval.data) = xcp_data_new_eof ();
			(yyval.data)->cant_close = 1;
			xcp_data_set_completion ((yyval.data),
			                         XMMS_COLLPARSER_COMPLETE_ID);
			xcp_data_set_complete_infos ((yyval.data), &(yylsp[(3) - (3)]), NULL, -1, "");
			break;
		case XMMS_COLLPARSER_MODE_COMPILE:
			collparser->result = xmmsv_new_error ("Id expected.");
			xcp_yyerror (&(yylsp[(1) - (3)]), collparser, scanner, XCP_ERROR_INCOMPLETE);
		default:
			YYERROR;
		}
	}
    break;

  case 34:
/* Line 1792 of yacc.c  */
#line 756 "/home/vdust/devel/xmms2/xmms2-all/src/clients/lib/xmmsclient/collparser/parser.y"
    {
		PRINT_R ("TOKEN_ID", "id");
		(yyval.ival) = (yyvsp[(1) - (1)].ival);
	}
    break;

  case 35:
/* Line 1792 of yacc.c  */
#line 760 "/home/vdust/devel/xmms2/xmms2-all/src/clients/lib/xmmsclient/collparser/parser.y"
    {
		PRINT_R ("TOKEN_ID TOKEN_EOF_SEQUENCE", "id");
		(yyval.ival) = -(yyvsp[(1) - (2)].ival) - 1; /* add -1 to avoid annoying id 0. */
	}
    break;

  case 36:
/* Line 1792 of yacc.c  */
#line 767 "/home/vdust/devel/xmms2/xmms2-all/src/clients/lib/xmmsclient/collparser/parser.y"
    {
		PRINT_R ("TOKEN_GROUP_OPEN operation", "group");
		(yyval.data) = (yyvsp[(2) - (2)].data);
	}
    break;

  case 37:
/* Line 1792 of yacc.c  */
#line 771 "/home/vdust/devel/xmms2/xmms2-all/src/clients/lib/xmmsclient/collparser/parser.y"
    {
		PRINT_R ("TOKEN_GROUP_OPEN", "group");
		(yyval.data) = xcp_data_new ();
	}
    break;

  case 38:
/* Line 1792 of yacc.c  */
#line 778 "/home/vdust/devel/xmms2/xmms2-all/src/clients/lib/xmmsclient/collparser/parser.y"
    {
		PRINT_R ("unary_filter", "filter");
		(yyval.data) = (yyvsp[(1) - (1)].data);
	}
    break;

  case 39:
/* Line 1792 of yacc.c  */
#line 782 "/home/vdust/devel/xmms2/xmms2-all/src/clients/lib/xmmsclient/collparser/parser.y"
    {
		PRINT_R ("binary_filter", "filter");
		(yyval.data) = (yyvsp[(1) - (1)].data);
	}
    break;

  case 40:
/* Line 1792 of yacc.c  */
#line 786 "/home/vdust/devel/xmms2/xmms2-all/src/clients/lib/xmmsclient/collparser/parser.y"
    {
		PRINT_R ("string", "filter");
		int set_coll = 0;
		switch (collparser->mode) {
		case XMMS_COLLPARSER_MODE_COMPLETE:
			if (HAS_EOF ((yyvsp[(1) - (1)].data))) {
				(yyval.data) = (yyvsp[(1) - (1)].data);
				xcp_data_set_completion ((yyval.data),
				                         XMMS_COLLPARSER_COMPLETE_FIELD |
				                         XMMS_COLLPARSER_COMPLETE_VALUE);
				if (!(yyval.data)->quote_eof) {
					xcp_data_add_completion ((yyval.data), XMMS_COLLPARSER_COMPLETE_FILTER);
				}
				if (HAS_STRING ((yyvsp[(1) - (1)].data))) {
					xcp_data_set_complete_infos ((yyval.data), &(yylsp[(1) - (1)]), NULL, -1, (yyvsp[(1) - (1)].data)->string);
				} else {
					xcp_data_set_complete_infos ((yyval.data), &(yylsp[(1) - (1)]), NULL, -1, "");
				}
			} else {
				set_coll = 1;
			}
			break;
		case XMMS_COLLPARSER_MODE_COMPILE:
			set_coll = 1;
			break;
		default:
			YYERROR;
		}
		if (set_coll) {
			xmmsv_coll_t *c;
			if (HAS_STRING ((yyvsp[(1) - (1)].data))) {
				c = xcp_build_filter (XMMS_COLLECTION_TYPE_TOKEN, NULL, (yyvsp[(1) - (1)].data)->string);
			} else {
				c = xcp_build_filter (XMMS_COLLECTION_TYPE_TOKEN, NULL, "");
			}
			(yyval.data) = xcp_data_new_coll (c);
			xmmsv_unref (c);
			xcp_data_free ((yyvsp[(1) - (1)].data));
		}
	}
    break;

  case 41:
/* Line 1792 of yacc.c  */
#line 831 "/home/vdust/devel/xmms2/xmms2-all/src/clients/lib/xmmsclient/collparser/parser.y"
    {
		PRINT_R ("unary_filop property", "unary_filter");
		int set_coll = 0;
		switch (collparser->mode) {
		case XMMS_COLLPARSER_MODE_COMPLETE:
			if (HAS_EOF((yyvsp[(2) - (2)].data))) {
				(yyval.data) = xcp_data_new_eof();
				xcp_data_set_completion ((yyval.data), XMMS_COLLPARSER_COMPLETE_FIELD);
				xcp_data_set_complete_infos ((yyval.data), &(yylsp[(2) - (2)]), NULL, ((yyvsp[(1) - (2)].filop)).f, (yyvsp[(2) - (2)].data)->string);
				(yyval.data)->cant_close = (yyvsp[(2) - (2)].data)->cant_close;
				(yyval.data)->quote_eof = (yyval.data)->quote_eof;
			} else {
				set_coll = 1;
			}
			break;
		case XMMS_COLLPARSER_MODE_COMPILE:
			set_coll = 1;
			break;
		default:
			YYERROR;
		}
		if (set_coll) {
			xmmsv_coll_t *c;
			c = xcp_build_filter (((yyvsp[(1) - (2)].filop)).f, (yyvsp[(2) - (2)].data)->string, NULL);
			(yyval.data) = xcp_data_new_coll (c);
			xmmsv_unref (c);
		}
		xcp_data_free ((yyvsp[(2) - (2)].data));
	}
    break;

  case 42:
/* Line 1792 of yacc.c  */
#line 860 "/home/vdust/devel/xmms2/xmms2-all/src/clients/lib/xmmsclient/collparser/parser.y"
    {
		PRINT_R ("unary_filop TOKEN_EOF_FILTER", "unary_filter");
		switch (collparser->mode) {
		case XMMS_COLLPARSER_MODE_COMPLETE:
			(yyval.data) = xcp_data_new_eof ();
			xcp_data_set_completion ((yyval.data),
			                         XMMS_COLLPARSER_COMPLETE_FIELD);
			xcp_data_set_complete_infos ((yyval.data), &(yylsp[(2) - (2)]), NULL, ((yyvsp[(1) - (2)].filop)).f, "");
			(yyval.data)->cant_close = 1;
			break;
		case XMMS_COLLPARSER_MODE_COMPILE:
			collparser->result = xmmsv_new_error ("Property name expected.");
			xcp_yyerror (&(yylsp[(2) - (2)]), collparser, scanner, XCP_ERROR_INCOMPLETE);
		default:
			YYERROR;
		}
	}
    break;

  case 43:
/* Line 1792 of yacc.c  */
#line 880 "/home/vdust/devel/xmms2/xmms2-all/src/clients/lib/xmmsclient/collparser/parser.y"
    {
		PRINT_R ("TOKEN_FILTER_HAS", "unary_filop");
		(yyval.filop).f = XMMS_COLLECTION_TYPE_HAS;
		(yyval.filop).alt = 0;
	}
    break;

  case 44:
/* Line 1792 of yacc.c  */
#line 888 "/home/vdust/devel/xmms2/xmms2-all/src/clients/lib/xmmsclient/collparser/parser.y"
    {
		PRINT_R ("TOKEN_PROPERTY", "property");
		if (strlen((yyvsp[(1) - (1)].str)) == 0) { /* Cases like  +(  will reach this block. */
			collparser->result = xmmsv_new_error ("Property name empty.");
			xcp_yyerror (&(yylsp[(1) - (1)]), collparser, scanner, XCP_ERROR_INCOMPLETE);
			YYERROR;
		} else if (strlen((yyvsp[(1) - (1)].str)) == 1) {
			(yyval.data) = xcp_data_new_string (xcp_build_property_from_short ((yyvsp[(1) - (1)].str)));
			free ((yyvsp[(1) - (1)].str));
		} else {
			(yyval.data) = xcp_data_new_string ((yyvsp[(1) - (1)].str));
		}
	}
    break;

  case 45:
/* Line 1792 of yacc.c  */
#line 901 "/home/vdust/devel/xmms2/xmms2-all/src/clients/lib/xmmsclient/collparser/parser.y"
    {
		PRINT_R ("TOKEN_PROPERTY_EOF_QUOTE", "property");
		switch (collparser->mode) {
		case XMMS_COLLPARSER_MODE_COMPLETE:
			(yyval.data) = xcp_data_new_string ((yyvsp[(1) - (1)].str));
			(yyval.data)->eof = 1;
			(yyval.data)->quote_eof = 1;
			(yyval.data)->cant_close = 1;
			break;
		case XMMS_COLLPARSER_MODE_COMPILE:
			collparser->result = xmmsv_new_error ("Unclosed quote.");
			xcp_yyerror (&(yylsp[(1) - (1)]), collparser, scanner, XCP_ERROR_INCOMPLETE);
		default:
			YYERROR;
		}
	}
    break;

  case 46:
/* Line 1792 of yacc.c  */
#line 917 "/home/vdust/devel/xmms2/xmms2-all/src/clients/lib/xmmsclient/collparser/parser.y"
    {
		PRINT_R ("TOKEN_PROPERTY TOKEN_PROPERTY_EOF", "property");
		switch (collparser->mode) {
		case XMMS_COLLPARSER_MODE_COMPLETE:
			(yyval.data) = xcp_data_new_string ((yyvsp[(1) - (2)].str));
			(yyval.data)->eof = 1;
			break;
		case XMMS_COLLPARSER_MODE_COMPILE:
			if (strlen((yyvsp[(1) - (2)].str)) == 0) {
				/* XXX Probably unreachable block. Check with scanner. */
				collparser->result = xmmsv_new_error ("Property name empty.");
				xcp_yyerror (&(yylsp[(1) - (2)]), collparser, scanner, XCP_ERROR_INCOMPLETE);
				YYERROR;
			} else if (strlen((yyvsp[(1) - (2)].str)) == 1) {
				(yyval.data) = xcp_data_new_string (xcp_build_property_from_short ((yyvsp[(1) - (2)].str)));
				free ((yyvsp[(1) - (2)].str));
			} else {
				(yyval.data) = xcp_data_new_string ((yyvsp[(1) - (2)].str));
			}
			break;
		default:
			YYERROR;
		}
	}
    break;

  case 47:
/* Line 1792 of yacc.c  */
#line 944 "/home/vdust/devel/xmms2/xmms2-all/src/clients/lib/xmmsclient/collparser/parser.y"
    {
		PRINT_R ("property binary_filop string", "binary_filter");
		int set_coll = 0;
		switch (collparser->mode) {
		case XMMS_COLLPARSER_MODE_COMPLETE:
			if (HAS_EOF ((yyvsp[(3) - (3)].data))) {
				(yyval.data) = xcp_data_new_eof ();
				xcp_data_set_completion ((yyval.data),
				                         XMMS_COLLPARSER_COMPLETE_VALUE);
				(yyval.data)->quote_eof = (yyvsp[(3) - (3)].data)->quote_eof;
				(yyval.data)->cant_close = (yyvsp[(3) - (3)].data)->cant_close;
				if (HAS_STRING ((yyvsp[(3) - (3)].data))) {
					xcp_data_set_complete_infos ((yyval.data), &(yylsp[(3) - (3)]), (yyvsp[(1) - (3)].data)->string, ((yyvsp[(2) - (3)].filop)).f, (yyvsp[(3) - (3)].data)->string);
				} else {
					xcp_data_set_complete_infos ((yyval.data), &(yylsp[(3) - (3)]), (yyvsp[(1) - (3)].data)->string, ((yyvsp[(2) - (3)].filop)).f, "");
				}
				break;
			}
			/* If not EOF, same as compile mode */
		case XMMS_COLLPARSER_MODE_COMPILE:
			if (HAS_STRING ((yyvsp[(3) - (3)].data))) {
				set_coll = 1;
			} else {
				xcp_yyerror (&(yylsp[(3) - (3)]), collparser, scanner, XCP_ERROR_VALUE_EMPTY);
				YYERROR;
			}
			break;
		default:
			YYERROR;
		}
		if (set_coll) {
			xmmsv_coll_t *c;
			char *str;
			str = _prepare_match ((yyvsp[(3) - (3)].data)->string, (yyvsp[(2) - (3)].filop).alt);
			c = xcp_build_filter (((yyvsp[(2) - (3)].filop)).f, (yyvsp[(1) - (3)].data)->string, str);
			(yyval.data) = xcp_data_new_coll (c);
			xmmsv_unref (c);
			free (str);
		}
		xcp_data_free ((yyvsp[(1) - (3)].data));
		xcp_data_free ((yyvsp[(3) - (3)].data));
	}
    break;

  case 48:
/* Line 1792 of yacc.c  */
#line 986 "/home/vdust/devel/xmms2/xmms2-all/src/clients/lib/xmmsclient/collparser/parser.y"
    {
		PRINT_R ("binary_filop string", "binary_filter");
		int set_coll = 0;
		switch (collparser->mode) {
		case XMMS_COLLPARSER_MODE_COMPLETE:
			if (HAS_EOF ((yyvsp[(2) - (2)].data))) {
				(yyval.data) = xcp_data_new_eof ();
				xcp_data_set_completion ((yyval.data),
				                         XMMS_COLLPARSER_COMPLETE_VALUE);
				(yyval.data)->quote_eof = (yyvsp[(2) - (2)].data)->quote_eof;
				(yyval.data)->cant_close = (yyvsp[(2) - (2)].data)->cant_close;
				if (HAS_STRING ((yyvsp[(2) - (2)].data))) {
					xcp_data_set_complete_infos ((yyval.data), &(yylsp[(2) - (2)]), NULL, ((yyvsp[(1) - (2)].filop)).f, (yyvsp[(2) - (2)].data)->string);
				} else {
					xcp_data_set_complete_infos ((yyval.data), &(yylsp[(2) - (2)]), NULL, ((yyvsp[(1) - (2)].filop)).f, "");
				}
				break;
			}
			/* If not EOF, same as compile mode. */
		case XMMS_COLLPARSER_MODE_COMPILE:
			if (HAS_STRING ((yyvsp[(2) - (2)].data))) {
				set_coll = 1;
			} else {
				xcp_yyerror (&(yylsp[(2) - (2)]), collparser, scanner, XCP_ERROR_VALUE_EMPTY);
				YYERROR;
			}
			set_coll = 1;
			break;
		default:
			YYERROR;
		}
		if (set_coll) {
			xmmsv_coll_t *c;
			char *str;
			str = _prepare_match ((yyvsp[(2) - (2)].data)->string, (yyvsp[(1) - (2)].filop).alt);
			c = xcp_build_filter (((yyvsp[(1) - (2)].filop)).f, NULL, str);
			(yyval.data) = xcp_data_new_coll (c);
			xmmsv_unref (c);
			free (str);
		}
		xcp_data_free ((yyvsp[(2) - (2)].data));
	}
    break;

  case 49:
/* Line 1792 of yacc.c  */
#line 1028 "/home/vdust/devel/xmms2/xmms2-all/src/clients/lib/xmmsclient/collparser/parser.y"
    {
		PRINT_R ("property binary_filop TOKEN_EOF_FILTER", "binary_filter");
		switch (collparser->mode) {
		case XMMS_COLLPARSER_MODE_COMPLETE:
			(yyval.data) = xcp_data_new_eof ();
			xcp_data_set_completion ((yyval.data),
			                         XMMS_COLLPARSER_COMPLETE_VALUE);
			xcp_data_set_complete_infos ((yyval.data), &(yylsp[(3) - (3)]), (yyvsp[(1) - (3)].data)->string, ((yyvsp[(2) - (3)].filop)).f, "");
			(yyval.data)->cant_close = 1;
			xcp_data_free ((yyvsp[(1) - (3)].data));
			break;
		case XMMS_COLLPARSER_MODE_COMPILE:
			collparser->result = xmmsv_new_error ("Value expected.");
			xcp_yyerror (&(yylsp[(3) - (3)]), collparser, scanner, XCP_ERROR_INCOMPLETE);
		default:
			YYERROR;
		}
	}
    break;

  case 50:
/* Line 1792 of yacc.c  */
#line 1046 "/home/vdust/devel/xmms2/xmms2-all/src/clients/lib/xmmsclient/collparser/parser.y"
    {
		PRINT_R ("binary_filop TOKEN_EOF_FILTER", "binary_filter");
		switch (collparser->mode) {
		case XMMS_COLLPARSER_MODE_COMPLETE:
			(yyval.data) = xcp_data_new_eof ();
			xcp_data_set_completion ((yyval.data),
			                         XMMS_COLLPARSER_COMPLETE_VALUE);
			xcp_data_set_complete_infos ((yyval.data), &(yylsp[(2) - (2)]), NULL, ((yyvsp[(1) - (2)].filop)).f, "");
			(yyval.data)->cant_close = 1;
			break;
		case XMMS_COLLPARSER_MODE_COMPILE:
			collparser->result = xmmsv_new_error ("Value expected.");
			xcp_yyerror (&(yylsp[(2) - (2)]), collparser, scanner, XCP_ERROR_INCOMPLETE);
		default:
			YYERROR;
		}
	}
    break;

  case 51:
/* Line 1792 of yacc.c  */
#line 1066 "/home/vdust/devel/xmms2/xmms2-all/src/clients/lib/xmmsclient/collparser/parser.y"
    {
		PRINT_R ("TOKEN_FILTER_EQUAL", "binary_filop");
		(yyval.filop).f = XMMS_COLLECTION_TYPE_EQUALS;
		(yyval.filop).alt = 0;
	}
    break;

  case 52:
/* Line 1792 of yacc.c  */
#line 1071 "/home/vdust/devel/xmms2/xmms2-all/src/clients/lib/xmmsclient/collparser/parser.y"
    {
		PRINT_R ("TOKEN_FILTER_MATCH_GLOB", "binary_filop");
		(yyval.filop).f = XMMS_COLLECTION_TYPE_MATCH;
		(yyval.filop).alt = 1;
	}
    break;

  case 53:
/* Line 1792 of yacc.c  */
#line 1076 "/home/vdust/devel/xmms2/xmms2-all/src/clients/lib/xmmsclient/collparser/parser.y"
    {
		PRINT_R ("TOKEN_FILTER_MATCH", "binary_filop");
		(yyval.filop).f = XMMS_COLLECTION_TYPE_MATCH;
		(yyval.filop).alt = 0;
	}
    break;

  case 54:
/* Line 1792 of yacc.c  */
#line 1081 "/home/vdust/devel/xmms2/xmms2-all/src/clients/lib/xmmsclient/collparser/parser.y"
    {
		PRINT_R ("TOKEN_FILTER_TOKEN", "binary_filop");
		(yyval.filop).f = XMMS_COLLECTION_TYPE_TOKEN;
		(yyval.filop).alt = 0;
	}
    break;

  case 55:
/* Line 1792 of yacc.c  */
#line 1086 "/home/vdust/devel/xmms2/xmms2-all/src/clients/lib/xmmsclient/collparser/parser.y"
    {
		PRINT_R ("TOKEN_FILTER_LESS", "binary_filop");
		(yyval.filop).f = XMMS_COLLECTION_TYPE_SMALLER;
		(yyval.filop).alt = 0;
	}
    break;

  case 56:
/* Line 1792 of yacc.c  */
#line 1091 "/home/vdust/devel/xmms2/xmms2-all/src/clients/lib/xmmsclient/collparser/parser.y"
    {
		PRINT_R ("TOKEN_FILTER_LESSEQUAL", "binary_filop");
		(yyval.filop).f = XMMS_COLLECTION_TYPE_SMALLEREQ;
		(yyval.filop).alt = 0;
	}
    break;

  case 57:
/* Line 1792 of yacc.c  */
#line 1096 "/home/vdust/devel/xmms2/xmms2-all/src/clients/lib/xmmsclient/collparser/parser.y"
    {
		PRINT_R ("TOKEN_FILTER_GREATER", "binary_filop");
		(yyval.filop).f = XMMS_COLLECTION_TYPE_GREATER;
		(yyval.filop).alt = 0;
	}
    break;

  case 58:
/* Line 1792 of yacc.c  */
#line 1101 "/home/vdust/devel/xmms2/xmms2-all/src/clients/lib/xmmsclient/collparser/parser.y"
    {
		PRINT_R ("TOKEN_FILTER_GREATEREQUAL", "binary_filop");
		(yyval.filop).f = XMMS_COLLECTION_TYPE_GREATEREQ;
		(yyval.filop).alt = 0;
	}
    break;


/* Line 1792 of yacc.c  */
#line 2993 "parser.tab.c"
      default: break;
    }
  /* User semantic actions sometimes alter yychar, and that requires
     that yytoken be updated with the new translation.  We take the
     approach of translating immediately before every use of yytoken.
     One alternative is translating here after every semantic action,
     but that translation would be missed if the semantic action invokes
     YYABORT, YYACCEPT, or YYERROR immediately after altering yychar or
     if it invokes YYBACKUP.  In the case of YYABORT or YYACCEPT, an
     incorrect destructor might then be invoked immediately.  In the
     case of YYERROR or YYBACKUP, subsequent parser actions might lead
     to an incorrect destructor call or verbose syntax error message
     before the lookahead is translated.  */
  YY_SYMBOL_PRINT ("-> $$ =", yyr1[yyn], &yyval, &yyloc);

  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);

  *++yyvsp = yyval;
  *++yylsp = yyloc;

  /* Now `shift' the result of the reduction.  Determine what state
     that goes to, based on the state we popped back to and the rule
     number reduced by.  */

  yyn = yyr1[yyn];

  yystate = yypgoto[yyn - YYNTOKENS] + *yyssp;
  if (0 <= yystate && yystate <= YYLAST && yycheck[yystate] == *yyssp)
    yystate = yytable[yystate];
  else
    yystate = yydefgoto[yyn - YYNTOKENS];

  goto yynewstate;


/*------------------------------------.
| yyerrlab -- here on detecting error |
`------------------------------------*/
yyerrlab:
  /* Make sure we have latest lookahead translation.  See comments at
     user semantic actions for why this is necessary.  */
  yytoken = yychar == YYEMPTY ? YYEMPTY : YYTRANSLATE (yychar);

  /* If not already recovering from an error, report this error.  */
  if (!yyerrstatus)
    {
      ++yynerrs;
#if ! YYERROR_VERBOSE
      yyerror (&yylloc, collparser, scanner, YY_("syntax error"));
#else
# define YYSYNTAX_ERROR yysyntax_error (&yymsg_alloc, &yymsg, \
                                        yyssp, yytoken)
      {
        char const *yymsgp = YY_("syntax error");
        int yysyntax_error_status;
        yysyntax_error_status = YYSYNTAX_ERROR;
        if (yysyntax_error_status == 0)
          yymsgp = yymsg;
        else if (yysyntax_error_status == 1)
          {
            if (yymsg != yymsgbuf)
              YYSTACK_FREE (yymsg);
            yymsg = (char *) YYSTACK_ALLOC (yymsg_alloc);
            if (!yymsg)
              {
                yymsg = yymsgbuf;
                yymsg_alloc = sizeof yymsgbuf;
                yysyntax_error_status = 2;
              }
            else
              {
                yysyntax_error_status = YYSYNTAX_ERROR;
                yymsgp = yymsg;
              }
          }
        yyerror (&yylloc, collparser, scanner, yymsgp);
        if (yysyntax_error_status == 2)
          goto yyexhaustedlab;
      }
# undef YYSYNTAX_ERROR
#endif
    }

  yyerror_range[1] = yylloc;

  if (yyerrstatus == 3)
    {
      /* If just tried and failed to reuse lookahead token after an
	 error, discard it.  */

      if (yychar <= YYEOF)
	{
	  /* Return failure if at end of input.  */
	  if (yychar == YYEOF)
	    YYABORT;
	}
      else
	{
	  yydestruct ("Error: discarding",
		      yytoken, &yylval, &yylloc, collparser, scanner);
	  yychar = YYEMPTY;
	}
    }

  /* Else will try to reuse lookahead token after shifting the error
     token.  */
  goto yyerrlab1;


/*---------------------------------------------------.
| yyerrorlab -- error raised explicitly by YYERROR.  |
`---------------------------------------------------*/
yyerrorlab:

  /* Pacify compilers like GCC when the user code never invokes
     YYERROR and the label yyerrorlab therefore never appears in user
     code.  */
  if (/*CONSTCOND*/ 0)
     goto yyerrorlab;

  yyerror_range[1] = yylsp[1-yylen];
  /* Do not reclaim the symbols of the rule which action triggered
     this YYERROR.  */
  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);
  yystate = *yyssp;
  goto yyerrlab1;


/*-------------------------------------------------------------.
| yyerrlab1 -- common code for both syntax error and YYERROR.  |
`-------------------------------------------------------------*/
yyerrlab1:
  yyerrstatus = 3;	/* Each real token shifted decrements this.  */

  for (;;)
    {
      yyn = yypact[yystate];
      if (!yypact_value_is_default (yyn))
	{
	  yyn += YYTERROR;
	  if (0 <= yyn && yyn <= YYLAST && yycheck[yyn] == YYTERROR)
	    {
	      yyn = yytable[yyn];
	      if (0 < yyn)
		break;
	    }
	}

      /* Pop the current state because it cannot handle the error token.  */
      if (yyssp == yyss)
	YYABORT;

      yyerror_range[1] = *yylsp;
      yydestruct ("Error: popping",
		  yystos[yystate], yyvsp, yylsp, collparser, scanner);
      YYPOPSTACK (1);
      yystate = *yyssp;
      YY_STACK_PRINT (yyss, yyssp);
    }

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END

  yyerror_range[2] = yylloc;
  /* Using YYLLOC is tempting, but would change the location of
     the lookahead.  YYLOC is available though.  */
  YYLLOC_DEFAULT (yyloc, yyerror_range, 2);
  *++yylsp = yyloc;

  /* Shift the error token.  */
  YY_SYMBOL_PRINT ("Shifting", yystos[yyn], yyvsp, yylsp);

  yystate = yyn;
  goto yynewstate;


/*-------------------------------------.
| yyacceptlab -- YYACCEPT comes here.  |
`-------------------------------------*/
yyacceptlab:
  yyresult = 0;
  goto yyreturn;

/*-----------------------------------.
| yyabortlab -- YYABORT comes here.  |
`-----------------------------------*/
yyabortlab:
  yyresult = 1;
  goto yyreturn;

#if !defined yyoverflow || YYERROR_VERBOSE
/*-------------------------------------------------.
| yyexhaustedlab -- memory exhaustion comes here.  |
`-------------------------------------------------*/
yyexhaustedlab:
  yyerror (&yylloc, collparser, scanner, YY_("memory exhausted"));
  yyresult = 2;
  /* Fall through.  */
#endif

yyreturn:
  if (yychar != YYEMPTY)
    {
      /* Make sure we have latest lookahead translation.  See comments at
         user semantic actions for why this is necessary.  */
      yytoken = YYTRANSLATE (yychar);
      yydestruct ("Cleanup: discarding lookahead",
                  yytoken, &yylval, &yylloc, collparser, scanner);
    }
  /* Do not reclaim the symbols of the rule which action triggered
     this YYABORT or YYACCEPT.  */
  YYPOPSTACK (yylen);
  YY_STACK_PRINT (yyss, yyssp);
  while (yyssp != yyss)
    {
      yydestruct ("Cleanup: popping",
		  yystos[*yyssp], yyvsp, yylsp, collparser, scanner);
      YYPOPSTACK (1);
    }
#ifndef yyoverflow
  if (yyss != yyssa)
    YYSTACK_FREE (yyss);
#endif
#if YYERROR_VERBOSE
  if (yymsg != yymsgbuf)
    YYSTACK_FREE (yymsg);
#endif
  /* Make sure YYID is used.  */
  return YYID (yyresult);
}


/* Line 2055 of yacc.c  */
#line 1108 "/home/vdust/devel/xmms2/xmms2-all/src/clients/lib/xmmsclient/collparser/parser.y"


void
xcp_parse (xmmsv_collparser_t *collparser, void *scanner)
{
	xcp_yyparse (collparser, scanner);
}

static char *
_prepare_match (const char *s, int alt)
{
	char *str;
	int n;
	n = strlen(s);
	if (alt == 0 || (s[0] == '*' && s[n-1] == '*')) {
		str = xcp_strdup_safe (s);
	} else if (s[0] == '*') {
		str = xcp_strjoin (s, "*", NULL);
	} else if (s[n-1] == '*') {
		str = xcp_strjoin ("*", s, NULL);
	} else {
		str = xcp_strjoin ("*", s, "*", NULL);
	}
	return str;
}

void xcp_yyerror (YYLTYPE *locp, xmmsv_collparser_t *collparser, void *scanner, const char *msg)
{
	/* TODO: Format error. */
	collparser->result = xmmsv_new_error (msg);
}

