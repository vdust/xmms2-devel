/*  XMMS2 - X Music Multiplexer System
 *  Copyright (C) 2003-2013 XMMS2 Team
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

#include <string.h>

#include <xmmsc/xmmsc_stdbool.h>
#include <xmmsc/xmmsv.h>
#include <xmmscpriv/xmmsc_util.h>

static bool _internal_put_on_bb_bin (xmmsv_t *bb, const unsigned char *data, unsigned int len);
static bool _internal_put_on_bb_error (xmmsv_t *bb, const char *errmsg);
static bool _internal_put_on_bb_int32 (xmmsv_t *bb, int32_t v);
static bool _internal_put_on_bb_string (xmmsv_t *bb, const char *str);
static bool _internal_put_on_bb_collection (xmmsv_t *bb, xmmsv_t *coll);
static bool _internal_put_on_bb_value_list (xmmsv_t *bb, xmmsv_t *v);
static bool _internal_put_on_bb_value_dict (xmmsv_t *bb, xmmsv_t *v);

static bool _internal_get_from_bb_bin_alloc (xmmsv_t *bb, unsigned char **buf, unsigned int *len);
static bool _internal_get_from_bb_error_alloc (xmmsv_t *bb, char **buf, unsigned int *len);
static bool _internal_get_from_bb_int32 (xmmsv_t *bb, int32_t *v);
static bool _internal_get_from_bb_int32_positive (xmmsv_t *bb, int32_t *v);
static bool _internal_get_from_bb_string_alloc (xmmsv_t *bb, char **buf, unsigned int *len);
static bool _internal_get_from_bb_collection_alloc (xmmsv_t *bb, xmmsv_t **coll);
static bool _internal_get_from_bb_value_dict_alloc (xmmsv_t *bb, xmmsv_t **val);
static bool _internal_get_from_bb_value_list_alloc (xmmsv_t *bb, xmmsv_t **val);

static bool _internal_get_from_bb_value_of_type_alloc (xmmsv_t *bb, xmmsv_type_t type, xmmsv_t **val);


static bool
_internal_put_on_bb_bin (xmmsv_t *bb,
                         const unsigned char *data,
                         unsigned int len)
{
	if (!xmmsv_bitbuffer_put_bits (bb, 32, len))
		return false;

	return xmmsv_bitbuffer_put_data (bb, data, len);
}

static bool
_internal_put_on_bb_error (xmmsv_t *bb, const char *errmsg)
{
	if (!bb) {
		return -1;
	}

	if (!errmsg) {
		return xmmsv_bitbuffer_put_bits (bb, 32, 0);
	}

	if (!xmmsv_bitbuffer_put_bits (bb, 32, strlen (errmsg) + 1))
		return false;

	return xmmsv_bitbuffer_put_data (bb, (const unsigned char *) errmsg, strlen (errmsg) + 1);
}

static bool
_internal_put_on_bb_int32 (xmmsv_t *bb, int32_t v)
{
	return xmmsv_bitbuffer_put_bits (bb, 32, v);
}

static bool
_internal_put_on_bb_string (xmmsv_t *bb, const char *str)
{
	if (!bb) {
		return false;
	}

	if (!str) {
		return xmmsv_bitbuffer_put_bits (bb, 32, 0);
	}

	if (!xmmsv_bitbuffer_put_bits (bb, 32, strlen (str) + 1))
		return false;

	return xmmsv_bitbuffer_put_data (bb, (const unsigned char *) str, strlen (str) + 1);
}

static bool
_internal_put_on_bb_collection (xmmsv_t *bb, xmmsv_t *coll)
{
	xmmsv_list_iter_t *it;
	xmmsv_t *v;
	int n;
	uint32_t ret;
	int32_t entry;

	if (!bb || !coll) {
		return false;
	}

	/* push type */
	if (!xmmsv_bitbuffer_put_bits (bb, 32, xmmsv_coll_get_type (coll)))
		return false;

	/* attributes */
	_internal_put_on_bb_value_dict (bb, xmmsv_coll_attributes_get (coll));

	/* idlist counter and content */
	xmmsv_bitbuffer_put_bits (bb, 32, xmmsv_coll_idlist_get_size (coll));

	xmmsv_get_list_iter (xmmsv_coll_idlist_get (coll), &it);
	for (xmmsv_list_iter_first (it);
	     xmmsv_list_iter_valid (it);
	     xmmsv_list_iter_next (it)) {

		if (!xmmsv_list_iter_entry_int (it, &entry)) {
			x_api_error ("Non integer in idlist", 0);
		}
		xmmsv_bitbuffer_put_bits (bb, 32, entry);
	}
	xmmsv_list_iter_explicit_destroy (it);

	/* operands counter and objects */
	n = 0;
	if (xmmsv_coll_get_type (coll) != XMMS_COLLECTION_TYPE_REFERENCE) {
		n = xmmsv_list_get_size (xmmsv_coll_operands_get (coll));
	}

	ret = xmmsv_bitbuffer_pos (bb);
	xmmsv_bitbuffer_put_bits (bb, 32, n);

	if (n > 0) {
		xmmsv_get_list_iter (xmmsv_coll_operands_get (coll), &it);

		while (xmmsv_list_iter_entry (it, &v)) {
			if (!xmmsv_is_type (v, XMMSV_TYPE_COLL)) {
				x_api_error ("Non collection operand", 0);
			}

			_internal_put_on_bb_int32 (bb, XMMSV_TYPE_COLL);

			ret = _internal_put_on_bb_collection (bb, v);
			xmmsv_list_iter_next (it);
		}
	}

	return ret;
}

