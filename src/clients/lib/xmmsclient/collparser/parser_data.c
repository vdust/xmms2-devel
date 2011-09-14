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

#include <stdarg.h>
#include <stdio.h>

#include "parser_data_priv.h"
#include "utils_priv.h"

static xcp_data_complete_infos_t *
_data_complete_infos_new (YYLTYPE *lloc, const char *field, xmmsv_coll_type_t filter,
                          const char *value)
{
	xcp_data_complete_infos_t *infos;
	infos = xcp_new (xcp_data_complete_infos_t, 1);
	infos->lloc = *lloc;
	infos->field = xcp_strdup_safe (field);
	infos->filter = filter;
	infos->value = xcp_strdup_safe (value);
	return infos;
}

static void
_data_complete_infos_free (xcp_data_complete_infos_t *ci)
{
	if (ci) {
		free (ci->field);
		free (ci->value);
		free (ci);
	}
}

xcp_data_t *
xcp_data_new (void)
{
	return xcp_new0 (xcp_data_t, 1);
}

xcp_data_t *
xcp_data_new_eof (void)
{
	xcp_data_t *data;

	data = xcp_data_new ();
	data->eof = 1;

	return data;
}

#define _xv_unref(xv) if (xv) { xmmsv_unref (xv); } else ((void)0)
void
xcp_data_free (xcp_data_t *data)
{
	if (data) {
		if (data->collection) {
			xmmsv_unref (data->collection);
		}
		_xv_unref (data->sequence);
		if (data->string) {
			free (data->string);
		}
		/* _xv_unref (data->completion); */
		_data_complete_infos_free (data->complete_infos);
		_xv_unref (data->error);
		free (data);
	}
}

xcp_data_t *
xcp_data_new_coll (xmmsv_t *coll)
{
	xcp_data_t *data;

	data = xcp_data_new ();
	data->collection = xmmsv_ref (coll);
	return data;
}

void
xcp_data_set_coll (xcp_data_t *data, xmmsv_t *coll)
{
	if (data->collection) {
		xmmsv_unref (data->collection);
	}
	data->collection = xmmsv_ref (coll);
}

xcp_data_t *
xcp_data_new_sequence (unsigned int start, unsigned int end)
{
	xmmsv_t *seq;
	xcp_data_t *data;

	data = xcp_data_new ();
	seq = xmmsv_new_list ();
	for (; start <= end; ++start) {
		xmmsv_list_append_int (seq, start);
	}
	data->sequence = seq;
	return data;
}

static void
_xvalue_append_int_list (xmmsv_t *src, xmmsv_t *dest)
{
	xmmsv_list_iter_t *it;
	int32_t val;
	if (xmmsv_get_list_iter (src, &it)) {
		for (; xmmsv_list_iter_valid (it); xmmsv_list_iter_next (it)) {
			if (xmmsv_list_iter_entry_int (it, &val)) {
				xmmsv_list_append_int (dest, val);
			}
		}
	}
}

void
xcp_data_merge_sequence (xcp_data_t *src, xcp_data_t *dest)
{
	if (!dest->sequence) {
		dest->sequence = xmmsv_new_list ();
	}
	if (src->sequence) {
		_xvalue_append_int_list (src->sequence, dest->sequence);
	}
}

xcp_data_t *
xcp_data_new_string (char *string)
{
	xcp_data_t *data;

	data = xcp_data_new ();
	data->string = string;

	return data;
}

void
xcp_data_set_string (xcp_data_t *data, char *string)
{
	if (data->string) {
		free (data->string);
	}
	data->string = string;
}

void
xcp_data_set_error (xcp_data_t *data, const char *errstr)
{
	if (data->error) {
		xmmsv_unref (data->error);
	}
	data->error = xmmsv_new_error (errstr);
}

void
xcp_data_copy_error (xcp_data_t *src, xcp_data_t *dest)
{
	if (dest->error) {
		xmmsv_unref (dest->error);
		dest->error = NULL;
	}
	if (src->error) {
		dest->error = xmmsv_ref (src->error);
	}
}

void
xcp_data_set_completion (xcp_data_t *data, xmmsv_collparser_complete_t flags)
{
	data->completion = flags;
}

void
xcp_data_add_completion (xcp_data_t *data, xmmsv_collparser_complete_t flags)
{
	data->completion |= flags;
}

void
xcp_data_merge_completion (xcp_data_t *src, xcp_data_t *dest)
{
	xcp_data_add_completion (dest, src->completion);
}

void
xcp_data_clear_completion (xcp_data_t *data)
{
	data->completion = XMMS_COLLPARSER_COMPLETE_NONE;
}

void
xcp_data_set_complete_infos (xcp_data_t *data, YYLTYPE *lloc, const char *field,
                             xmmsv_coll_type_t filter, const char *value)
{
	_data_complete_infos_free (data->complete_infos);
	data->complete_infos = _data_complete_infos_new (lloc, field, filter, value);
}

void
xcp_data_copy_complete_infos (xcp_data_t *src, xcp_data_t *dest)
{
	xcp_data_complete_infos_t *ci;
	_data_complete_infos_free (dest->complete_infos);
	ci = src->complete_infos;
	if (ci) {
		dest->complete_infos =
			_data_complete_infos_new(&ci->lloc, ci->field, ci->filter, ci->value);
	}
}

xmmsv_t *
xcp_data_build_result (xcp_data_t *data, xmmsv_collparser_mode_t mode)
{
	xmmsv_t *res = NULL;
	xcp_data_complete_infos_t *ci;
	YYLTYPE lloc;
	int sl, sc, el, ec;

	switch (mode) {
		case XMMS_COLLPARSER_MODE_COMPILE:
			if (data->error) {
				res = xmmsv_ref (data->error);
			} else if (data->collection) {
				res = xmmsv_ref (data->collection);
			}
			break;
		case XMMS_COLLPARSER_MODE_COMPLETE:
			if (data->error) {
				res = xmmsv_ref (data->error);
			} else if (data->completion) {
				res = xmmsv_new_dict ();
				xmmsv_dict_set_int (res, "complete", (int) data->completion);
				if (data->collection) {
					xmmsv_dict_set (res, "context", data->collection);
				}
				ci = data->complete_infos;
				if (ci) {
					if (ci->field) {
						xmmsv_dict_set_string (res, "field", ci->field);
					}
					if (((int) ci->filter) >= 0) {
						xmmsv_dict_set_int (res, "filter", (int) ci->filter);
					}
					if (ci->value) {
						xmmsv_dict_set_string (res, "value", ci->value);
					}
					lloc = data->complete_infos->lloc;
					/* Convert indices from 1-based to 0-based */
					sl = lloc.first_line - 1;
					sc = lloc.first_column - 1;
					el = lloc.last_line - 1;
					ec = lloc.last_column - 1;
					xmmsv_dict_set_int (res, "start_line", sl > 0 ? sl : 0);
					xmmsv_dict_set_int (res, "start_column", sc > 0 ? sc : 0);
					xmmsv_dict_set_int (res, "end_line", el > 0 ? el : 0);
					xmmsv_dict_set_int (res, "end_column", ec > 0 ? ec : 0);
				}
			}
			break;
	}
	/* NOTE: If res == NULL at this point, something is wrong in the
	 * parser implementation. */
	return res;
}
