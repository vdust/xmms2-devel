# encoding: utf-8
"""
Local service client.

To prevent collisions with service APIs, all methods and variables in
presentation objects are prefixed by '_sc__'. Since this prefix may change in
the future, such methods and variables should not be called directly,
but through convenience API, exported at module level.
"""

__all__ = [
		'isservice',
		'lookup',
		'method_arg',
		'method_varg',
		'Namespace',
		'path',
		'Service',
		'service_method',
		'service_broadcast',
		'service_constant',
		'walk',
		]

from .. import types

from .errors import NotFound

from inspect import getmembers

def _noop(*a, **kw): pass
def _setcb(f, fallback=_noop):
	return f if hasattr(f, '__call__') else fallback
def _exclattr(name):
	return name.startswith('__') or name.startswith('_sc__')

class method_arg:
	def __init__(self, name, type = '', doc = '', **kw):
		for key, value in kw.items():
			setattr(self, key, value)
		self.name = name
		self.type = type.lower()
		self._x_type = types.typeid(self.type)
		if self._x_type is None:
			raise TypeError("Unsupported argument type %r" % self.type)
		self.doc = doc

	def _sc__introspect_data(self):
		data = dict(
				name = self.name,
				docstring = self.doc,
				type = self._x_type,
				)
		for x in ('default', 'default_value'):
			if hasattr(self, x):
				data['default_value'] = getattr(self, x)
				break
		return data

class method_varg:
	def __init__(self, doc = '', **kw):
		self.doc = doc
		for key, value in kw.items():
			setattr(self, key, value)

class service_method:
	def __init__(self, positional = None, named = None, name = None, doc = None):
		self.positional = positional or ()
		self.named = named or ()
		self.name = name
		self.doc = doc

	def __call__(self, f):
		f._sc__service_entity = dict(
				entity = 'method',
				positional = self.positional,
				named = self.named,
				name = self.name or f.__name__,
				doc = self.doc or f.__doc__,
				)
		def _sc__introspect_data():
			data = dict(
					name = f._sc__service_entity['name'],
					docstring = f._sc__service_entity['doc']
					)
			for x in ('positional', 'named'):
				data[x+'-arguments'] = tuple([
					arg._sc__introspect_data()
						for arg in f._sc__service_entity[x]
						if isinstance(arg, method_arg)
					])
				data['va-'+x] = any(map(
					lambda arg: isinstance(arg, method_varg),
					f._sc__service_entity[x]))
			return data
		f._sc__introspect_data = _sc__introspect_data
		return f

	@staticmethod
	def bind(bind_to, method, bind = True):
		if not hasattr(method, '__call__'):
			raise ValueError("%r is not a callable" % method.__name__)
		if bind:
			def _bound_method(*a, **ka):
				return method(bind_to, *a, **ka)
			_bound_method.__name__ = '_bound_%s' % method.__name__
			_bound_method.__doc__ = method.__doc__
			_bound_method._method = method
			if hasattr(method, '_sc__service_entity'):
				_bound_method._sc__service_entity = method._sc__service_entity
			return _bound_method
		return method

class service_broadcast:
	def __init__(self, name = None, doc = None):
		self.name = name
		self.doc = doc
		self.namespace = None

	def _bind(self, namespace, name = None):
		"""
		Bind a broadcast to a Namespace instance
		For internal use on service instanciation
		"""
		obj = service_broadcast(self.name or name, self.doc)
		obj.namespace = namespace
		return obj

	def isbound(self):
		return self.namespace is not None

	def emit(self, value = None):
		if self.namespace is None:
			raise RuntimeError("No namespace bound to broadcast")
		path = self.namespace.path + (self.name,)
		return self.namespace._sc__client.service_emit_broadcast(path, value)

	def __call__(self, value = None):
		return self.emit(value)

	def _sc__introspect_data(self):
		return dict(
			name = self.name,
			docstring = self.doc,
			)

class service_constant:
	def __init__(self, value):
		self.value = value


## Convenience functions called on namespaces and services

def invalidate_cache(service, path = None, recursive = True):
	""" Equivalent to
	del service[path]
	"""
	service._sc__invalidate_cache(path, recursive)

