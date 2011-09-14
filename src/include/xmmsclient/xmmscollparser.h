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

#ifndef __XMMS_COLLPARSER_H__
#define __XMMS_COLLPARSER_H__

#include "xmmsc/xmmsv.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct xmmsv_collparser_St xmmsv_collparser_t;

typedef enum {
	XMMS_COLLPARSER_MODE_COMPILE = 0,
	XMMS_COLLPARSER_MODE_COMPLETE
} xmmsv_collparser_mode_t;

typedef enum {
	XMMS_COLLPARSER_COMPLETE_NONE = 0,
	XMMS_COLLPARSER_COMPLETE_BINARY_OPERATOR = 1 << 0,
	XMMS_COLLPARSER_COMPLETE_UNARY_OPERATOR = 1 << 1,
	XMMS_COLLPARSER_COMPLETE_FIELD = 1 << 2,
	XMMS_COLLPARSER_COMPLETE_FILTER = 1 << 3,
	XMMS_COLLPARSER_COMPLETE_VALUE = 1 << 4,
	XMMS_COLLPARSER_COMPLETE_REFERENCE = 1 << 5,
	XMMS_COLLPARSER_COMPLETE_UNIVERSE = 1 << 6,
	XMMS_COLLPARSER_COMPLETE_OPEN_GROUP = 1 << 7,
	XMMS_COLLPARSER_COMPLETE_CLOSE_GROUP = 1 << 8,
	XMMS_COLLPARSER_COMPLETE_SEQUENCE = 1 << 9,
	XMMS_COLLPARSER_COMPLETE_ID = 1 << 10
} xmmsv_collparser_complete_t;

xmmsv_collparser_t *xmmsv_collparser_new (void);
xmmsv_t *xmmsv_collparser_parse (xmmsv_collparser_t *collparser, xmmsv_collparser_mode_t mode, const char *pattern);
void xmmsv_collparser_free (xmmsv_collparser_t *collparser);


/* Convenience functions */
xmmsv_t *xmmsv_collparser_compile (const char *pattern);
xmmsv_t *xmmsv_collparser_complete (const char *pattern);

#ifdef __cplusplus
}
#endif

#endif /* __XMMS_COLLPARSER_H__ */
