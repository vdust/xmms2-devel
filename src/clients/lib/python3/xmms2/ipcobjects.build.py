#!/usr/bin/env python

import os
import sys
import re

sys.path.append('../waftools')
import genipc
from indenter import Indenter

# Use this trick to ensure we load the correct file
_typesfile = os.path.join(os.path.dirname(__file__), 'types.py')
try:
	from importlib.machinery import SourceFileLoader
	types = SourceFileLoader('xmms.types', _typesfile).load_module()
except ImportError: # python < 3.3
	from imp import load_source
	types = load_source('xmms.types', _typesfile)

_blacklist = set([
	'visualization',
	])

class _Attr: # Used with enum names where repr() is called.
	def __init__(self, name):
		self._name = name
	def __repr__(self):
		return self._name

def classname(s):
	"""
	Create a python-style class name from an underscore-separated composed name
	e.g. foo_bar => FooBar
	"""
	return s.title().replace('_', '')

def filtered(objects):
	"""
	Enumerate objects not in the blacklist
	"""
	for o in objects:
		if o.name in _blacklist:
			continue
		yield o

def _enum_type_resolve(typelist, ref=None, enums=None):
	"""
	Flatten 'enum-value' tuple to 'enum' string in type list.

	Additionally, if #ref and #enums are not None, and the first argument in
	typelist is 'enum-value', resolve #ref as an _Attr instance.

	Return a tuple containing the flattened typelist and the resolved reference
	(or the original #ref argument if no resolution was done).
	"""
	newlist = []
	i = 0
	refval = ref
	for t in typelist:
		if not isinstance(t, str):
			et, ename = t
			newlist.append('enum')
			if i == 0 and ref and enums:
				refval = _Attr(enums[ename][ref].fullname())
		else:
			newlist.append(t)
		i += 1
	return newlist, refval


def emit_docstring(target):
	""" Print the docstring code """
	Indenter.printx('"""')
	print(target.documentation.replace("\\", "\\\\").replace('"', '\\"'), end='')
	print('"""')

def emit_processing(target, cmd, args, obj_id = 'self.__ipcid__'):
	"""
	Emit code for command queueing and result creation.

	#args must be a list of strings with literal values in their repr() form.

	Example:

	    emit_processing(target, cmd, [repr("A literal string"), 'arg_name', repr(42)])
	"""
	rtype = getattr(getattr(target, 'return_value', None), 'type', None)
	rtype, _unused = _enum_type_resolve(rtype or [])
	rtype = types.typeid(rtype) or None

	Indenter.printline('_ = self.ipc.message_factory(%s, %s, data = [%s])' % (
		obj_id,
		cmd.fullname(),
		', '.join(args),
		))
	Indenter.printline('self.ipc.queue_msg(_)')
	Indenter.printline('return self.ipc.result_factory(_, self.ipc, %r)' % (rtype,))

def emit_method(obj, method, enums):
	"""
	Emit object method code
	"""
	args = []
	nargs = ['self']
	kwargs = []
	for a in method.arguments:
		args.append(a.name)

		hasdefault = hasattr(a, 'default_hint')
		if hasdefault:
			try:
				_hint = a.default_hint
				xtype, _hint = _enum_type_resolve(a.type, _hint, enums)
				xtype = types.typeid(xtype) or None

				if not isinstance(_hint, _Attr):
					_hint = types.parse_value(_hint, xtype or None)

				kwargs.append('%s=%r' % (a.name, _hint))
			except Exception:
				# Fallback to standard argument without default.
				hasdefault = False

		if not hasdefault:
			nargs.append(a.name)

	Indenter.printline("def %s(%s%s%s):" % (method.name, ", ".join(nargs), kwargs and ", " or "", ", ".join(kwargs)))
	Indenter.enter()

	emit_docstring(method)

	for a in method.arguments:
		_type, _u = _enum_type_resolve(a.type or [])
		_type = types.typeid(_type) or None
		Indenter.printline("_v(%s, %r, name=%r)" % (a.name, _type, a.name))

	emit_processing(method, method.id, args)

	Indenter.leave()
	Indenter.printline("%s.__revid__ = '%%s:%%s' %% (%s, %s)\n" % (method.name, obj.id.fullname(), method.id.fullname()))

