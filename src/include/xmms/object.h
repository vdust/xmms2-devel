/*  XMMS2 - X Music Multiplexer System
 *  Copyright (C) 2003	Peter Alm, Tobias Rundstr�m, Anders Gustafsson
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




#ifndef __XMMS_OBJECT_H__
#define __XMMS_OBJECT_H__

#include <glib.h>
#include <xmms/error.h>
#include "xmms/signal_xmms.h"

#define XMMS_OBJECT_MID 0x00455574

struct xmms_object_St;
typedef struct xmms_object_St xmms_object_t;
typedef struct xmms_object_cmd_desc_St xmms_object_cmd_desc_t;

typedef void (*xmms_object_destroy_func_t) (xmms_object_t *object);

/** @addtogroup Object
  * @{
  */
struct xmms_object_St {
	guint32 id;
	GMutex *mutex;

	GList *signals[XMMS_IPC_SIGNAL_END];
	xmms_object_cmd_desc_t *cmds[XMMS_IPC_CMD_END];

	gint ref;
	xmms_object_destroy_func_t destroy_func;
};

/** @} */

typedef enum {
	XMMS_OBJECT_CMD_ARG_NONE,
	XMMS_OBJECT_CMD_ARG_UINT32,
	XMMS_OBJECT_CMD_ARG_INT32,
	XMMS_OBJECT_CMD_ARG_STRING,
	XMMS_OBJECT_CMD_ARG_HASHTABLE,
	XMMS_OBJECT_CMD_ARG_UINTLIST,
	XMMS_OBJECT_CMD_ARG_INTLIST,
	XMMS_OBJECT_CMD_ARG_STRINGLIST,
	XMMS_OBJECT_CMD_ARG_PLCH,
	XMMS_OBJECT_CMD_ARG_HASHLIST,
} xmms_object_cmd_arg_type_t;

typedef void (*xmms_object_handler_t) (xmms_object_t *object, gconstpointer data, gpointer userdata);

#include <xmms/playlist.h>

#define XMMS_OBJECT_CMD_MAX_ARGS 2
typedef struct {
	xmms_object_cmd_arg_type_t types[XMMS_OBJECT_CMD_MAX_ARGS];
	union {
		gint32 int32;
		guint uint32;
		char *string;
	} values[XMMS_OBJECT_CMD_MAX_ARGS];
	xmms_object_cmd_arg_type_t rettype;
	union {
		guint32 uint32;
		gint32 int32;
		char *string;
		GHashTable *hashtable;
		GList *uintlist;
		GList *intlist;
		GList *stringlist; /* GList of const gchar * */
		GList *hashlist;
		xmms_playlist_changed_msg_t *plch;
	} retval;
	xmms_error_t error;
} xmms_object_cmd_arg_t;

typedef void (*xmms_object_cmd_func_t) (xmms_object_t *object, xmms_object_cmd_arg_t *arg);

struct xmms_object_cmd_desc_St {
	xmms_object_cmd_func_t func;
	xmms_object_cmd_arg_type_t arg1;
	xmms_object_cmd_arg_type_t arg2;
	xmms_object_cmd_arg_type_t retval;
};



#define XMMS_OBJECT(p) ((xmms_object_t *)p)
#define XMMS_IS_OBJECT(p) (XMMS_OBJECT (p)->id == XMMS_OBJECT_MID)

void xmms_object_cleanup (xmms_object_t *object);

void xmms_object_parent_set (xmms_object_t *object, xmms_object_t *parent);

void xmms_object_connect (xmms_object_t *object, guint32 signalid,
			  xmms_object_handler_t handler, gpointer userdata);

void xmms_object_disconnect (xmms_object_t *object, guint32 signalid,
			     xmms_object_handler_t handler);

void xmms_object_emit (xmms_object_t *object, guint32 signalid,
		       gconstpointer data);

void xmms_object_emit_f (xmms_object_t *object, guint32 signalid,
			 xmms_object_cmd_arg_type_t type, ...);

