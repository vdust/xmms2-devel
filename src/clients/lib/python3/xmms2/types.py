# encoding: utf-8
"""
Data types supported by the ipc protocol.
"""

# Common usecases don't need ipc types directly.
# Exporting only members that don't require them.
__all__ = [
		'iscollection',
		'Error',
		'RestrictedList',
		'rlist',
		]

NONE = 0
ERROR = 1
INTEGER = 2
STRING = 3
COLLECTION = 4
BINARY = 5
LIST = 6
DICTIONARY = 7
BITBUFFER = 8 # unused
FLOAT = 9

_types = {
	NONE: 'none',
	ERROR: 'error',
	INTEGER: 'integer',
	STRING: 'string',
	COLLECTION: 'collection',
	BINARY: 'binary',
	LIST: 'list',
	DICTIONARY: 'dictionary',
	BITBUFFER: 'bitbuffer',
	FLOAT: 'float',
	}
# Build reverse too
for _k in list(_types.keys()):
	_types[_types[_k]] = _k
	# one char aliases for lazy people
	if _types[_k] not in ('none', 'bitbuffer'):
		_types[_types[_k][0]] = _k
del _k

_types['coll'] = COLLECTION # convenience for lazy people
_types['dict'] = DICTIONARY # convenience for lazy people
_types['enum'] = INTEGER # special case for genipc
_types['int'] = INTEGER # required for genipc

def typename(types):
	"""
	Translate an ipc type id to a string.
	Accept an integer or an iterable as argument.
	Return None if a type id has no associated string.
	If an iterable is passed in, return translated elements as a tuple.
	"""
	if isinstance(types, int):
		return _types.get(types)
	_res = []
	for t in types:
		if not isinstance(t, int):
			raise TypeError("integer expected")
		_res.append(_types.get(t))
	return tuple(_res)

def typeid(types):
	"""
	Translate an ipc type name to a type id.
	Accept a string or an iterable as argument.
	Return None if a string has no associated id.
	If an iterable is passed in, return translated elements as a tuple.
	"""
	if isinstance(types, str):
		return _types.get(types)
	_res = []
	for t in types:
		if not isinstance(t, str):
			raise TypeError("string expected")
		_res.append(_types.get(t))
	return tuple(_res)

def iscollection(object):
	"""
	Check whether an object is a serializable Collection.
	This test is prefered to instance type check to allow duck typing
	"""
	return getattr(object, '__iscollection__', False)

def typeof(object):
	"""
	Get the serialized ipc type for a given python object.
	"""
	if hasattr(object, '__ipctype__'):
		# Allow any type to be serialized as one of the supported types.
		return object.__ipctype__
	elif iscollection(object):
		return COLLECTION
	elif object is None:
		return NONE
	elif isinstance(object, Exception):
		return ERROR
	elif isinstance(object, int):
		return INTEGER
	elif isinstance(object, str):
		return STRING
	elif isinstance(object, bytes):
		return BINARY
	elif isinstance(object, (list, set, tuple)):
		return LIST
	elif isinstance(object, dict):
		return DICTIONARY
	elif isinstance(object, float):
		return FLOAT
	elif hasattr(getattr(object, 'keys', None), '__call__'):
		return DICTIONARY
	else:
		try:
			it = iter(object)
		except TypeError:
			pass
		else:
			return LIST
	return None

def parse_value(string, to_type):
	if not to_type: # Assumes string is supported
		return string
	if not isinstance(to_type, int):
		to_type = to_type[0]
	if to_type == INTEGER:
		return int(string.strip())
	elif to_type == STRING:
		return string
	elif to_type == FLOAT:
		return float(string.strip())
	else:
		raise TypeError("Value parsing supported for int, float and str only")

# Used by validate()
_pytypes = {
	NONE: 'NoneType',
	ERROR: 'Exception',
	INTEGER: 'int',
	STRING: 'string',
	COLLECTION: 'Collection',
	BINARY: 'bytes',
	LIST: 'list',
	DICTIONARY: 'dict',
	BITBUFFER: '<unsupported>',
	FLOAT: 'float',
	}
