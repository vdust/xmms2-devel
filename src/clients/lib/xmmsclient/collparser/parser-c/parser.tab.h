/* A Bison parser, made by GNU Bison 2.7.  */

/* Bison interface for Yacc-like parsers in C
   
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
/* Line 2058 of yacc.c  */
#line 54 "/home/vdust/devel/xmms2/xmms2-all/src/clients/lib/xmmsclient/collparser/parser.y"

	int ival;
	unsigned int group;
	char *str;
	struct {
		xmmsv_coll_type_t f;
		int alt;
	} filop;
	xcp_data_t *data;


/* Line 2058 of yacc.c  */
#line 103 "parser.tab.h"
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