static bool
_internal_put_on_bb_value_list (xmmsv_t *bb, xmmsv_t *v)
{
	xmmsv_list_iter_t *it;
	xmmsv_t *entry;
	uint32_t offset, count;
	bool ret = true;

	if (!xmmsv_get_list_iter (v, &it)) {
		return false;
	}

	/* store a dummy value, store the real count once it's known */
	offset = xmmsv_bitbuffer_pos (bb);
	xmmsv_bitbuffer_put_bits (bb, 32, 0);

	count = 0;
	while (xmmsv_list_iter_valid (it)) {
		xmmsv_list_iter_entry (it, &entry);
		ret = xmmsv_bitbuffer_serialize_value (bb, entry);
		xmmsv_list_iter_next (it);
		count++;
	}

	/* overwrite with real size */
	xmmsv_bitbuffer_put_bits_at (bb, 32, count, offset);

	return ret;
}

static bool
_internal_put_on_bb_value_dict (xmmsv_t *bb, xmmsv_t *v)
{
	xmmsv_dict_iter_t *it;
	const char *key;
	xmmsv_t *entry;
	uint32_t ret, offset, count;

	if (!xmmsv_get_dict_iter (v, &it)) {
		return false;
	}

	/* store a dummy value, store the real count once it's known */
	offset = xmmsv_bitbuffer_pos (bb);
	xmmsv_bitbuffer_put_bits (bb, 32, 0);

	count = 0;
	while (xmmsv_dict_iter_valid (it)) {
		xmmsv_dict_iter_pair (it, &key, &entry);
		ret = _internal_put_on_bb_string (bb, key);
		ret = xmmsv_bitbuffer_serialize_value (bb, entry);
		xmmsv_dict_iter_next (it);
		count++;
	}

	/* overwrite with real size */
	xmmsv_bitbuffer_put_bits_at (bb, 32, count, offset);

	return ret;
}

static bool
_internal_get_from_bb_data (xmmsv_t *bb, void *buf, unsigned int len)
{
	if (!bb)
		return false;

	return xmmsv_bitbuffer_get_data (bb, buf, len);
}

static bool
_internal_get_from_bb_error_alloc (xmmsv_t *bb, char **buf,
                                   unsigned int *len)
{
	/* currently, an error is just a string, so reuse that */
	return _internal_get_from_bb_string_alloc (bb, buf, len);
}

static bool
_internal_get_from_bb_int32 (xmmsv_t *bb, int32_t *v)
{
	return xmmsv_bitbuffer_get_bits (bb, 32, v);
}

static bool
_internal_get_from_bb_int32_positive (xmmsv_t *bb, int32_t *v)
{
	bool ret;
	ret = _internal_get_from_bb_int32 (bb, v);
	if (ret && *v < 0)
		ret = false;
	return ret;
}
static bool
_internal_get_from_bb_string_alloc (xmmsv_t *bb, char **buf,
                                    unsigned int *len)
{
	char *str;
	int32_t l;

	if (!_internal_get_from_bb_int32_positive (bb, &l)) {
		return false;
	}

	str = x_malloc (l + 1);
	if (!str) {
		return false;
	}

	if (!_internal_get_from_bb_data (bb, str, l)) {
		free (str);
		return false;
	}

	str[l] = '\0';

	*buf = str;
	*len = l;

	return true;
}

