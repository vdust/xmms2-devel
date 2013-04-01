/*  XMMS2 - X Music Multiplexer System
 *  Copyright (C) 2003-2011 XMMS2 Team
 *
 *  PLUGINS ARE NOT CONSIDERED TO BE DERIVED WORK !!!
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Lesser General Public
 *  License as published by the Free Software Foundation; either
 *  version 2.1 of the License, or (at your option) any later version.
 *
 *  This library is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  Lesser General Public License for more details.
 */

%define api.pure
%name-prefix="xcp_yy"

%locations
%defines
%error-verbose

%{
#include <stdio.h> /* snprintf */
#include <string.h> /* strdup, strlen */

#include "collparser_priv.h"
#include "parser_data_priv.h"
#include "collbuilder_priv.h"
#include "utils_priv.h"
#include "errors_priv.h"

/* uncomment this line to show all reductions. */
/* #define SHOW_REDUCTION 1 */
%}

%parse-param {xmmsv_collparser_t *collparser}
%parse-param {void *scanner}
%lex-param {void *scanner}

%{
#ifdef SHOW_REDUCTION
# define PRINT_R(from, to) printf ("%s <-- %s\n", to, from)
#else
# define PRINT_R(from, to) ((void)0)
#endif

static char *_prepare_match (const char *s, int alt, int is_url);
void xcp_yyerror (YYLTYPE *locp, xmmsv_collparser_t *collparser, void *scanner, const char *msg);
int xcp_yylex (YYSTYPE *lval, YYLTYPE *lloc, void *scanner);
%}

%union {
	int ival;
	unsigned int group;
	char *str;
	struct {
		xmmsv_coll_type_t f;
		int alt;
	} filop;
	xcp_data_t *data;
}

%token TOKEN_EOF
%token TOKEN_EOF_FILTER
%token TOKEN_EOF_OPERATOR
%token TOKEN_EOF_PROPERTY
%token <str> TOKEN_EOF_QUOTE
%token TOKEN_EOF_SEQUENCE
%token TOKEN_EOF_STRING
%token TOKEN_EOF_UNIVERSE
%token TOKEN_ERR
%token TOKEN_FILTER_EQUAL
%token TOKEN_FILTER_GREATER
%token TOKEN_FILTER_GREATEREQUAL
%token TOKEN_FILTER_HAS
%token TOKEN_FILTER_LESS
%token TOKEN_FILTER_LESSEQUAL
%token TOKEN_FILTER_MATCH
%token TOKEN_FILTER_MATCH_GLOB
%token TOKEN_FILTER_TOKEN
%token <group> TOKEN_GROUP_CLOSE
%token <group> TOKEN_GROUP_OPEN
%token <ival> TOKEN_ID
%token TOKEN_ID_RANGE
%token TOKEN_ID_SEQUENCE
%token TOKEN_OPERATOR_AND
%token TOKEN_OPERATOR_NOT
%token TOKEN_OPERATOR_OR
%token <str> TOKEN_PROPERTY
%token <str> TOKEN_PROPERTY_EOF_QUOTE
%token TOKEN_REFERENCE
%token <str> TOKEN_STRING
%token TOKEN_SYMBOL_ID
%token TOKEN_UNIVERSE


%type <data> and_op
%type <filop> binary_filop
%type <data> binary_filter
%type <data> expr
%type <data> filter
%type <data> group
%type <ival> id
%type <data> operation
%type <data> or_op
%type <data>  property
%type <data> seqrange
%type <data> sequence
%type <data> string
%type <filop> unary_filop
%type <data> unary_filter

%start s

%destructor { free ($$); } <str>
%destructor { xcp_data_free ($$); } <data>

%%

s :
	TOKEN_EOF {
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
			xcp_yyerror (&@1, collparser, scanner, XCP_ERROR_PATTERN_EMPTY);
		default:
			YYERROR;
		}
	}
