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

%option reentrant
%option bison-bridge
%option bison-locations
%option noyywrap
%option nounput
%option noinput
%option prefix="xcp_yy"
%option extra-type="xmmsv_collparser_t *"

%{
#include <stdio.h>
#include "utils_priv.h"
#include "collparser_priv.h"
#include "parser.tab.h"

#ifdef YY_BREAK
# undef YY_BREAK
#endif
#define YY_BREAK

/* #define SC_DEBUG printf */
#define SC_DEBUG(x, ...) (void)(0)

#define BEGIN_STR(orig,tgt) do { \
	switch (YY_START) { \
		case prop##orig: \
			BEGIN (prop##tgt); \
			break; \
		case strprop##orig: \
			BEGIN (strprop##tgt); \
			break; \
		case string##orig: \
			BEGIN (string##tgt); \
			break; \
	} \
} while (0)

static void _start_loc (YYLTYPE *llocp);
static void _update_loc (YYLTYPE *llocp, const char *data);
static void _set_loc (YYLTYPE *llocp, const char *data);

%}

%x binaryFilter
%x checkOP
%x idseq
%x opEOF
%x parse
%x prop
%x propEOF
%x propFIL
%x propINIT
%x propQ
%x propSQ
%x string
%x stringEOF
%x stringFIL
%x stringINIT
%x stringQ
%x stringSQ
%x strprop
%x strpropEOF
%x strpropFIL
/* ^^^ Unused. Only to avoid error in BEGIN_STR macros. */
%x strpropINIT
%x strpropQ
%x strpropSQ
%x unaryFilter
%x universeEOF

SP         [ \t\n\r]
SPG        [ \t\n\r()]
SPOG       [ \t\n\r(]
QUOTE      ["]
SQUOTE     [']
STRQ       ("\\".|[^"])*
STRSQ      ("\\".|[^'])*
FILTER     (<=|>=|[:~<>=^])
NFILTERSPGQ [^:~<>= \t\n\r"'^()]

%%

<INITIAL>{
	<<EOF>> {
		if (!yyextra->scanner_context->started) {
			yyextra->scanner_context->started = 1;
			BEGIN (parse);
			break;
		}
		yyterminate ();
		break;
	}
	""/. {
		yyextra->scanner_context->started = 1;
		BEGIN (parse);
		break;
	}
}

<parse>{
	<<EOF>> {
		_start_loc (yylloc);
		/* Ensure an EOF is emitted for each openned sub group. */
		if (xcp_scanner_dec_group (yyextra) <= 0) {
			BEGIN (INITIAL);
		}
		return TOKEN_EOF;
	}
	{SP}+ {
		_set_loc (yylloc, yytext);
		break;
	}
	"(" {
		_set_loc (yylloc, yytext);
		yylval->group = xcp_scanner_inc_group (yyextra);
		return TOKEN_GROUP_OPEN;
	}
	")" {
		_set_loc (yylloc, yytext);
		yylval->group = xcp_scanner_dec_group (yyextra);
		return TOKEN_GROUP_CLOSE;
	}
	"#" {
		_set_loc (yylloc, yytext);
		BEGIN (idseq);
		return TOKEN_SYMBOL_ID;
	}
	"+" {
		_set_loc (yylloc, yytext);
		BEGIN (unaryFilter);
		return TOKEN_FILTER_HAS;
	}
	"=" {
		_set_loc (yylloc, yytext);
		BEGIN (binaryFilter);
		return TOKEN_FILTER_EQUAL;
	}
	"~" {
		_set_loc (yylloc, yytext);
		BEGIN (binaryFilter);
		return TOKEN_FILTER_MATCH_GLOB;
	}
	":" {
		_set_loc (yylloc, yytext);
		BEGIN (binaryFilter);
		return TOKEN_FILTER_MATCH;
	}
	"^" {
		_set_loc (yylloc, yytext);
		BEGIN (binaryFilter);
		return TOKEN_FILTER_TOKEN;
	}
	"<=" {
		_set_loc (yylloc, yytext);
		BEGIN (binaryFilter);
		return TOKEN_FILTER_LESSEQUAL;
	}
	"<" {
		_set_loc (yylloc, yytext);
		BEGIN (binaryFilter);
		return TOKEN_FILTER_LESS;
	}
	">=" {
		_set_loc (yylloc, yytext);
		BEGIN (binaryFilter);
		return TOKEN_FILTER_GREATEREQUAL;
	}
	">" {
		_set_loc (yylloc, yytext);
		BEGIN (binaryFilter);
		return TOKEN_FILTER_GREATER;
	}
	"OR"/{SPOG} {
		_set_loc (yylloc, yytext);
		return TOKEN_OPERATOR_OR;
	}
	"AND"/{SPOG} {
		_set_loc (yylloc, yytext);
		return TOKEN_OPERATOR_AND;
	}
	"NOT"/{SPOG} {
		_set_loc (yylloc, yytext);
		return TOKEN_OPERATOR_NOT;
	}
	"in:" {
		_set_loc (yylloc, yytext);
		BEGIN (stringINIT);
		return TOKEN_REFERENCE;
	}
	[*]/{SPG} {
		_set_loc (yylloc, yytext);
		return TOKEN_UNIVERSE;
	}
	""/. {
		BEGIN (checkOP);
		break;
	}
}

<checkOP>{
	""/("OR"|"AND"|"NOT"|"*"). { /* ; can't be a space here. */
		BEGIN (strpropINIT);
		break;
	}
	"OR" { /* Matched only if followed by EOF */
		_set_loc (yylloc, yytext);
		BEGIN (opEOF);
		return TOKEN_OPERATOR_OR;
	}
	"AND" { /* Matched only if followed by EOF */
		_set_loc (yylloc, yytext);
		BEGIN (opEOF);
		return TOKEN_OPERATOR_AND;
	}
	"NOT" { /* Matched only if followed by EOF */
		_set_loc (yylloc, yytext);
		BEGIN (opEOF);
		return TOKEN_OPERATOR_NOT;
	}
	"*" { /* Matched only if followed by EOF */
		_set_loc (yylloc, yytext);
		BEGIN (universeEOF);
		return TOKEN_UNIVERSE;
	}
	""/. {
		BEGIN (strpropINIT);
		break;
	}
}

<universeEOF>{
	<<EOF>> {
		_start_loc (yylloc);
		BEGIN (parse);
		return TOKEN_EOF_UNIVERSE;
	}
}

<opEOF>{
	<<EOF>> {
		_start_loc (yylloc);
		BEGIN (parse);
		return TOKEN_EOF_OPERATOR;
	}
}

<unaryFilter>{
	<<EOF>> {
		_start_loc (yylloc);
		BEGIN (parse);
		return TOKEN_EOF_FILTER;
	}
	""/. {
		BEGIN (propINIT);
		break;
	}
}

<binaryFilter>{
	<<EOF>> {
		_start_loc (yylloc);
		BEGIN (parse);
		return TOKEN_EOF_FILTER;
	}
	""/. {
		BEGIN (stringINIT);
		break;
	}
}

<idseq>{
	<<EOF>> {
		_start_loc (yylloc);
		BEGIN (parse);
		return TOKEN_EOF_SEQUENCE;
	}
	[[:digit:]]+ {
		_set_loc (yylloc, yytext);
		yylval->ival = atoi (yytext);
		return TOKEN_ID;
	}
	"-" {
		_set_loc (yylloc, yytext);
		return TOKEN_ID_RANGE;
	}
	"," {
		_set_loc (yylloc, yytext);
		return TOKEN_ID_SEQUENCE;
	}
	""/{SPG} {
		BEGIN (parse);
		break;
	}
}

<strpropINIT,propINIT,stringINIT>{
	<<EOF>> { /* Initialize the string buffer. */
		_start_loc (yylloc);
		xcp_scanner_string_buffer_init (yyextra);
		BEGIN_STR (INIT,);
		break;
	}
	""/{SPG} { /* Prevent odd side-effects. */
		BEGIN (parse);
		return TOKEN_ERR;
	}
	""/. { /* Initialize the string buffer. and select next context */
		_start_loc (yylloc);
		xcp_scanner_string_buffer_init (yyextra);
		BEGIN_STR (INIT,);
		break;
	}
}

<propFIL,stringFIL>{
	{FILTER}+ {
		_update_loc (yylloc, yytext);
		xcp_scanner_string_buffer_append (yyextra, yytext);
		BEGIN_STR (FIL,);
		break;
	}
}

<prop,strprop,string>{
	<<EOF>> {
		int start = YY_START;
		BEGIN_STR (,EOF);
		xcp_scanner_string_buffer_unescape (yyextra);
		yylval->str = xcp_scanner_string_buffer_steal (yyextra);
		if (start == prop) {
			return TOKEN_PROPERTY;
		} else {
			return TOKEN_STRING;
		}
	}
	{QUOTE} {
		_update_loc (yylloc, yytext);
		BEGIN_STR (,Q);
		break;
	}
	{SQUOTE} {
		_update_loc (yylloc, yytext);
		BEGIN_STR (,SQ);
		break;
	}
	("\\".|{NFILTERSPGQ})+ {
		_update_loc (yylloc, yytext);
		xcp_scanner_string_buffer_append (yyextra, yytext);
		break;
	}
	""/{FILTER} {
		if (YY_START == strprop) {
			BEGIN (parse);
			xcp_scanner_string_buffer_unescape (yyextra);
			yylval->str = xcp_scanner_string_buffer_steal (yyextra);
			return TOKEN_PROPERTY;
		} else {
			BEGIN_STR (,FIL);
		}
		break;
	}
	""/{SPG} {
		int start = YY_START;
		BEGIN (parse);
		xcp_scanner_string_buffer_unescape (yyextra);
		yylval->str = xcp_scanner_string_buffer_steal (yyextra);
		if (start == prop) {
			return TOKEN_PROPERTY;
		} else {
			return TOKEN_STRING;
		}
	}
}

<propEOF,strpropEOF,stringEOF>{
	<<EOF>> {
		/* Special case for completion mode:
		 *   Compile mode requires TOKEN_EOF, but it must not be intercepted before
		 *   all reductions are done. On the other end, Completion mode needs to
		 *   know the input has no trailing spaces to detect whether the string
		 *   need completion. We return a special EOF token for this purpose,
		 *   and then return the usual TOKEN_EOF. */
		_start_loc (yylloc);
		int start = YY_START;
		BEGIN (parse);
		switch (start) {
			case propEOF:
				return TOKEN_EOF_PROPERTY;
			default:
				return TOKEN_EOF_STRING;
		}
	}
}

<propQ,strpropQ,stringQ,propSQ,strpropSQ,stringSQ>{
	<<EOF>> {
		xcp_scanner_string_buffer_unescape (yyextra);
		yylval->str = xcp_scanner_string_buffer_steal (yyextra);

		/* Malformed pattern: Need closing quote. */
		int start = YY_START;
		BEGIN (parse);
		switch (start) {
			case propQ:
			case propSQ:
				return TOKEN_PROPERTY_EOF_QUOTE;
			case strpropQ:
			case stringQ:
			case strpropSQ:
			case stringSQ:
				return TOKEN_EOF_QUOTE;
			default:
				return TOKEN_ERR;
		}
	}
}

<propQ,strpropQ,stringQ>{
	{QUOTE} {
		_update_loc (yylloc, yytext);
		BEGIN_STR(Q,);
		break;
	}
	{STRQ}* {
		_update_loc (yylloc, yytext);
		xcp_scanner_string_buffer_append (yyextra, yytext);
		break;
	}
}

<propSQ,strpropSQ,stringSQ>{
	{SQUOTE} {
		_update_loc (yylloc, yytext);
		BEGIN_STR(SQ,);
		break;
	}
	{STRSQ}* {
		_update_loc (yylloc, yytext);
		xcp_scanner_string_buffer_append (yyextra, yytext);
		break;
	}
}

<*>. {
	_set_loc (yylloc, yytext);
	printf (">>> %s <<<\n", yytext); /* XXX Should never be reached ? */
	return TOKEN_ERR;
}

%%

static void
_start_loc (YYLTYPE *llocp)
{
	llocp->first_line = llocp->last_line;
	llocp->first_column = llocp->last_column;
}

static void
_update_loc (YYLTYPE *llocp, const char *data)
{
	const char *p;
	while ((p = strchr (data, '\n')) != NULL) {
		data = p+1;
		llocp->last_line++;
		llocp->last_column = 1;
	}
	llocp->last_column += strlen(data);
}

static void
_set_loc (YYLTYPE *llocp, const char *data)
{
	_start_loc (llocp);
	_update_loc (llocp, data);
}

void
xcp_scanner_context_init (xmmsv_collparser_t *collparser, const char *input)
{
	xcp_scanner_context_t *ctx;

	/* Clear the previous scanner context. This is required because we can't
	 * reset the internal state of an existing scanner. */
	xcp_scanner_context_free (collparser);

	ctx = collparser->scanner_context = xcp_new0 (xcp_scanner_context_t, 1);

	yylex_init_extra (collparser, (yyscan_t *)&(ctx->scanner));
	ctx->buffer = xcp_yy_scan_string (input, (yyscan_t) ctx->scanner);
}

void
xcp_scanner_context_free (xmmsv_collparser_t *collparser)
{
	xcp_scanner_context_t *ctx = collparser->scanner_context;

	if (ctx) {
		if (ctx->scanner) {
			if (ctx->buffer) {
				xcp_yy_delete_buffer ((YY_BUFFER_STATE) ctx->buffer,
				                      (yyscan_t) ctx->scanner);
			}
			xcp_yylex_destroy ((yyscan_t) ctx->scanner);
		}
		free (ctx);
	}
}
