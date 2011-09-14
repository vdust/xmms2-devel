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

#ifndef __XMMS_COLLPARSER_COLLBUILDER_PRIV_H__
#define __XMMS_COLLPARSER_COLLBUILDER_PRIV_H__

#include "collparser_priv.h"

xmmsv_t *xcp_build_union (xmmsv_t *op1, xmmsv_t *op2);
xmmsv_t *xcp_build_intersection (xmmsv_t *op1, xmmsv_t *op2);
xmmsv_t *xcp_build_idlist (xmmsv_t *sequence);
xmmsv_t *xcp_build_complement (xmmsv_t *op);
xmmsv_t *xcp_build_reference (const char *collection);
xmmsv_t *xcp_build_universe (void);
xmmsv_t *xcp_build_filter (xmmsv_coll_type_t filter, const char *field, const char *value);
char *xcp_build_property_from_short (const char *_short);

#endif /* __XMMS_COLLPARSER_COLLBUILDER_PRIV_H__ */