|	operation TOKEN_EOF {
		PRINT_R ("operation TOKEN_EOF", "s");
		if (collparser->mode == XMMS_COLLPARSER_MODE_COMPLETE) {
			if (!HAS_COMPLETION ($1)) {
				xcp_data_set_completion ($1,
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
		collparser->result = xcp_data_build_result ($1, collparser->mode);
		xcp_data_free ($1);
	}
|	operation { /* Can happen only when an expression is incomplete. */
		PRINT_R ("operation", "s");
		switch (collparser->mode) {
		case XMMS_COLLPARSER_MODE_COMPLETE:
			collparser->result = xcp_data_build_result ($1, collparser->mode);
			break;
		case XMMS_COLLPARSER_MODE_COMPILE:
			collparser->result = xmmsv_ref ($1->error);
			xcp_yyerror (&@1, collparser, scanner, XCP_ERROR_INCOMPLETE);
		default:
			YYERROR;
		}
		xcp_data_free ($1);
	}
;

operation :
	or_op {
		PRINT_R ("or_op", "operation");
		$$ = $1;
	}
;

or_op :
	or_op TOKEN_OPERATOR_OR and_op {
		PRINT_R ("or_op TOKEN_OPERATOR_OR and_op", "or_op");
		switch (collparser->mode) {
		case XMMS_COLLPARSER_MODE_COMPLETE:
			if (HAS_EOF ($3) || !APPLY_OPERATOR ($3)) {
				$$ = $3;
			} else {
				xmmsv_t *c;
				c = xcp_build_union ($1->collection, $3->collection);
				if (c) {
					$$ = xcp_data_new_coll (c);
					xmmsv_unref (c);
				} else {
					$$ = xcp_data_new ();
				}
				xcp_data_free ($3);
			}
			break;
		case XMMS_COLLPARSER_MODE_COMPILE:
			if (HAS_COLLECTION ($3)) {
				xmmsv_t *c;
				c = xcp_build_union ($1->collection, $3->collection);
				if (c) {
					$$ = xcp_data_new_coll (c);
					xmmsv_unref (c);
					xcp_data_free ($3);
					break;
				}
			}
			/* Should never reached this point.*/
		default:
			YYERROR;
		}
		xcp_data_free ($1);
	}
|	and_op {
		PRINT_R ("and_op", "or_op");
		$$ = $1;
	}
|   or_op TOKEN_OPERATOR_OR TOKEN_EOF_OPERATOR {
		PRINT_R ("or_op TOKEN_OPERATOR_OR TOKEN_EOF_OPERATOR", "or_op");
		switch (collparser->mode) {
		case XMMS_COLLPARSER_MODE_COMPLETE:
			$$ = xcp_data_new_eof ();
			xcp_data_set_completion ($$,
			                         XMMS_COLLPARSER_COMPLETE_BINARY_OPERATOR);
			xcp_data_set_complete_infos ($$, &@2, NULL, -1, "OR");
			$$->cant_close = 1;
			break;
		case XMMS_COLLPARSER_MODE_COMPILE:
			collparser->result = xmmsv_new_error ("Expression expected.");
			xcp_yyerror (&@3, collparser, scanner, XCP_ERROR_INCOMPLETE);
		default:
			YYERROR;
		}
		xcp_data_free ($1);
	}
|	or_op TOKEN_OPERATOR_OR TOKEN_EOF {
		PRINT_R ("or_op TOKEN_OPERATOR_OR TOKEN_EOF", "or_op");
		switch (collparser->mode) {
		case XMMS_COLLPARSER_MODE_COMPLETE:
			$$ = xcp_data_new_eof ();
			xcp_data_set_completion ($$,
			                         XMMS_COLLPARSER_COMPLETE_UNARY_OPERATOR |
			                         XMMS_COLLPARSER_COMPLETE_UNIVERSE |
			                         XMMS_COLLPARSER_COMPLETE_FIELD |
			                         XMMS_COLLPARSER_COMPLETE_FILTER |
			                         XMMS_COLLPARSER_COMPLETE_VALUE |
			                         XMMS_COLLPARSER_COMPLETE_SEQUENCE |
			                         XMMS_COLLPARSER_COMPLETE_OPEN_GROUP);
			$$->cant_close = 1;
			break;
		case XMMS_COLLPARSER_MODE_COMPILE:
			collparser->result = xmmsv_new_error ("Expression expected.");
			xcp_yyerror (&@3, collparser, scanner, XCP_ERROR_INCOMPLETE);
		default:
			YYERROR;
		}
		xcp_data_free ($1);
	}
;

and_op :
	and_op expr {
		PRINT_R ("and_op expr", "and_op");
		switch (collparser->mode) {
		case XMMS_COLLPARSER_MODE_COMPLETE:
			if (!APPLY_OPERATOR ($2)) {
				$$ = $2;
				if (NEED_CONTEXT ($2)) {
					/* Set context. */
					xmmsv_ref ($1->collection);
					$$->collection = $1->collection;
				}
			} else {
				xmmsv_t *c;
				c = xcp_build_intersection ($1->collection, $2->collection);
				if (c) {
					$$ = xcp_data_new_coll (c);
					xmmsv_unref (c);
				} else {
					$$ = xcp_data_new ();
				}
				$$->eof = $2->eof;
				$$->quote_eof = $2->eof;
				$$->cant_close = $2->cant_close;
				xcp_data_merge_completion ($2, $$);
				xcp_data_copy_complete_infos ($2, $$);
				xcp_data_free ($2);
			}
			break;
		case XMMS_COLLPARSER_MODE_COMPILE:
			if (HAS_COLLECTION ($2)) {
				xmmsv_t *c;
				c = xcp_build_intersection ($1->collection, $2->collection);
				if (c) {
					$$ = xcp_data_new_coll (c);
					xmmsv_unref (c);
					xcp_data_free ($2);
					break;
				}
			}
		default:
			YYERROR;
		}
		xcp_data_free ($1);
	}
|	and_op TOKEN_OPERATOR_AND expr {
		PRINT_R ("and_op TOKEN_OPERATOR_AND expr", "and_op");
		switch (collparser->mode) {
		case XMMS_COLLPARSER_MODE_COMPLETE:
			if (!APPLY_OPERATOR ($3)) {
				$$ = $3;
				if (NEED_CONTEXT ($3)) {
					/* Set context. */
					xmmsv_ref ($1->collection);
					$$->collection = $1->collection;
				}
			} else {
				xmmsv_t *c;
				c = xcp_build_intersection ($1->collection, $3->collection);
				if (c) {
					$$ = xcp_data_new_coll (c);
					xmmsv_unref (c);
				} else {
					$$ = xcp_data_new ();
				}
				$$->eof = $3->eof;
				$$->quote_eof = $3->eof;
				$$->cant_close = $3->cant_close;
				xcp_data_merge_completion ($3, $$);
				xcp_data_copy_complete_infos ($3, $$);
				xcp_data_free ($3);
			}
			break;
		case XMMS_COLLPARSER_MODE_COMPILE:
			if (HAS_COLLECTION ($3)) {
				xmmsv_t *c;
				c = xcp_build_intersection ($1->collection, $3->collection);
				if (c) {
					$$ = xcp_data_new_coll (c);
					xmmsv_unref (c);
				} else {
					$$ = xcp_data_new ();
				}
				xcp_data_free ($3);
				break;
			}
		default:
			YYERROR;
		}
		xcp_data_free ($1);
	}
|	expr {
		PRINT_R ("expr", "and_op");
		$$ = $1;
	}
| and_op TOKEN_OPERATOR_AND TOKEN_EOF_OPERATOR {
		PRINT_R ("and_op TOKEN_OPERATOR_AND TOKEN_EOF_OPERATOR", "and_op");
		switch (collparser->mode) {
		case XMMS_COLLPARSER_MODE_COMPLETE:
			$$ = $1;
			$$->eof = 1;
			$$->cant_close = 1;
			xcp_data_set_completion ($$,
			                         XMMS_COLLPARSER_COMPLETE_BINARY_OPERATOR);
			xcp_data_set_complete_infos ($$, &@2, NULL, -1, "AND");
			break;
		case XMMS_COLLPARSER_MODE_COMPILE:
			collparser->result = xmmsv_new_error ("Expression expected.");
			xcp_yyerror (&@3, collparser, scanner, XCP_ERROR_INCOMPLETE);
		default:
			YYERROR;
		}
	}
| and_op TOKEN_OPERATOR_AND TOKEN_EOF {
		PRINT_R ("and_op TOKEN_OPERATOR_AND TOKEN_EOF", "and_op");
		switch (collparser->mode) {
		case XMMS_COLLPARSER_MODE_COMPLETE:
			$$ = $1;
			$$->eof = 1;
			$$->cant_close = 1;
			xcp_data_set_completion ($$,
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
			xcp_yyerror (&@3, collparser, scanner, XCP_ERROR_INCOMPLETE);
		default:
			YYERROR;
		}
	}
;

expr :
	TOKEN_SYMBOL_ID sequence {
		PRINT_R ("TOKEN_SYMBOL_ID sequence", "expr");
		switch (collparser->mode) {
		case XMMS_COLLPARSER_MODE_COMPLETE:
			if (HAS_SEQUENCE ($2) && !HAS_EOF ($2)) {
				xmmsv_t *c;
				c = xcp_build_idlist ($2->sequence);
				if (c) {
					$$ = xcp_data_new_coll (c);
					xmmsv_unref (c);
				} else {
					$$ = xcp_data_new ();
				}
				$$->eof = $2->eof;
				xcp_data_merge_completion ($2, $$);
				xcp_data_copy_complete_infos ($2, $$);
				xcp_data_free ($2);
			} else { /* If no sequence, must have completion infos. */
				$$ = $2;
			}
			break;
		case XMMS_COLLPARSER_MODE_COMPILE:
			if (HAS_SEQUENCE ($2)) {
				xmmsv_t *c;
				c = xcp_build_idlist ($2->sequence);
				if (c) {
					$$ = xcp_data_new_coll (c);
					xmmsv_unref (c);
					xcp_data_free ($2);
					break;
				}
			}
		default:
			YYERROR;
		}
	}
|	filter {
		PRINT_R ("filter", "expr");
		$$ = $1;
	}
|	group TOKEN_GROUP_CLOSE {
		PRINT_R ("group TOKEN_GROUP_CLOSE", "expr");
		if (!HAS_COLLECTION($1)) {
			xcp_yyerror (&@2, collparser, scanner, XCP_ERROR_GROUP_EMPTY);
			YYERROR; /* "()" not allowed. */
		}
		$1->stop_context = 1;
		$$ = $1;
	}
|	TOKEN_OPERATOR_NOT expr {
		PRINT_R ("TOKEN_OPERATOR_NOT expr", "expr");
		switch (collparser->mode) {
		case XMMS_COLLPARSER_MODE_COMPLETE:
			if (HAS_EOF ($2) || !APPLY_OPERATOR ($2)) {
				$$ = $2;
				$$->stop_context = 1;
			} else {
				xmmsv_t *c;
				c = xcp_build_complement ($2->collection);
				if (c) {
					$$ = xcp_data_new_coll (c);
					xmmsv_unref (c);
				} else {
					$$ = xcp_data_new ();
				}
				xcp_data_merge_completion ($2, $$);
				xcp_data_copy_complete_infos ($2, $$);
				xcp_data_free ($2);
			}
			break;
		case XMMS_COLLPARSER_MODE_COMPILE:
			if (HAS_COLLECTION ($2)) {
				xmmsv_t *c;
				c = xcp_build_complement ($2->collection);
				if (c) {
					$$ = xcp_data_new_coll (c);
					xmmsv_unref (c);
					xcp_data_free ($2);
					break;
				}
			}
		default:
			YYERROR;
		}
	}
|	TOKEN_REFERENCE string {
		PRINT_R ("TOKEN_REFERENCE string", "expr");
		switch (collparser->mode) {
		case XMMS_COLLPARSER_MODE_COMPLETE:
			if (HAS_STRING ($2) && !HAS_EOF ($2)) {
				xmmsv_t *c;
				c = xcp_build_reference ($2->string);
				$$ = xcp_data_new_coll (c);
				xmmsv_unref (c);
			} else if (!HAS_EOF ($2)) {
				collparser->result = xmmsv_new_error ("Collection name expected.");
				xcp_yyerror (&@2, collparser, scanner, XCP_ERROR_INCOMPLETE);
				YYERROR;
			} else {
				$$ = xcp_data_new ();
			}
			if (HAS_EOF ($2)) {
				$$->eof = 1;
				/* Need to propagate for CLOSE_GROUP hint. */
				$$->quote_eof = $2->quote_eof;
				$$->cant_close = $2->cant_close || !HAS_STRING ($2);
				xcp_data_set_completion ($$,
				                         XMMS_COLLPARSER_COMPLETE_REFERENCE);
				if (HAS_STRING ($2)) {
					xcp_data_set_complete_infos ($$, &@2, NULL, -1, $2->string);
				} else {
					xcp_data_set_complete_infos ($$, &@2, NULL, -1, "");
				}
			}
			break;
		case XMMS_COLLPARSER_MODE_COMPILE:
			if (HAS_STRING ($2)) {
				xmmsv_t *c;
				c = xcp_build_reference ($2->string);
				$$ = xcp_data_new_coll (c);
				xmmsv_unref (c);
				break;
			} else {
				$$ = xcp_data_new ();
				collparser->result = xmmsv_new_error ("Collection name expected.");
				xcp_yyerror (&@2, collparser, scanner, XCP_ERROR_INCOMPLETE);
			}
		default:
			YYERROR;
		}
		xcp_data_free ($2);
	}
|	TOKEN_UNIVERSE {
		PRINT_R ("TOKEN_UNIVERSE", "expr");
		xmmsv_t *c;
		c = xcp_build_universe ();
		$$ = xcp_data_new_coll (c);
		xmmsv_unref (c);
	}
|	TOKEN_UNIVERSE TOKEN_EOF_UNIVERSE {
		PRINT_R ("TOKEN_UNIVERSE TOKEN_EOF_UNIVERSE", "expr");
		xmmsv_t *c;
		c = xcp_build_universe ();
		$$ = xcp_data_new_coll (c);
		xmmsv_unref (c);
		if (collparser->mode == XMMS_COLLPARSER_MODE_COMPLETE) {
			xcp_data_set_completion ($$,
			                         XMMS_COLLPARSER_COMPLETE_UNIVERSE |
									 XMMS_COLLPARSER_COMPLETE_VALUE);
			xcp_data_set_complete_infos ($$, &@1, NULL, -1, "*");
		}
	}
|	group TOKEN_EOF {
		PRINT_R ("group TOKEN_EOF", "expr");
		switch (collparser->mode) {
		case XMMS_COLLPARSER_MODE_COMPLETE:
			$$ = $1;
			$$->eof = 1;
			if (!HAS_COMPLETION ($$)) {
				xcp_data_set_completion ($$,
				                         XMMS_COLLPARSER_COMPLETE_UNARY_OPERATOR |
				                         XMMS_COLLPARSER_COMPLETE_OPEN_GROUP |
				                         XMMS_COLLPARSER_COMPLETE_UNIVERSE |
				                         XMMS_COLLPARSER_COMPLETE_FIELD |
				                         XMMS_COLLPARSER_COMPLETE_FILTER |
				                         XMMS_COLLPARSER_COMPLETE_VALUE |
				                         XMMS_COLLPARSER_COMPLETE_SEQUENCE);
				if (HAS_COLLECTION ($$)) {
					xcp_data_add_completion ($$, XMMS_COLLPARSER_COMPLETE_BINARY_OPERATOR);
				} else {
					$$->cant_close = 1;
				}
			}
			if (!$$->cant_close) {
				xcp_data_add_completion ($$, XMMS_COLLPARSER_COMPLETE_CLOSE_GROUP);
			}
			break;
		case XMMS_COLLPARSER_MODE_COMPILE:
			collparser->result = xmmsv_new_error ("')' expected.");
			xcp_yyerror (&@2, collparser, scanner, XCP_ERROR_INCOMPLETE);
		default:
			YYERROR;
		}
	}
|	TOKEN_OPERATOR_NOT TOKEN_EOF_OPERATOR {
		PRINT_R ("TOKEN_OPERATOR_NOT TOKEN_EOF_OPERATOR", "expr");
		switch (collparser->mode) {
		case XMMS_COLLPARSER_MODE_COMPLETE:
			$$ = xcp_data_new_eof ();
			xcp_data_set_completion ($$,
			                         XMMS_COLLPARSER_COMPLETE_UNARY_OPERATOR);
			xcp_data_set_complete_infos ($$, &@1, NULL, -1, "NOT");
			$$->stop_context = 1;
			$$->cant_close = 1;
			break;
		case XMMS_COLLPARSER_MODE_COMPILE:
			collparser->result = xmmsv_new_error ("Expression expected.");
			xcp_yyerror (&@2, collparser, scanner, XCP_ERROR_INCOMPLETE);
		default:
			YYERROR;
		}
	}
|	TOKEN_OPERATOR_NOT TOKEN_EOF {
		PRINT_R ("TOKEN_OPERATOR_NOT TOKEN_EOF", "expr");
		switch (collparser->mode) {
		case XMMS_COLLPARSER_MODE_COMPLETE:
			$$ = xcp_data_new_eof ();
			xcp_data_set_completion ($$,
			                         XMMS_COLLPARSER_COMPLETE_UNARY_OPERATOR |
			                         XMMS_COLLPARSER_COMPLETE_UNIVERSE |
			                         XMMS_COLLPARSER_COMPLETE_FIELD |
			                         XMMS_COLLPARSER_COMPLETE_FILTER |
			                         XMMS_COLLPARSER_COMPLETE_VALUE |
			                         XMMS_COLLPARSER_COMPLETE_SEQUENCE |
			                         XMMS_COLLPARSER_COMPLETE_OPEN_GROUP);
			$$->stop_context = 1;
			$$->cant_close = 1;
			break;
		case XMMS_COLLPARSER_MODE_COMPILE:
			collparser->result = xmmsv_new_error ("Expression expected.");
			xcp_yyerror (&@2, collparser, scanner, XCP_ERROR_INCOMPLETE);
		default:
			YYERROR;
		}
	}
;

string :
	TOKEN_STRING {
		PRINT_R ("TOKEN_STRING", "string");
		$$ = xcp_data_new_string ($1); /* data struct take over string ownership. */
	}
|	TOKEN_STRING TOKEN_EOF_STRING {
		PRINT_R ("TOKEN_STRING TOKEN_EOF_STRING", "string");
		$$ = xcp_data_new_string ($1);
		/* Ignored in compile mode. */
		$$->eof = 1;
	}
|	TOKEN_EOF_QUOTE {
		PRINT_R ("TOKEN_EOF_QUOTE", "string");
		switch (collparser->mode) {
		case XMMS_COLLPARSER_MODE_COMPLETE:
			$$ = xcp_data_new_string ($1);
			$$->eof = 1;
			$$->quote_eof = 1;
			$$->cant_close = 1;
			break;
		case XMMS_COLLPARSER_MODE_COMPILE:
			collparser->result = xmmsv_new_error ("Unclosed quote.");
			xcp_yyerror (&@1, collparser, scanner, XCP_ERROR_INCOMPLETE);
		default:
			YYERROR;
		}
	}
;

sequence :
	seqrange {
		PRINT_R ("seqrange", "sequence");
		$$ = $1;
	}
|	sequence TOKEN_ID_SEQUENCE seqrange {
		PRINT_R ("sequence TOKEN_ID_SEQUENCE seqrange", "sequence");
		if (HAS_EOF ($3)) { /* Happens only in completion mode. */
			xcp_data_free ($1);
			$$ = $3;
		} else {
			$$ = $1;
			xcp_data_merge_sequence ($3, $1);
			xcp_data_free ($3);
		}
	}
;

seqrange :
	id {
		PRINT_R ("id", "seqrange");
		int i = $1 < 0 ? -$1 - 1 : $1;
		$$ = xcp_data_new_sequence (i, i);
		if (collparser->mode == XMMS_COLLPARSER_MODE_COMPLETE && $1 < 0) {
			int n;
			char *buf;
			$$->eof = 1;
			xcp_data_set_completion ($$,
			                         XMMS_COLLPARSER_COMPLETE_ID);
			n = (@1).last_column - (@1).first_column + 1;
			buf = malloc (n);
			snprintf(buf, n, "%d", i);
			xcp_data_set_complete_infos ($$, &@1, NULL, -1, buf);
			free(buf);
		}
	}
|	id TOKEN_ID_RANGE id {
		PRINT_R ("id TOKEN_ID_RANGE id", "seqrange");
		int i = $3 < 0 ? -$3 - 1 : $3;
		$$ = xcp_data_new_sequence ($1, i);
		if (collparser->mode == XMMS_COLLPARSER_MODE_COMPLETE && $3 < 0) {
			int n;
			char *buf;
			$$->eof = 1;
			xcp_data_set_completion ($$,
			                         XMMS_COLLPARSER_COMPLETE_ID);
			n = (@3).last_column - (@3).first_column + 1;
			buf = malloc (n);
			snprintf(buf, n, "%d", i);
			xcp_data_set_complete_infos ($$, &@3, NULL, -1, buf);
			free(buf);
		}
	}
|	TOKEN_EOF_SEQUENCE {
		PRINT_R ("TOKEN_EOF_SEQUENCE", "seqrange");
		switch (collparser->mode) {
		case XMMS_COLLPARSER_MODE_COMPLETE:
			$$ = xcp_data_new_eof ();
			$$->cant_close = 1;
			xcp_data_set_completion ($$,
			                         XMMS_COLLPARSER_COMPLETE_ID);
			xcp_data_set_complete_infos ($$, &@1, NULL, -1, "");
			break;
		case XMMS_COLLPARSER_MODE_COMPILE:
			collparser->result = xmmsv_new_error ("Id expected.");
			xcp_yyerror (&@1, collparser, scanner, XCP_ERROR_INCOMPLETE);
		default:
			YYERROR;
		}
	}
|   id TOKEN_ID_RANGE TOKEN_EOF_SEQUENCE {
		PRINT_R ("id TOKEN_ID_RANGE TOKEN_EOF_SEQUENCE", "seqrange");
		switch (collparser->mode) {
		case XMMS_COLLPARSER_MODE_COMPLETE:
			$$ = xcp_data_new_eof ();
			$$->cant_close = 1;
			xcp_data_set_completion ($$,
			                         XMMS_COLLPARSER_COMPLETE_ID);
			xcp_data_set_complete_infos ($$, &@3, NULL, -1, "");
			break;
		case XMMS_COLLPARSER_MODE_COMPILE:
			collparser->result = xmmsv_new_error ("Id expected.");
			xcp_yyerror (&@1, collparser, scanner, XCP_ERROR_INCOMPLETE);
		default:
			YYERROR;
		}
	}
;

id :
	TOKEN_ID {
		PRINT_R ("TOKEN_ID", "id");
		$$ = $1;
	}
|   TOKEN_ID TOKEN_EOF_SEQUENCE {
		PRINT_R ("TOKEN_ID TOKEN_EOF_SEQUENCE", "id");
		$$ = -$1 - 1; /* add -1 to avoid annoying id 0. */
	}
;

group :
	TOKEN_GROUP_OPEN operation {
		PRINT_R ("TOKEN_GROUP_OPEN operation", "group");
		$$ = $2;
	}
|	TOKEN_GROUP_OPEN {
		PRINT_R ("TOKEN_GROUP_OPEN", "group");
		$$ = xcp_data_new ();
	}
;

filter :
	unary_filter {
		PRINT_R ("unary_filter", "filter");
		$$ = $1;
	}
|	binary_filter {
		PRINT_R ("binary_filter", "filter");
		$$ = $1;
	}
|	string {
		PRINT_R ("string", "filter");
		int set_coll = 0;
		switch (collparser->mode) {
		case XMMS_COLLPARSER_MODE_COMPLETE:
			if (HAS_EOF ($1)) {
				$$ = $1;
				xcp_data_set_completion ($$,
				                         XMMS_COLLPARSER_COMPLETE_FIELD |
				                         XMMS_COLLPARSER_COMPLETE_VALUE);
				if (!$$->quote_eof) {
					xcp_data_add_completion ($$, XMMS_COLLPARSER_COMPLETE_FILTER);
				}
				if (HAS_STRING ($1)) {
					xcp_data_set_complete_infos ($$, &@1, NULL, -1, $1->string);
				} else {
					xcp_data_set_complete_infos ($$, &@1, NULL, -1, "");
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
			xmmsv_t *c;
			if (HAS_STRING ($1)) {
				c = xcp_build_filter (XMMS_COLLECTION_TYPE_TOKEN, NULL, $1->string);
			} else {
				c = xcp_build_filter (XMMS_COLLECTION_TYPE_TOKEN, NULL, "");
			}
			$$ = xcp_data_new_coll (c);
			xmmsv_unref (c);
			xcp_data_free ($1);
		}
	}
;



unary_filter :
	unary_filop property {
		PRINT_R ("unary_filop property", "unary_filter");
		int set_coll = 0;
		switch (collparser->mode) {
		case XMMS_COLLPARSER_MODE_COMPLETE:
			if (HAS_EOF($2)) {
				$$ = xcp_data_new_eof();
				xcp_data_set_completion ($$, XMMS_COLLPARSER_COMPLETE_FIELD);
				xcp_data_set_complete_infos ($$, &@2, NULL, ($1).f, $2->string);
				$$->cant_close = $2->cant_close;
				$$->quote_eof = $$->quote_eof;
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
			xmmsv_t *c;
			c = xcp_build_filter (($1).f, $2->string, NULL);
			$$ = xcp_data_new_coll (c);
			xmmsv_unref (c);
		}
		xcp_data_free ($2);
	}
|	unary_filop TOKEN_EOF_FILTER {
		PRINT_R ("unary_filop TOKEN_EOF_FILTER", "unary_filter");
		switch (collparser->mode) {
		case XMMS_COLLPARSER_MODE_COMPLETE:
			$$ = xcp_data_new_eof ();
			xcp_data_set_completion ($$,
			                         XMMS_COLLPARSER_COMPLETE_FIELD);
			xcp_data_set_complete_infos ($$, &@2, NULL, ($1).f, "");
			$$->cant_close = 1;
			break;
		case XMMS_COLLPARSER_MODE_COMPILE:
			collparser->result = xmmsv_new_error ("Property name expected.");
			xcp_yyerror (&@2, collparser, scanner, XCP_ERROR_INCOMPLETE);
		default:
			YYERROR;
		}
	}
;

unary_filop :
	TOKEN_FILTER_HAS {
		PRINT_R ("TOKEN_FILTER_HAS", "unary_filop");
		$$.f = XMMS_COLLECTION_TYPE_HAS;
		$$.alt = 0;
	}
;

property :
	TOKEN_PROPERTY {
		PRINT_R ("TOKEN_PROPERTY", "property");
		if (strlen($1) == 0) { /* Cases like  +(  will reach this block. */
			collparser->result = xmmsv_new_error ("Property name empty.");
			xcp_yyerror (&@1, collparser, scanner, XCP_ERROR_INCOMPLETE);
			YYERROR;
		} else if (strlen($1) == 1) {
			$$ = xcp_data_new_string (xcp_build_property_from_short ($1));
			free ($1);
		} else {
			$$ = xcp_data_new_string ($1);
		}
	}
|	TOKEN_PROPERTY_EOF_QUOTE {
		PRINT_R ("TOKEN_PROPERTY_EOF_QUOTE", "property");
		switch (collparser->mode) {
		case XMMS_COLLPARSER_MODE_COMPLETE:
			$$ = xcp_data_new_string ($1);
			$$->eof = 1;
			$$->quote_eof = 1;
			$$->cant_close = 1;
			break;
		case XMMS_COLLPARSER_MODE_COMPILE:
			collparser->result = xmmsv_new_error ("Unclosed quote.");
			xcp_yyerror (&@1, collparser, scanner, XCP_ERROR_INCOMPLETE);
		default:
			YYERROR;
		}
	}
|	TOKEN_PROPERTY TOKEN_EOF_PROPERTY {
		PRINT_R ("TOKEN_PROPERTY TOKEN_PROPERTY_EOF", "property");
		switch (collparser->mode) {
		case XMMS_COLLPARSER_MODE_COMPLETE:
			$$ = xcp_data_new_string ($1);
			$$->eof = 1;
			break;
		case XMMS_COLLPARSER_MODE_COMPILE:
			if (strlen($1) == 0) {
				/* XXX Probably unreachable block. Check with scanner. */
				collparser->result = xmmsv_new_error ("Property name empty.");
				xcp_yyerror (&@1, collparser, scanner, XCP_ERROR_INCOMPLETE);
				YYERROR;
			} else if (strlen($1) == 1) {
				$$ = xcp_data_new_string (xcp_build_property_from_short ($1));
				free ($1);
			} else {
				$$ = xcp_data_new_string ($1);
			}
			break;
		default:
			YYERROR;
		}
	}
;

binary_filter :
	property binary_filop string {
		PRINT_R ("property binary_filop string", "binary_filter");
		int set_coll = 0;
		switch (collparser->mode) {
		case XMMS_COLLPARSER_MODE_COMPLETE:
			if (HAS_EOF ($3)) {
				$$ = xcp_data_new_eof ();
				xcp_data_set_completion ($$,
				                         XMMS_COLLPARSER_COMPLETE_VALUE);
				$$->quote_eof = $3->quote_eof;
				$$->cant_close = $3->cant_close;
				if (HAS_STRING ($3)) {
					xcp_data_set_complete_infos ($$, &@3, $1->string, ($2).f, $3->string);
				} else {
					xcp_data_set_complete_infos ($$, &@3, $1->string, ($2).f, "");
				}
				break;
			}
			/* If not EOF, same as compile mode */
		case XMMS_COLLPARSER_MODE_COMPILE:
			if (HAS_STRING ($3)) {
				set_coll = 1;
			} else {
				xcp_yyerror (&@3, collparser, scanner, XCP_ERROR_VALUE_EMPTY);
				YYERROR;
			}
			break;
		default:
			YYERROR;
		}
		if (set_coll) {
			xmmsv_t *c;
			char *str;
			str = _prepare_match ($3->string, $2.alt, !strcmp($1->string, "url"));
			c = xcp_build_filter (($2).f, $1->string, str);
			$$ = xcp_data_new_coll (c);
			xmmsv_unref (c);
			free (str);
		}
		xcp_data_free ($1);
		xcp_data_free ($3);
	}
|	binary_filop string {
		PRINT_R ("binary_filop string", "binary_filter");
		int set_coll = 0;
		switch (collparser->mode) {
		case XMMS_COLLPARSER_MODE_COMPLETE:
			if (HAS_EOF ($2)) {
				$$ = xcp_data_new_eof ();
				xcp_data_set_completion ($$,
				                         XMMS_COLLPARSER_COMPLETE_VALUE);
				$$->quote_eof = $2->quote_eof;
				$$->cant_close = $2->cant_close;
				if (HAS_STRING ($2)) {
					xcp_data_set_complete_infos ($$, &@2, NULL, ($1).f, $2->string);
				} else {
					xcp_data_set_complete_infos ($$, &@2, NULL, ($1).f, "");
				}
				break;
			}
			/* If not EOF, same as compile mode. */
		case XMMS_COLLPARSER_MODE_COMPILE:
			if (HAS_STRING ($2)) {
				set_coll = 1;
			} else {
				xcp_yyerror (&@2, collparser, scanner, XCP_ERROR_VALUE_EMPTY);
				YYERROR;
			}
			set_coll = 1;
			break;
		default:
			YYERROR;
		}
		if (set_coll) {
			xmmsv_t *c;
			char *str;
			str = _prepare_match ($2->string, $1.alt, 0);
			c = xcp_build_filter (($1).f, NULL, str);
			$$ = xcp_data_new_coll (c);
			xmmsv_unref (c);
			free (str);
		}
		xcp_data_free ($2);
	}
|	property binary_filop TOKEN_EOF_FILTER {
		PRINT_R ("property binary_filop TOKEN_EOF_FILTER", "binary_filter");
		switch (collparser->mode) {
		case XMMS_COLLPARSER_MODE_COMPLETE:
			$$ = xcp_data_new_eof ();
			xcp_data_set_completion ($$,
			                         XMMS_COLLPARSER_COMPLETE_VALUE);
			xcp_data_set_complete_infos ($$, &@3, $1->string, ($2).f, "");
			$$->cant_close = 1;
			xcp_data_free ($1);
			break;
		case XMMS_COLLPARSER_MODE_COMPILE:
			collparser->result = xmmsv_new_error ("Value expected.");
			xcp_yyerror (&@3, collparser, scanner, XCP_ERROR_INCOMPLETE);
		default:
			YYERROR;
		}
	}
|	binary_filop TOKEN_EOF_FILTER {
		PRINT_R ("binary_filop TOKEN_EOF_FILTER", "binary_filter");
		switch (collparser->mode) {
		case XMMS_COLLPARSER_MODE_COMPLETE:
			$$ = xcp_data_new_eof ();
			xcp_data_set_completion ($$,
			                         XMMS_COLLPARSER_COMPLETE_VALUE);
			xcp_data_set_complete_infos ($$, &@2, NULL, ($1).f, "");
			$$->cant_close = 1;
			break;
		case XMMS_COLLPARSER_MODE_COMPILE:
			collparser->result = xmmsv_new_error ("Value expected.");
			xcp_yyerror (&@2, collparser, scanner, XCP_ERROR_INCOMPLETE);
		default:
			YYERROR;
		}
	}
;

binary_filop :
	TOKEN_FILTER_EQUAL {
		PRINT_R ("TOKEN_FILTER_EQUAL", "binary_filop");
		$$.f = XMMS_COLLECTION_TYPE_EQUALS;
		$$.alt = 0;
	}
|	TOKEN_FILTER_MATCH_GLOB {
		PRINT_R ("TOKEN_FILTER_MATCH_GLOB", "binary_filop");
		$$.f = XMMS_COLLECTION_TYPE_MATCH;
		$$.alt = 1;
	}
|	TOKEN_FILTER_MATCH {
		PRINT_R ("TOKEN_FILTER_MATCH", "binary_filop");
		$$.f = XMMS_COLLECTION_TYPE_MATCH;
		$$.alt = 0;
	}
|	TOKEN_FILTER_TOKEN {
		PRINT_R ("TOKEN_FILTER_TOKEN", "binary_filop");
		$$.f = XMMS_COLLECTION_TYPE_TOKEN;
		$$.alt = 0;
	}
|	TOKEN_FILTER_LESS {
		PRINT_R ("TOKEN_FILTER_LESS", "binary_filop");
		$$.f = XMMS_COLLECTION_TYPE_SMALLER;
		$$.alt = 0;
	}
|	TOKEN_FILTER_LESSEQUAL {
		PRINT_R ("TOKEN_FILTER_LESSEQUAL", "binary_filop");
		$$.f = XMMS_COLLECTION_TYPE_SMALLEREQ;
		$$.alt = 0;
	}
|	TOKEN_FILTER_GREATER {
		PRINT_R ("TOKEN_FILTER_GREATER", "binary_filop");
		$$.f = XMMS_COLLECTION_TYPE_GREATER;
		$$.alt = 0;
	}
|	TOKEN_FILTER_GREATEREQUAL {
		PRINT_R ("TOKEN_FILTER_GREATEREQUAL", "binary_filop");
		$$.f = XMMS_COLLECTION_TYPE_GREATEREQ;
		$$.alt = 0;
	}
;

%%

void
xcp_parse (xmmsv_collparser_t *collparser, void *scanner)
{
	xcp_yyparse (collparser, scanner);
}

static char *
_prepare_match (const char *s, int alt, int is_url)
{
	char *enc_s = NULL;
	char *str;
	int n;
	n = strlen(s);

	if (alt == 1 && is_url) {
		enc_s = xmmsv_encode_url (s);
		s = enc_s;
	}

	if (alt == 0 || (s[0] == '*' && s[n-1] == '*')) {
		str = xcp_strdup_safe (s);
	} else if (s[0] == '*') {
		str = xcp_strjoin (s, "*", NULL);
	} else if (s[n-1] == '*') {
		str = xcp_strjoin ("*", s, NULL);
	} else {
		str = xcp_strjoin ("*", s, "*", NULL);
	}

	if (enc_s) {
		free (enc_s);
	}

	return str;
}

void xcp_yyerror (YYLTYPE *locp, xmmsv_collparser_t *collparser, void *scanner, const char *msg)
{
	/* TODO: Format error. */
	collparser->result = xmmsv_new_error (msg);
}

