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

#include "xcu.h"

#include <stdlib.h>
#include <stdio.h>
#include <limits.h>
#include <string.h>
#include <assert.h>
#include <dirent.h>
#include <sys/stat.h>

#include "xmmsclient/xmmscollparser.h"
#include "utils/jsonism.h"
#include "utils/coll_utils.h"
#include "utils/value_utils.h"

#define ASSERT_RETURN_VAL(test, ret) do { \
	int __t = !!(test); \
	CU_assertImplementation (__t, __LINE__, #test, __FILE__, "", CU_FALSE); \
	if (!__t) { return ret; } \
} while (0)
#define ASSERT_RETURN(test) ASSERT_RETURN_VAL(test,)

static xmmsv_t *tests_datapath = NULL;

static void scan_directory (const char *directory, const char *prefix, xmmsv_t *data_path);

PRESETUP(collparser)
{
	tests_datapath = xmmsv_new_dict ();
	scan_directory ("xmmsclient/collparser_tests", "collparser", tests_datapath);
}

static struct {
	const char *str;
	xmmsv_collparser_complete_t flag;
} complete_flag_strings[] = {
	{"binary-op", XMMS_COLLPARSER_COMPLETE_BINARY_OPERATOR},
	{"unary-op", XMMS_COLLPARSER_COMPLETE_UNARY_OPERATOR},
	{"field", XMMS_COLLPARSER_COMPLETE_FIELD},
	{"filter", XMMS_COLLPARSER_COMPLETE_FILTER},
	{"value", XMMS_COLLPARSER_COMPLETE_VALUE},
	{"reference", XMMS_COLLPARSER_COMPLETE_REFERENCE},
	{"universe", XMMS_COLLPARSER_COMPLETE_UNIVERSE},
	{"open-group", XMMS_COLLPARSER_COMPLETE_OPEN_GROUP},
	{"close-group", XMMS_COLLPARSER_COMPLETE_CLOSE_GROUP},
	{"sequence", XMMS_COLLPARSER_COMPLETE_SEQUENCE},
	{"id", XMMS_COLLPARSER_COMPLETE_ID},
	{NULL, 0}
};

static struct {
	const char *str;
	xmmsv_coll_type_t type;
} filter_strings[] = {
	{"+", XMMS_COLLECTION_TYPE_HAS},
	{"~", XMMS_COLLECTION_TYPE_MATCH},
	{":", XMMS_COLLECTION_TYPE_MATCH},
	{"^", XMMS_COLLECTION_TYPE_TOKEN},
	{"<", XMMS_COLLECTION_TYPE_SMALLER},
	{"<=", XMMS_COLLECTION_TYPE_SMALLEREQ},
	{"=", XMMS_COLLECTION_TYPE_EQUALS},
	{"!=", XMMS_COLLECTION_TYPE_NOTEQUAL},
	{">=", XMMS_COLLECTION_TYPE_GREATEREQ},
	{">", XMMS_COLLECTION_TYPE_GREATER},
	{NULL, 0}
};

static xmmsv_collparser_mode_t
mode_from_string (const char *s)
{
	if (strcmp ("compile", s) == 0) {
		return XMMS_COLLPARSER_MODE_COMPILE;
	} else if (strcmp ("complete", s) == 0) {
		return XMMS_COLLPARSER_MODE_COMPLETE;
	} else {
		return -1;
	}
}

static xmmsv_collparser_complete_t
complete_flag_from_string (const char *s)
{
	int i;

	for (i = 0; complete_flag_strings[i].str != NULL; i++) {
		if (strcmp (s, complete_flag_strings[i].str) == 0) {
			return complete_flag_strings[i].flag;
		}
	}
	CU_FAIL ("Unknown completion flag.");
	return 0;
}

static xmmsv_coll_type_t
filter_from_string (const char *s)
{
	int i;

	for (i = 0; filter_strings[i].str != NULL; i++) {
		if (strcmp (s, filter_strings[i].str) == 0) {
			return filter_strings[i].type;
		}
	}
	CU_FAIL ("Unknown filter.");
	return -1;
}