def lookup(ns, path, sep='.'):
	"""
	Lookup a path in a namespace
	"""
	return ns._sc__lookup(path, sep)

def path(ns):
	"""
	Return the real path of a namespace
	"""
	return ns._sc__path()

def walk(ns, namespace = None, constant = None, method = None, broadcast = None, other = _noop, depth = -1, deep = False, _path = ()):
	"""
	Walk through namespace api.

	Arguments should be provided as keyword arguments only (order may change in
	the future).

	#namespace, #constant, #method, #broadcast are callbacks handlers for each
	type of entity in the api. Their signature is callback(path, object, infos)

	#other is called for entity types that don't have an explicit handler set
	(default to noop)

	#depth set the maximum depth recursion. Defaults to -1 (no limit)

	If #deep is True, handlers are called on namespaces' children before calls on
	the namespaces themselves.

	#_path is the path prefix to prepend to each entity path (used for recursion
	in sub namespaces)
	"""
	return ns._sc__walk(
		namespace = namespace,
		constant = constant,
		method = method,
		broadcast = broadcast,
		other = other,
		depth = depth,
		deep = deep,
		_path = _path)

class Namespace:
	_sc__namespace_path = ()

	def _sc__path(self):
		if self._sc__parent:
			_path = self._sc__parent._sc__path()
		else:
			_path = ()
		return _path + self._sc__namespace_path

	def __init__(self, client, parent = None):
		self._sc__parent = parent
		self._sc__client = client
		self._sc__registered = False
		self._sc__constants = set()

		for attr, obj in getmembers(self):
			if _exclattr(attr):
				continue

			if isinstance(obj, service_constant):
				self._sc__constants.add(attr)
				setattr(self, attr, obj.value)
				continue

			if isinstance(obj, service_broadcast):
				bc_obj = obj
				setattr(self, attr, bc_obj._bind(self, attr))
				continue

			try:
				if issubclass(obj, Service):
					raise RuntimeError("non-root namespace subclassing root class Service")
				if issubclass(obj, Namespace):
					inst = obj(self._sc__client, parent = self)
					inst._sc__namespace_path = (attr,)

					setattr(self, attr, inst)
					inst._sc__service_entity = dict(
							entity = 'namespace',
							name = attr,
							doc = inst.__doc__
							)
			except TypeError:
				pass

	def _sc__walk(self, namespace = None, constant = None, method = None, broadcast = None, other = _noop, depth = -1, deep = False, _path = ()):
		callbacks = dict(
				namespace = _setcb(namespace, other),
				constant = _setcb(constant, other),
				method = _setcb(method, other),
				broadcast = _setcb(broadcast, other),
				)

		for attr, obj in getmembers(self):
			if _exclattr(attr):
				continue
			if attr in self._sc__constants:
				entity = dict(
						entity = 'constant', 
						name = attr,
						)
			elif isinstance(obj, service_broadcast):
				entity = dict(
						entity = 'broadcast',
						name = obj.name or attr,
						doc = obj.doc
						)
			else:
				entity = getattr(obj, '_sc__service_entity', None)

			if not isinstance(entity, dict):
				continue

			entity_type = entity.get('entity', '')

			if entity_type not in callbacks:
				continue

			if not deep:
				callbacks[entity_type](_path, obj, entity)
			if entity_type == 'namespace' and depth:
				obj._sc__walk(
					depth = depth - 1,
					deep = deep,
					_path = _path + (entity['name'] or attr,),
					**callbacks)
			if deep:
				callbacks[entity_type](_path, obj, entity)

	def _sc__norm_path(self, path):
		if isinstance(path, str):
			if sep:
				path = path.split(sep)
				# Special case for obtaining the namespace itself
				if len(path == 1) and path[0] == '':
					path = ()
			else:
				path = (path,)

		path = tuple(path)

		if any(map(lambda s: not isinstance(s, str), path)):
			raise TypeError("Path must be a string or an iterable returning strings")

		return path

	def _sc__lookup(self, path, sep='.', _fullpath = None):
		path = self._sc__norm_path(path)

		if not _fullpath:
			_fullpath = path

		if len(path) == 0:
			return ('namespace', self)

		attr = path[0]
		if not hasattr(self, attr):
			raise AttributeError("Entity not found")
		obj = getattr(self, attr, None)

		retval = None

		if attr in self._sc__constants:
			retval = ('constant', obj)
		elif isinstance(obj, service_broadcast):
			retval = ('broadcast', obj)
		else:
			infos = getattr(obj, '_sc__service_entity', None)
			if len(path) == 1:
				if infos:
					retval = (infos['entity'], obj)
			elif hasattr(getattr(obj, '_sc__lookup', None), '__call__'): # duck typing
				return obj._sc__lookup(path[1:], sep, _fullpath)

		if not retval or len(path) > 1:
			raise NotFound("The entity '%s' doesn't exist" % (sep or '.').join(_fullpath))

		return retval

	def _sc__introspect_data(self):
		namespaces = list()
		constants = dict()
		methods = list()
		broadcasts = list()

		def _namespace(path, obj, entity):
			namespaces.append(entity['name'])
		def _constant(path, obj, entity):
			constants[entity['name']] = obj
		def _method(path, obj, entity):
			methods.append(obj._sc__introspect_data())
		def _broadcast(path, obj, entity):
			broadcasts.append(obj._sc__introspect_data())

		self._sc__walk(
			namespace = _namespace,
			constant = _constant,
			method = _method,
			broadcast = _broadcast,
			depth = 0, # We don't want recursion in this case
			)

		return dict(
			name = self._sc__service_entity['name'],
			docstring = self._sc__service_entity['doc'],
			constants = constants,
			namespaces = namespaces,
			methods = methods,
			broadcasts = broadcasts,
			)