static bool
_internal_get_from_bb_bin_alloc (xmmsv_t *bb,
                                 unsigned char **buf,
                                 unsigned int *len)
{
	unsigned char *b;
	int32_t l;

	if (!_internal_get_from_bb_int32_positive (bb, &l)) {
		return false;
	}

	b = x_malloc (l);
	if (!b) {
		return false;
	}

	if (!_internal_get_from_bb_data (bb, b, l)) {
		free (b);
		return false;
	}

	*buf = b;
	*len = l;

	return true;
}

static bool
_internal_get_from_bb_collection_alloc (xmmsv_t *bb, xmmsv_t **coll)
{
	int i;
	int32_t type;
	int32_t n_items;
	int id;
	int32_t *idlist = NULL;
	xmmsv_t *dict, *attrs;
	xmmsv_dict_iter_t *it;

	/* Get the type and create the collection */
	if (!_internal_get_from_bb_int32_positive (bb, &type)) {
		return false;
	}

	*coll = xmmsv_new_coll (type);

	/* Get the attributes */
	if (!_internal_get_from_bb_value_dict_alloc (bb, &dict)) {
		return false;
	}

	attrs = xmmsv_coll_attributes_get (*coll);

	xmmsv_get_dict_iter (dict, &it);
	while (xmmsv_dict_iter_valid (it)) {
		const char *key;
		xmmsv_t *value;
		xmmsv_dict_iter_pair (it, &key, &value);
		xmmsv_dict_set (attrs, key, value);
		xmmsv_dict_iter_next (it);
	}

	xmmsv_unref (dict);

	/* Get the idlist */
	if (!_internal_get_from_bb_int32_positive (bb, &n_items)) {
		goto err;
	}

	if (!(idlist = x_new (int32_t, n_items + 1))) {
		goto err;
	}

	for (i = 0; i < n_items; i++) {
		if (!_internal_get_from_bb_int32 (bb, &id)) {
			goto err;
		}

		idlist[i] = id;
	}

	idlist[i] = 0;
	xmmsv_coll_set_idlist (*coll, idlist);
	free (idlist);
	idlist = NULL;

	/* Get the operands */
	if (!_internal_get_from_bb_int32_positive (bb, &n_items)) {
		goto err;
	}

	for (i = 0; i < n_items; i++) {
		xmmsv_t *operand;

		if (!_internal_get_from_bb_int32_positive (bb, &type) ||
		    type != XMMSV_TYPE_COLL ||
		    !_internal_get_from_bb_collection_alloc (bb, &operand)) {
			goto err;
		}

		xmmsv_coll_add_operand (*coll, operand);
		xmmsv_unref (operand);
	}

	return true;

err:
	if (idlist != NULL) {
		free (idlist);
	}

	xmmsv_unref (*coll);

	return false;
}


static bool
_internal_get_from_bb_value_dict_alloc (xmmsv_t *bb, xmmsv_t **val)
{
	xmmsv_t *dict;
	int32_t len;
	unsigned int ignore;
	char *key;

	dict = xmmsv_new_dict ();

	if (!_internal_get_from_bb_int32_positive (bb, &len)) {
		goto err;
	}

	while (len--) {
		xmmsv_t *v;

		if (!_internal_get_from_bb_string_alloc (bb, &key, &ignore)) {
			goto err;
		}

		if (!xmmsv_bitbuffer_deserialize_value (bb, &v)) {
			free (key);
			goto err;
		}

		xmmsv_dict_set (dict, key, v);
		free (key);
		xmmsv_unref (v);
	}

	*val = dict;

	return true;

err:
	x_internal_error ("Message from server did not parse correctly!");
	xmmsv_unref (dict);
	return false;
}

static bool
_internal_get_from_bb_value_list_alloc (xmmsv_t *bb, xmmsv_t **val)
{
	xmmsv_t *list;
	int32_t len;

	list = xmmsv_new_list ();

	if (!_internal_get_from_bb_int32_positive (bb, &len)) {
		goto err;
	}

	while (len--) {
		xmmsv_t *v;
		if (xmmsv_bitbuffer_deserialize_value (bb, &v)) {
			xmmsv_list_append (list, v);
		} else {
			goto err;
		}
		xmmsv_unref (v);
	}

	*val = list;

	return true;

err:
	x_internal_error ("Message from server did not parse correctly!");
	xmmsv_unref (list);
	return false;
}