#define READ_SIZE 1024
static int
read_contents (const char *path, char **contents)
{
	FILE *f = NULL;
	char readbuf[READ_SIZE];
	size_t r, c_size;

	if (!contents) return 0;

	if (!(f = fopen(path, "rb"))) {
		return 0;
	}

	c_size = 1;
	*contents = (char *) calloc (c_size, 1);

	while ((r = fread (readbuf, 1, READ_SIZE-1, f)) > 0) {
		readbuf[r] = '\0';
		c_size += r;
		*contents = (char *) realloc (*contents, c_size);
		strncat (*contents, readbuf, r);
	}

	if (!feof(f) || ferror(f)) {
		free (*contents);
		*contents = NULL;
	}
	fclose (f);

	return (*contents) != NULL;
}

static int
prepare_complete_data (xmmsv_t *data)
{
	xmmsv_t *xvalue;
	xmmsv_list_iter_t *it;
	int complete_flags = 0, filter = 0;
	const char *complete_flag_str, *filter_str;
	xmmsv_t *coll;

	ASSERT_RETURN_VAL (xmmsv_dict_get (data, "complete", &xvalue), 0);
	ASSERT_RETURN_VAL (xmmsv_get_list_iter (xvalue, &it), 0);
	for (; xmmsv_list_iter_valid (it); xmmsv_list_iter_next (it)) {
		ASSERT_RETURN_VAL (xmmsv_list_iter_entry_string (it, &complete_flag_str), 0);
		complete_flags |= complete_flag_from_string (complete_flag_str);
	}
	xmmsv_list_iter_explicit_destroy (it);
	xmmsv_dict_set_int (data, "complete", complete_flags);
	if (xmmsv_dict_entry_get_string (data, "filter", &filter_str)) {
		filter = filter_from_string (filter_str);
		xmmsv_dict_set_int (data, "filter", filter);
	}

	if (xmmsv_dict_get (data, "context", &xvalue)) {
		coll = xmmsv_coll_from_dict (xvalue);
		xmmsv_dict_set (data, "context", coll);
		xmmsv_unref (coll);
	}

	return 1;
}

static int
prepare_test_data (xmmsv_t *data)
{
	const char *value_str;
	xmmsv_collparser_mode_t mode;
	xmmsv_t *cases;
	xmmsv_list_iter_t *it;
	xmmsv_dict_iter_t *dit;
	xmmsv_t *case_;
	xmmsv_t *repr;
	xmmsv_t *coll;
	xmmsv_t *error;

	ASSERT_RETURN_VAL (xmmsv_dict_entry_get_string (data, "mode", &value_str), 0);
	mode = mode_from_string (value_str);
	ASSERT_RETURN_VAL (mode != ((xmmsv_collparser_mode_t) -1), 0);
	ASSERT_RETURN_VAL (xmmsv_dict_get (data, "cases", &cases), 0);
	ASSERT_RETURN_VAL (xmmsv_get_list_iter (cases, &it), 0);

	for (; xmmsv_list_iter_valid (it); xmmsv_list_iter_next (it)) {
		xmmsv_list_iter_entry (it, &case_);
		ASSERT_RETURN_VAL (xmmsv_dict_entry_get_string (case_, "pattern", &value_str), 0);
		error = NULL;
		if (xmmsv_dict_get (case_, "error", &repr)) {
			ASSERT_RETURN_VAL (xmmsv_get_string (repr, &value_str), 0);
			error = xmmsv_new_error (value_str);
			xmmsv_dict_set (case_, "result", error);
			xmmsv_dict_remove (case_, "error");
			xmmsv_unref (error);
		} else {
			switch (mode) {
				case XMMS_COLLPARSER_MODE_COMPILE:
					ASSERT_RETURN_VAL (xmmsv_dict_get (case_, "collection", &repr), 0);
					coll = xmmsv_coll_from_dict (repr);
					ASSERT_RETURN_VAL (coll != NULL, 0);
					xmmsv_dict_set (case_, "result", coll);
					xmmsv_dict_remove (case_, "collection");
					xmmsv_unref (coll);
					break;
				case XMMS_COLLPARSER_MODE_COMPLETE:
					ASSERT_RETURN_VAL (xmmsv_dict_get (case_, "complete-data", &repr), 0);
					ASSERT_RETURN_VAL (prepare_complete_data (repr), 0);
					xmmsv_dict_set (case_, "result", repr);
					xmmsv_dict_remove (case_, "complete-data");
					break;
				default:
					break;
			}
		}
		ASSERT_RETURN_VAL (xmmsv_get_dict_iter (case_, &dit), 0);
		for (; xmmsv_dict_iter_valid (dit); xmmsv_dict_iter_next (dit)) {
			ASSERT_RETURN_VAL (xmmsv_dict_iter_pair (dit, &value_str, &repr), 0);
			if (strcmp (value_str, "pattern") && strcmp (value_str, "result")) {
				CU_FAIL ("Unexpected dict entry");
			}
		}
		xmmsv_dict_iter_explicit_destroy (dit);
		xmmsv_dict_set_int (case_, "mode", (int32_t) mode);
	}
	xmmsv_list_iter_explicit_destroy (it);

	return 1;
}