def isservice(object):
	return getattr(object, '_sc__isservice', False)

class Service(Namespace):
	"""
	Service root namespace.

	Should always be instanciated for root namespace only. A RuntimeError will be
	raised if a sub-namespace inherits from it.

	Example:
	class MyServices(Service):
		class org(Namespace):
			class xmms2(Namespace):
				class example1(Namespace):
					pass
				class example2(Namespace):
				    pass
	"""

	# Flag checked instead of instance's class to allow service-like objects
	# that don't inherit from Service
	_sc__isservice = True

	def __init__(self, client):
		self._namespace_path = () # Enforce root service
		self.name = service_constant(client._clientname) # Resolved in super()
		super(Service, self).__init__(client)

		# Required by _sc__introspect_data()
		self._sc__service_entity = dict(
				entity = 'service',
				name = client._clientname,
				doc = self.__doc__ or ''
				)
		self._sc__path_cache = dict()
		self._sc__failed_cache = set()

	def _sc__lookup(self, path, sep='.'):
		"""
		Same as Namespace._sc__lookup(), but also maintains a cache of requested
		paths to speed up future lookups. The cache can be invalidated (cleared)
		by calling self._invalidate_cache() when the api is changed.
		"""
		path = self._sc__norm_path(path)

		if path in self._sc__path_cache:
			return self._sc__path_cache[path]
		elif path in self._sc__failed_cache:
			raise NotFound("The entity '%s' doesn't exists" % (sep or '.').join(path))

		try:
			retval = super(Service, self)._sc__lookup(path, sep)
		except NotFound:
			self._sc__failed_cache.add(path)
			raise

		self._sc__path_cache[path] = retval
		return retval

	def _sc__invalidate_cache(self, path = None, recursive = True):
		if path is None:
			self._sc__path_cache.clear()
			self._sc__failed_cache.clear()
		elif not recursive:
			if path in self._sc__path_cache:
				del self._sc__path_cache[path]
			elif path in self._sc__failed_cache:
				self._sc__failed_cache.remove(path)
		else:
			l = len(path)
			_cmp = lambda k: k[:l] == path
			for k in list(filter(_cmp, self._sc__path_cache)):
				del self._sc__path_cache[k]
			for k in list(filter(_cmp, self._sc__failed_cache)):
				self._sc__failed_cache.remove(k)

	def __delitem__(self, path = None):
		self._sc__invalidate_cache(path)
