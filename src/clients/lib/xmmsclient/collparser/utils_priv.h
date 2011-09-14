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

#ifndef __XMMS_COLLPARSER_UTILS_PRIV_H__
#define __XMMS_COLLPARSER_UTILS_PRIV_H__

#include <stdlib.h>
#include <string.h>

#define xcp_new0(type, c) ((type *) calloc((c), sizeof (type)))
#define xcp_new(type, c) ((type *) malloc((c) * sizeof (type)))
#define xcp_renew(buf, type, c) ((type *) realloc(buf, (c) * sizeof (type)))

static inline char *
xcp_strdup_safe (const char *p)
{
	char *dup = NULL;
	if (p) {
		dup = strdup (p);
	}
	return dup;
}

char *xcp_strjoin (const char *str, ...);

#endif /* __XMMS_COLLPARSER_UTILS_PRIV_H__ */