static xmmsv_t *
build_testdata (const char *path)
{
	char *content = NULL;
	xmmsv_t *testdata;

	CU_ASSERT_FATAL (read_contents (path, &content));
	testdata = xmmsv_from_json (content);
	free (content);
	CU_ASSERT_FATAL (testdata != NULL);

	if (!prepare_test_data (testdata)) {
		xmmsv_unref (testdata);
		testdata = NULL;
	}

	return testdata;
}

/* Defined in src/clients/lib/xmmsclient/collparser/utils.c */
char *xcp_strjoin(const char *, ...);
static void run_test (void);

#include <errno.h>

static void
scan_directory (const char *directory, const char *prefix, xmmsv_t *data_path)
{
	struct dirent **namelist;
	int n, i;
	CU_pSuite testsuite = NULL;

	n = scandir (directory, &namelist, NULL, alphasort);
	assert (n > 0);
	for (i = 0; i < n; i++) {
		if (namelist[i]->d_name[0] != '.') {
			char *path, *testname, *ptr;
			struct stat buf;

			path = xcp_strjoin(directory, "/", namelist[i]->d_name, NULL);
			testname = xcp_strjoin (prefix, "_", namelist[i]->d_name, NULL);
			ptr = strchr (testname, '.');
			if (ptr) {
				*ptr = '\0';
			}
			if (!stat (path, &buf)) {
				if (S_ISREG (buf.st_mode)) {
					if (!testsuite) {
						testsuite = CU_add_suite (prefix, NULL, NULL);
					}
					xmmsv_dict_set_string (data_path, testname, path);
					CU_add_test (testsuite, testname, run_test);
				} else if (S_ISDIR (buf.st_mode)) {
					scan_directory (path, testname, data_path);
				}
			}

			free (testname);
			free (path);
		}
		free (namelist[i]);
	}
	free (namelist);
}

static void
run_test ()
{
	xmmsv_collparser_mode_t mode = -1;
	const char *path = NULL;
	const char *pattern;
	xmmsv_t *result, *parsed, *data, *case_;
	xmmsv_t *cases = NULL;
	xmmsv_list_iter_t *it = NULL;
	CU_pTest test;

	test = CU_get_current_test ();
	CU_ASSERT_FATAL (xmmsv_dict_entry_get_string (tests_datapath, test->pName, &path));
	data = build_testdata (path);
	CU_ASSERT_FATAL (data != NULL);

	if (data) {
		CU_ASSERT (xmmsv_dict_get (data, "cases", &cases));
		CU_ASSERT (xmmsv_get_list_iter (cases, &it));
		for (; xmmsv_list_iter_valid (it); xmmsv_list_iter_next (it)) {
			if (!xmmsv_list_iter_entry (it, &case_)) {
				CU_FAIL ("Failed to retrieve test case.");
				break;
			}
			mode = -1;
			pattern = NULL;
			result = NULL;
			parsed = NULL;
			CU_ASSERT (xmmsv_dict_entry_get_int (case_, "mode", (int32_t *)&mode));
			CU_ASSERT (xmmsv_dict_entry_get_string (case_, "pattern", &pattern));
			CU_ASSERT (xmmsv_dict_get (case_, "result", &result));
	
			CU_ASSERT (pattern != NULL);
			if (pattern) {
				switch (mode) {
					case XMMS_COLLPARSER_MODE_COMPILE:
						parsed = xmmsv_collparser_compile (pattern);
						break;
					case XMMS_COLLPARSER_MODE_COMPLETE:
						parsed = xmmsv_collparser_complete (pattern);
						break;
					default:
						break;
				}
			}
			CU_ASSERT (parsed != NULL);
			if (parsed) {
				CU_ASSERT (xmmsv_compare (parsed, result));
				xmmsv_unref (parsed);
			}
		}
		xmmsv_unref (data);
	}
}

