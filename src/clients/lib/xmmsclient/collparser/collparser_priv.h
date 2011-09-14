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

#ifndef __XMMS_COLLPARSER_PRIV_H__
#define __XMMS_COLLPARSER_PRIV_H__

#include "xmmsclient/xmmscollparser.h"

typedef struct xcp_scanner_context_St xcp_scanner_context_t;
/* typedefs required by parser.tab.h */
typedef struct xcp_data_St xcp_data_t;

struct xmmsv_collparser_St
{
	xcp_scanner_context_t *scanner_context;
	xmmsv_t *result;
	xmmsv_collparser_mode_t mode;
};

struct xcp_scanner_context_St
{
	void *scanner;
	void *buffer;
	int started;
	int group;
	char *string;
	size_t str_len;
};

/* in scanner.l */
void xcp_scanner_context_init (xmmsv_collparser_t *collparser, const char *input);
void xcp_scanner_context_free (xmmsv_collparser_t *collparser);

/* in parser.y */
void xcp_parse (xmmsv_collparser_t *collparser, void *scanner);

/* in collparser.c */
void xcp_scanner_string_buffer_init (xmmsv_collparser_t *collparser);
void xcp_scanner_string_buffer_append (xmmsv_collparser_t *collparser, const char *s);
void xcp_scanner_string_buffer_unescape (xmmsv_collparser_t *collparser);
char *xcp_scanner_string_buffer_steal (xmmsv_collparser_t *collparser);

int xcp_scanner_inc_group (xmmsv_collparser_t *collparser);
int xcp_scanner_dec_group (xmmsv_collparser_t *collparser);

#endif /* __XMMS_COLLPARSER_PRIV_H__ */