void xmms_object_cmd_arg_init (xmms_object_cmd_arg_t *arg);

void xmms_object_cmd_add (xmms_object_t *object, guint cmdid, xmms_object_cmd_desc_t *desc);

void xmms_object_cmd_call (xmms_object_t *object, guint cmdid, xmms_object_cmd_arg_t *arg);

/* Some minor macro-magic. XMMS_CMD_DEFINE and XMMS_CMD_FUNC
 * are the only ones to be used directly */

#define __XMMS_CMD_DO_ARG_NONE(a)
#define __XMMS_CMD_DO_ARG_STRING(a) ,arg->values[a].string
#define __XMMS_CMD_DO_ARG_UINT32(a) ,arg->values[a].uint32
#define __XMMS_CMD_DO_ARG_INT32(a) ,arg->values[a].int32

#define __XMMS_CMD_DO_RETVAL_NONE()
#define __XMMS_CMD_DO_RETVAL_HASHTABLE() arg->retval.hashtable = 
#define __XMMS_CMD_DO_RETVAL_HASHLIST() arg->retval.hashlist = 
#define __XMMS_CMD_DO_RETVAL_UINT32() arg->retval.uint32 = 
#define __XMMS_CMD_DO_RETVAL_UINTLIST() arg->retval.uintlist = 
#define __XMMS_CMD_DO_RETVAL_INTLIST() arg->retval.intlist = 
#define __XMMS_CMD_DO_RETVAL_STRINGLIST() arg->retval.stringlist = 
#define __XMMS_CMD_DO_RETVAL_STRING() arg->retval.string = 
#define __XMMS_CMD_DO_RETVAL_PLCH() arg->retval.plch = 

#define XMMS_CMD_DEFINE(cmdid, realfunc, argtype0, _rettype, argtype1, argtype2) static void \
__int_xmms_cmd_##cmdid (xmms_object_t *object, xmms_object_cmd_arg_t *arg) \
{ \
g_return_if_fail (XMMS_IS_OBJECT (object)); \
g_return_if_fail (arg->types[0] == XMMS_OBJECT_CMD_ARG_##argtype1); \
g_return_if_fail (arg->types[1] == XMMS_OBJECT_CMD_ARG_##argtype2); \
__XMMS_CMD_DO_RETVAL_##_rettype() realfunc ((argtype0)object __XMMS_CMD_DO_ARG_##argtype1(0) __XMMS_CMD_DO_ARG_##argtype2(1), &arg->error); \
arg->rettype = XMMS_OBJECT_CMD_ARG_##_rettype; \
} \
xmms_object_cmd_desc_t __int_xmms_cmd_desc_##cmdid = { __int_xmms_cmd_##cmdid, XMMS_OBJECT_CMD_ARG_##argtype1, XMMS_OBJECT_CMD_ARG_##argtype2, XMMS_OBJECT_CMD_ARG_##_rettype }

#define XMMS_CMD_FUNC(cmdid) &__int_xmms_cmd_desc_##cmdid


void __int_xmms_object_unref (xmms_object_t *object);
xmms_object_t *__int_xmms_object_new (gint size, xmms_object_destroy_func_t destfunc);

#define xmms_object_ref(obj) do { \
	if (obj && XMMS_IS_OBJECT (obj)) { \
		XMMS_OBJECT (obj)->ref++; \
		XMMS_DBG ("Reffing %p(%d)", obj, XMMS_OBJECT (obj)->ref); \
	} \
} while (0)

#define xmms_object_unref(obj) do { \
	if (obj && XMMS_IS_OBJECT (obj)) { \
		XMMS_DBG ("unreffing %p(%d)", obj, XMMS_OBJECT (obj)->ref); \
		__int_xmms_object_unref (XMMS_OBJECT (obj)); \
	} \
} while (0)

#define xmms_object_new(objtype,destroyfunc) (objtype *) __int_xmms_object_new (sizeof (objtype), destroyfunc)

#endif /* __XMMS_OBJECT_H__ */
