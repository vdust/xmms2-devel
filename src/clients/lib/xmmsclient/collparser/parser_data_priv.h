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

#ifndef __XMMS_COLLPARSER_DATA_PRIV_H__
#define __XMMS_COLLPARSER_DATA_PRIV_H__

#include "collparser_priv.h"
#include "parser.tab.h"

typedef struct xcp_data_complete_infos_St xcp_data_complete_infos_t;
struct xcp_data_complete_infos_St
{
	YYLTYPE lloc;             /* Completion info: location in the input. */
	char *field;              /* Completion info: property name. */
	xmmsv_coll_type_t filter; /* Completion info: filter name. */
	char *value;              /* Completion info: value to complete. */
};

struct xcp_data_St
{
	xmmsv_t *collection; /* A collection */
	xmmsv_t *sequence;        /* A list of ids. */
	char *string;             /* A (re|m|c)alloc()ed string. */

	/* xmmsv_t *completion; */
	xmmsv_collparser_complete_t completion;
	xcp_data_complete_infos_t *complete_infos;

	xmmsv_t *error;           /* An error message */

	short group;              /* Nested groups. TODO */
	int eof:1;                /* Whether EOF occured. */
	int quote_eof:1;          /* Whether EOF occured in opened quotes. */
	int cant_close:1;          /* Whether an open group can close. */
	int stop_context:1;       /* Whether to apply 'AND' on eof in completion mode. */
};

xcp_data_t *xcp_data_new (void);
xcp_data_t *xcp_data_new_eof (void);
void xcp_data_free (xcp_data_t *data);
xcp_data_t *xcp_data_new_coll (xmmsv_t *coll);
void xcp_data_set_coll (xcp_data_t *data, xmmsv_t *coll);
xcp_data_t *xcp_data_new_sequence (unsigned int min, unsigned int max);
void xcp_data_merge_sequence (xcp_data_t *src, xcp_data_t *dest);
xcp_data_t *xcp_data_new_string (char *string);
void xcp_data_set_string (xcp_data_t *data, char *string);
/* xcp_data_t *xcp_data_new_filter (const char *filter); */
void xcp_data_set_error (xcp_data_t *data, const char *errstr);
void xcp_data_copy_error (xcp_data_t *src, xcp_data_t *dest);
void xcp_data_set_completion (xcp_data_t *data, xmmsv_collparser_complete_t flags);
void xcp_data_add_completion (xcp_data_t *data, xmmsv_collparser_complete_t flags);
void xcp_data_merge_completion (xcp_data_t *src, xcp_data_t *dest);
void xcp_data_clear_completion (xcp_data_t *data);
void xcp_data_set_complete_infos (xcp_data_t *data, YYLTYPE *lloc, const char *field, xmmsv_coll_type_t filter, const char *value);
void xcp_data_copy_complete_infos (xcp_data_t *src, xcp_data_t *dest);

xmmsv_t *xcp_data_build_result (xcp_data_t *data, xmmsv_collparser_mode_t mode);

#define HAS_EOF(data) (data->eof)
#define MATCH_EOF(data, eoftype) (data->eof == eoftype)
#define HAS_COLLECTION(data) (data->collection != NULL)
#define APPLY_OPERATOR(data) (HAS_COLLECTION (data) && !data->stop_context)
#define NEED_CONTEXT(data) (!HAS_COLLECTION (data) && !data->stop_context)
#define HAS_COMPLETION(data) (data->completion != XMMS_COLLPARSER_COMPLETE_NONE)
#define HAS_SEQUENCE(data) (data->sequence != NULL)
#define HAS_STRING(data) (data->string != NULL)

#endif /* __XMMS_COLLPARSER_DATA_PRIV_H__ */
