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

#include <string.h>

#include "collbuilder_priv.h"
#include "utils_priv.h"

#define XCP_COLLPARSER_NAMESPACE_DEFAULT "Collections"

static char *_extract_namespace (const char *refname, const char **ref);

static xmmsv_t *_build_operator (xmmsv_coll_type_t type, xmmsv_t *op1, xmmsv_t *op2);

typedef struct {
	char _short;
	const char *_long;
} _coll_prop_t;

static const _coll_prop_t coll_prop_short[] = {
	{ 'a', "artist" },
	{ 'b', "album" },
	{ 'd', "date" },
	{ 'l', "album" },
	{ 't', "title" },
	{ 'n', "tracknr" },
	{ 'y', "date" },
	{ 'g', "genre" },
	{ 'u', "url" },
	{ '\0', NULL },
};

static char *
_extract_namespace (const char *refname, const char **ref)
{
	size_t len;
	char *slash;
	char *namespace = NULL;
	if (refname) {
		slash = strchr (refname, '/');
		if (!slash) {
			namespace = xcp_strdup_safe (XCP_COLLPARSER_NAMESPACE_DEFAULT);
			if (ref) {
				*ref = refname;
			}
		} else {
			len = (size_t)(slash - refname);
			namespace = xcp_new (char, len + 1);
			strncpy (namespace, refname, len);
			namespace[len] = '\0';
			if (ref) {
				*ref = slash + 1;
			}
		}
	}
	return namespace;
}

static xmmsv_t *
_build_operator (xmmsv_coll_type_t type, xmmsv_t *op1, xmmsv_t *op2)
{
	xmmsv_t *coll, *coll2;
	xmmsv_t *operands;
	xmmsv_list_iter_t *iter;

	if (xmmsv_coll_get_type (op1) == type) {
		coll = xmmsv_ref (op1);
	} else {
		coll = xmmsv_new_coll (type);
		xmmsv_coll_add_operand (coll, op1);
	}
	if (xmmsv_coll_get_type (op2) == type) {
		operands = xmmsv_coll_operands_get (op2);
		xmmsv_get_list_iter (operands, &iter);
		while (xmmsv_list_iter_valid (iter)) {
			xmmsv_list_iter_entry (iter, &coll2);
			xmmsv_ref (coll2);
			xmmsv_list_iter_remove (iter);
			xmmsv_coll_add_operand (coll, coll2);
			xmmsv_unref (coll2);
		}
		xmmsv_list_iter_explicit_destroy (iter);
	} else {
		xmmsv_coll_add_operand (coll, op2);
	}

	return coll;
}


xmmsv_t *
xcp_build_union (xmmsv_t *op1, xmmsv_t *op2)
{
	return _build_operator (XMMS_COLLECTION_TYPE_UNION, op1, op2);
}

xmmsv_t *
xcp_build_intersection (xmmsv_t *op1, xmmsv_t *op2)
{
	return _build_operator (XMMS_COLLECTION_TYPE_INTERSECTION, op1, op2);
}

xmmsv_t *
xcp_build_idlist (xmmsv_t *sequence)
{
	xmmsv_t *coll;
	xmmsv_list_iter_t *it;
	int id;
	coll = xmmsv_new_coll (XMMS_COLLECTION_TYPE_IDLIST);
	xmmsv_get_list_iter (sequence, &it);
	for (; xmmsv_list_iter_valid (it); xmmsv_list_iter_next (it)) {
		xmmsv_list_iter_entry_int (it, &id);
		xmmsv_coll_idlist_append (coll, id);
	}
	xmmsv_list_iter_explicit_destroy (it);
	return coll;
}

xmmsv_t *
xcp_build_complement (xmmsv_t *op)
{
	xmmsv_t *coll;
	coll = xmmsv_new_coll (XMMS_COLLECTION_TYPE_COMPLEMENT);
	xmmsv_coll_add_operand (coll, op);

	return coll;
}

xmmsv_t *
xcp_build_reference (const char *refname)
{
	char *namespace;
	const char *ref = NULL;
	xmmsv_t *coll;

	namespace = _extract_namespace (refname, &ref);
	if (!strcmp (namespace, XMMS_COLLECTION_NS_COLLECTIONS) &&
	    !strcmp (ref, "All Media")) {
	    coll = xcp_build_universe ();
	} else {
		coll = xmmsv_new_coll (XMMS_COLLECTION_TYPE_REFERENCE);
		xmmsv_coll_attribute_set_string (coll, "namespace", namespace);
		xmmsv_coll_attribute_set_string (coll, "reference", ref);
	}
	free (namespace);

	return coll;
}

xmmsv_t *
xcp_build_universe (void)
{
	xmmsv_t *univ;
	univ = xmmsv_new_coll (XMMS_COLLECTION_TYPE_REFERENCE);
	xmmsv_coll_attribute_set_string (univ, "namespace", XMMS_COLLECTION_NS_COLLECTIONS);
	xmmsv_coll_attribute_set_string (univ, "reference", "All Media");
	return univ;
}

xmmsv_t *
xcp_build_filter (xmmsv_coll_type_t filter, const char *field, const char *value)
{
	xmmsv_t *coll, *univ;

	coll = xmmsv_new_coll (filter);

	if (field) {
		xmmsv_coll_attribute_set (coll, "field", field);
	}
	if (value) {
		xmmsv_coll_attribute_set_string (coll, "value", value);
	}
	univ = xcp_build_universe();
	xmmsv_coll_add_operand (coll, univ);
	xmmsv_unref (univ);

	return coll;
}

char *
xcp_build_property_from_short (const char *_short)
{
	const _coll_prop_t *iter;
	const char *ret = _short;

	for (iter = coll_prop_short; iter->_short; ++iter) {
		if (_short[0] == iter->_short) {
			ret = iter->_long;
			break;
		}
	}
	return xcp_strdup_safe (ret);
}
