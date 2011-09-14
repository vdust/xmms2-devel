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

#include <stdio.h>
#include <stdarg.h>

#include "utils_priv.h"

char *
xcp_strjoin (const char *str, ...)
{
	int n = 0;
	char *ret = NULL;
	char *p;
	const char *s;
	va_list ap;
	va_start (ap, str);
	s = str;
	while (s) {
		n += strlen (s);
		s = va_arg (ap, const char *);
	}
	va_end (ap);
	if (n) {
		p = ret = xcp_new0 (char, n + 1);
		va_start (ap, str);
		s = str;
		while (s) {
			n = strlen (s);
			p = strncat (p, s, n) + n;
			s = va_arg (ap, const char *);
		}
		va_end (ap);
	}
	return ret;
}