def emit_broadcast(obj, broadcast, sigid, sigenum):
	Indenter.printline("def %s_broadcast(self):" % broadcast.name)
	Indenter.enter()

	emit_docstring(broadcast)

	emit_processing(broadcast, sigenum['BROADCAST'], [broadcast.id.fullname()], sigid.fullname())

	Indenter.leave()

def emit_signal(obj, signal, sigid, sigenum):
	Indenter.printline("def %s_signal(self):" % signal.name)
	Indenter.enter()

	emit_docstring(signal)

	emit_processing(signal, sigenum['SIGNAL'], [signal.id.fullname()], sigid.fullname())

	Indenter.leave()

def emit_object(obj, enums):
	Indenter.printline("class %s(_base, ipcname=%r, ipcid=%s):"
			% (classname(obj.name), obj.name, obj.id.fullname()))
	Indenter.enter()

	for method in obj.methods:
		emit_method(obj, method, enums)

	sigid = enums['ipc_object']['SIGNAL']
	sigenum = enums['ipc_command_signal']

	for broadcast in obj.broadcasts:
		emit_broadcast(obj, broadcast, sigid, sigenum)

	for signal in obj.signals:
		emit_signal(obj, signal, sigid, sigenum)

	Indenter.leave()

def emit_api(objects):
	Indenter.enter("class Api(ApiBase):")

	for obj in filtered(objects):
		Indenter.printline("_%s = %s" % (classname(obj.name), classname(obj.name)))
	Indenter.printline()

	Indenter.enter("def __init__(self, ipc):")
	names = []
	for obj in filtered(objects):
		names.append(obj.name)
		Indenter.printline("self.%s = self._%s(ipc)" % (obj.name, classname(obj.name)))
	# ApiBase will initialize introspection index for ipc message debugging.
	Indenter.printline("super(Api, self).__init__()")
	Indenter.leave('')

	Indenter.enter("def __iter__(self):")
	for obj in filtered(objects):
		Indenter.printline("yield self.%s" % obj.name)
	Indenter.leave('')
	Indenter.enter("def __getitem__(self, key):")
	Indenter.printline("return getattr(self, key)")
	Indenter.leave()

	Indenter.leave()

def emit_constants_and_enums(constants, enums):
	for n, c in constants.items():
		Indenter.printline("%s = %r" % (c.name, c.value))
	print('')

	enum_resolved = dict()

	for n, e in enums.items():
		Indenter.printline("# Enum %s" % n)
		value = -1
		for k, m in e.items():
			if hasattr(m, 'value'):
				value = m.value
			elif hasattr(m, 'alias'):
				name, src, _type = m.alias
				if _type == 'constant':
					value = constants[name].value
				else:
					eref = enum_resolved[src] if src else e
					value = eref[name].resolved_value
			else:
				value += 1

			m.resolved_value = value # For aliases

			Indenter.printline("%s = %s" % (m.fullname(), value))
		enum_resolved[n] = e # For aliases
		print('')
	print('')


def build(ipcxml):
	ipc = genipc.parse_xml(ipcxml)

	Indenter.printline("# encoding: utf-8\n")
	Indenter.printline("# Generated from ipc.xml")
	Indenter.printline("# Do not edit\n")
	Indenter.printline('"""Ipc objects"""\n')

	Indenter.printline("from .types import validate as _v")
	Indenter.printline("from .ipc import ApiBase, IpcObject as _base\n")

	emit_constants_and_enums(ipc.constants, ipc.enums)

	for obj in filtered(ipc.objects):
		emit_object(obj, ipc.enums)

	emit_api(ipc.objects)

if __name__ == '__main__':
	self, ipcxml = sys.argv
	build(ipcxml)