def validate(object, typelist, ignoreNone = True, name='', acceptError = False):
	"""
	Validate the associated ipc type of an object.

	#typelist can be a single type id or a list-like object containing type
	ids. Extra types are used to perform recursive validation on list-like and
	dict-like objects only and are ignored for othe rtypes.

	Success occurs when one of this condition is met:
		- typelist is empty or typelist[0] is None
		- #acceptError is True and object is an Exception
		- #ignoreNone is True and typelist[0] == NONE
		- #object is of type #typelist[0] and recursive validation succeeds
		  where appliable.

	If a type mismatch is found, raises a TypeError exception. #name can be
	provided to enhance the exception message.
	"""
	if isinstance(typelist, int):
		typelist = (typelist,)
	elif not typelist: # empty list or None
		return

	t = typelist[0]
	typelist = typelist[1:]

	check = False
	if t is None: # unknown: accept any
		return
	elif acceptError and isinstance(object, Exception):
		return
	elif t == NONE:
		if ignoreNone:
			return
		test = type(None)
	elif t == ERROR:
		test = (Exception, str)
	elif t == INTEGER:
		test = int
	elif t == STRING:
		test = str
	elif t == COLLECTION:
		test = None
		check = iscollection(object)
	elif t == BINARY:
		test = bytes
	elif t == LIST:
		test = (list, set, tuple)
	elif t == DICTIONARY:
		test = dict
	elif t == FLOAT:
		test = (float, int)
	else:
		raise TypeError("Unsupported ipc type %r" % t)

	if test:
		if not isinstance(test, tuple):
			test = (test,)
		check = isinstance(object, test)

	if test and not check: # Extend support to dict-like and list-like objects
		if t == DICTIONARY and hasattr(getattr(object, 'keys', None), '__call__'):
			check = True
		elif t == LIST:
			try:
				iter(object)
				check = True
			except TypeError:
				pass

	if not check:
		raise TypeError("%s%s%s expected, got %s"
				% (name, name and ': ', _pytypes[t], type(object).__name__))

	# If required, recursive check on types that support it.
	if typelist and typelist[0] is not None:
		if t == DICTIONARY:
			for k in object.keys():
				validate(object[k], typelist, ignoreNone, name and ("%s[%s]" % (name, k)))
		elif t == LIST:
			for i, o in enumerate(object):
				validate(o, typelist, ignoreNone, name and ("%s[%d]" % (name, i)))


# Generic classes. (Collection classes defined in xmms.collections).

class Error(Exception): pass

class RestrictedList(list):
	""" Create a list with restrictions on items values type
	"""
	__restrictedtype__ = NONE

	def __init__(self, iterable=None, type=None):
		if type is not None:
			self.__restrictedtype__ = type

		if iterable is None:
			super(RestrictedList, self).__init__()
		else:
			super(RestrictedList, self).__init__(iterable)
			self.validate()

	def restrict(self, type, converter = None):
		""" Set type constraints on the list.

		#type can be a type id (see #typeid()), or a tuple of type ids (see #validate())

		If a #converter function is provided, existing elements will be first
		processed through this function before applying the constraints. Otherwise,
		existing elements must validate the new constraints.

		A TypeError exception is raised if an item fails to validate the constraints
		(with or without a #converter).
		"""
		if converter is None:
			self.validate(type)
			# Set the restriction once validation succeeded
			self.__restrictedtype__ = type
		elif hasattr(converter, '__call__'):
			# Use a proxy list to preserve the current list if an error occur.
			l = []
			for v in self:
				cv = converter(v)
				validate(cv, type)
				l.append(cv)
			self.clear()
			self.__restrictedtype__ = type
			# Con't need to check values again
			super(RestrictedList, self).extend(l)
		else:
			raise TypeError("The converter argument must be a function or None")

	def validate(self, type = None):
		""" Validate elements in the list against #type constraints
		A TypeError exception is raised if an element fails to validate the constraints.
		"""
		if type is None:
			type = self.__restrictedtype__

		if not type:
			return

		for v in self:
			validate(v, type)

	def __iadd__(self, value):
		""" self.__iadd__(value) <=> self += value """
		if not self.__restrictedtype__:
			return super(RestrictedList, self).__iadd__(value)
		self.extend(value)
		return self

	def __setitem__(self, key, value):
		""" self.__setitem__(key, value) <=> self[key] = value """
		if self.__restrictedtype__:
			validate(value, self.__restrictedtype__)
		return super(RestrictedList, self).__setitem__(key, value)

	def append(self, value):
		if self.__restrictedtype__:
			validate(value, self.__restrictedtype__)
		return super(RestrictedList, self).append(value)

	def copy(self):
		""" Copy the restricted list """
		newlist = self.__class__()
		newlist.__restrictedtype__ = self.__restrictedtype__
		super(RestrictedList, newlist).extend(self)
		return newlist

	def extend(self, iterable):
		""" Extend the list with items from #iterable after validating type contraints. """
		if not self.__restrictedtype__:
			return super(RestrictedList, self).extend(value)
		delfrom = len(self)
		try:
			for v in iterable:
				self.append(v)
		except TypeError:
			del self[delfrom:]
			raise

	def insert(self, index, value):
		""" Insert an element in the list after validating typ constraints. """
		if self.__restrictedtype__:
			validate(value, self.__restrictedtype__)
		return super(restrictedList, self).insert(index, value)

def rlist(iterable=None, type=NONE):
	""" Convenience function to create a RestrictedList with a specific type. """
	c = RestrictedList()
	c.__restrictedtype__ = type
	if iterable:
		c.extend(iterable)
	return c
