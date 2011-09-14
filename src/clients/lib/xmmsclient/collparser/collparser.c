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

#include "utils_priv.h"
#include "collparser_priv.h"
#include "parser_data_priv.h"


/* Local API */

static void _collparser_reset (xmmsv_collparser_t *collparser);

static void
_collparser_reset (xmmsv_collparser_t *collparser)
{
	xcp_scanner_context_free (collparser);
	if (collparser->result) {
		xmmsv_unref (collparser->result);
		collparser->result = NULL;
	}
}


/* Private API */

void
xcp_scanner_string_buffer_init (xmmsv_collparser_t *collparser)
{
	xcp_scanner_context_t *ctx = (xcp_scanner_context_t *) collparser->scanner_context;

	if (ctx->string) {
		free (ctx->string);
		ctx->string = NULL;
	}
	ctx->str_len = 0;
}

void
xcp_scanner_string_buffer_append (xmmsv_collparser_t *collparser, const char *s)
{
	xcp_scanner_context_t *ctx = collparser->scanner_context;
	int n;

	if (s) {
		n = strlen (s);
		if (!ctx->string) {
			ctx->string = xcp_strdup_safe (s);
			ctx->str_len = n;
		} else {
			ctx->string = xcp_renew (ctx->string, char, ctx->str_len + n + 1);
			strncat (ctx->string, s, n);
			ctx->str_len += n;
		}
	}
}

void
xcp_scanner_string_buffer_unescape (xmmsv_collparser_t *collparser)
{
	xcp_scanner_context_t *ctx = collparser->scanner_context;
	char *pw; /* write pointer. */
	char *pr; /* read pointer */

	if (ctx->string) {
		for (pr = pw = ctx->string; *pr; ++pr) {
			if (*pr == '\\') {
				++pr;
				if (*pr) {
					*pw = *pr;
					++pw;
				}
			} else {
				*pw = *pr;
				++pw;
			}
		}
		*pw = *pr; /* copy '\0' */
		ctx->str_len = strlen(ctx->string);
	}
}

char *
xcp_scanner_string_buffer_steal (xmmsv_collparser_t *collparser)
{
	xcp_scanner_context_t *ctx = collparser->scanner_context;
	char *s;

	s = ctx->string;
	ctx->string = NULL;
	ctx->str_len = 0;

	return s;
}

/* Increment group counter and return the new value. */
int
xcp_scanner_inc_group (xmmsv_collparser_t *collparser)
{
	return ++collparser->scanner_context->group;
}

/* Decrement group counter and return the value before decrement. */
int
xcp_scanner_dec_group (xmmsv_collparser_t *collparser)
{
	return collparser->scanner_context->group--;
}


/* Public API */

xmmsv_collparser_t *
xmmsv_collparser_new ()
{
	return xcp_new0 (xmmsv_collparser_t, 1);
}

xmmsv_t *
xmmsv_collparser_parse (xmmsv_collparser_t *collparser, xmmsv_collparser_mode_t mode, const char *pattern)
{
	xmmsv_t *res;

	_collparser_reset (collparser);
	collparser->mode = mode;
	xcp_scanner_context_init (collparser, pattern);
	xcp_parse (collparser, collparser->scanner_context->scanner);
	res = collparser->result;
	collparser->result = NULL;

	return res;
}

void
xmmsv_collparser_free (xmmsv_collparser_t *collparser)
{
	_collparser_reset (collparser);
	free (collparser);
}

xmmsv_t *
xmmsv_collparser_compile (const char *pattern)
{
	xmmsv_collparser_t *cp;
	xmmsv_t *res;

	cp = xmmsv_collparser_new ();
	res = xmmsv_collparser_parse (cp, XMMS_COLLPARSER_MODE_COMPILE, pattern);
	xmmsv_collparser_free (cp);
	return res;
}

xmmsv_t *
xmmsv_collparser_complete (const char *pattern)
{
	xmmsv_collparser_t *cp;
	xmmsv_t *res;

	cp = xmmsv_collparser_new ();
	res = xmmsv_collparser_parse (cp, XMMS_COLLPARSER_MODE_COMPLETE, pattern);
	xmmsv_collparser_free (cp);
	return res;
}