static bool
_internal_get_from_bb_value_of_type_alloc (xmmsv_t *bb, xmmsv_type_t type,
                                           xmmsv_t **val)
{
	int32_t i;
	uint32_t len;
	char *s;
	unsigned char *d;

	switch (type) {
		case XMMSV_TYPE_ERROR:
			if (!_internal_get_from_bb_error_alloc (bb, &s, &len)) {
				return false;
			}
			*val = xmmsv_new_error (s);
			free (s);
			break;
		case XMMSV_TYPE_INT32:
			if (!_internal_get_from_bb_int32 (bb, &i)) {
				return false;
			}
			*val = xmmsv_new_int (i);
			break;
		case XMMSV_TYPE_STRING:
			if (!_internal_get_from_bb_string_alloc (bb, &s, &len)) {
				return false;
			}
			*val = xmmsv_new_string (s);
			free (s);
			break;
		case XMMSV_TYPE_DICT:
			if (!_internal_get_from_bb_value_dict_alloc (bb, val)) {
				return false;
			}
			break;

		case XMMSV_TYPE_LIST :
			if (!_internal_get_from_bb_value_list_alloc (bb, val)) {
				return false;
			}
			break;

		case XMMSV_TYPE_COLL:
			if (!_internal_get_from_bb_collection_alloc (bb, val)) {
				return false;
			}
			break;

		case XMMSV_TYPE_BIN:
			if (!_internal_get_from_bb_bin_alloc (bb, &d, &len)) {
				return false;
			}
			*val = xmmsv_new_bin (d, len);
			free (d);
			break;

		case XMMSV_TYPE_NONE:
			*val = xmmsv_new_none ();
			break;
		default:
			x_internal_error ("Got message of unknown type!");
			return false;
	}

	return true;
}



int
xmmsv_bitbuffer_serialize_value (xmmsv_t *bb, xmmsv_t *v)
{
	bool ret;
	int32_t i;
	const char *s;
	const unsigned char *bc;
	unsigned int bl;
	xmmsv_type_t type;

	type = xmmsv_get_type (v);
	ret = _internal_put_on_bb_int32 (bb, type);
	if (!ret)
		return ret;

	switch (type) {
	case XMMSV_TYPE_ERROR:
		if (!xmmsv_get_error (v, &s)) {
			return false;
		}
		ret = _internal_put_on_bb_error (bb, s);
		break;
	case XMMSV_TYPE_INT32:
		if (!xmmsv_get_int (v, &i)) {
			return false;
		}
		ret = _internal_put_on_bb_int32 (bb, i);
		break;
	case XMMSV_TYPE_STRING:
		if (!xmmsv_get_string (v, &s)) {
			return false;
		}
		ret = _internal_put_on_bb_string (bb, s);
		break;
	case XMMSV_TYPE_COLL:
		ret = _internal_put_on_bb_collection (bb, v);
		break;
	case XMMSV_TYPE_BIN:
		if (!xmmsv_get_bin (v, &bc, &bl)) {
			return false;
		}
		ret = _internal_put_on_bb_bin (bb, bc, bl);
		break;
	case XMMSV_TYPE_LIST:
		ret = _internal_put_on_bb_value_list (bb, v);
		break;
	case XMMSV_TYPE_DICT:
		ret = _internal_put_on_bb_value_dict (bb, v);
		break;

	case XMMSV_TYPE_NONE:
		break;
	default:
		x_internal_error ("Tried to serialize value of unsupported type");
		return false;
	}

	return ret;
}

int
xmmsv_bitbuffer_deserialize_value (xmmsv_t *bb, xmmsv_t **val)
{
	int32_t type;

	if (!_internal_get_from_bb_int32 (bb, &type)) {
		return false;
	}

	return _internal_get_from_bb_value_of_type_alloc (bb, type, val);
}


xmmsv_t *
xmmsv_serialize (xmmsv_t *v)
{
	xmmsv_t *bb, *res;

	if (!v)
		return NULL;

	bb = xmmsv_new_bitbuffer ();

	if (!xmmsv_bitbuffer_serialize_value (bb, v)) {
		xmmsv_unref (bb);
		return NULL;
	}

	/* this is internally in xmmsv implementation,
	   so we could just switch the type,
	   but thats for later */
	res = xmmsv_new_bin (xmmsv_bitbuffer_buffer (bb), xmmsv_bitbuffer_len (bb) / 8);
	xmmsv_unref (bb);
	return res;
}

xmmsv_t *
xmmsv_deserialize (xmmsv_t *v)
{
	xmmsv_t *bb;
	xmmsv_t *res;
	const unsigned char *data;
	uint32_t len;


	if (!xmmsv_get_bin (v, &data, &len))
		return NULL;


	bb = xmmsv_new_bitbuffer_ro (data, len);

	if (!xmmsv_bitbuffer_deserialize_value (bb, &res)) {
		xmmsv_unref (bb);
		return NULL;
	}
	xmmsv_unref (bb);
	return res;
}
