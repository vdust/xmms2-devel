# encoding: utf-8

__all__ = [
		'ServiceClient'
		]

class client_method:
	def __init__(self, parent, name, doc, infos, cb = None):
		self._parent = parent
		self._callback = cb
		self._path = parent._path + (name,)
		self._clientid = parent._sc__clientid
		self.name = name
		self.doc = doc
		self.infos = infos

	def with_callback(self, cb):
		return client_method(self._parent, self.name, self.doc, self.infos, cb)

	def call(self, args=(), kargs=dict(), cb = None):
		# TODO: arguments checking
		res = self._parent._client.sc_call(self._clientid, self._path, args, kargs)
		if cb:
			res.bind(cb)
		return res

	def __call__(self, *args, **kargs):
		return self.call(args, kargs, cb = self._callback)

class client_broadcast:
	def __init__(self, parent, name, doc, infos = None):
		self._parent = parent
		self._path = parent._path + (name,)
		self._clientid = parent._sc__clientid
		self.name = name
		self.doc = doc

	def subscribe(self, *callbacks):
		res = self._parent._client.sc_broadcast_subscribe(self._clientid, self._path)
		if callbacks:
			res.bind(*callbacks)
		return res

	def __call__(self, *callbacks):
		return self.subscribe(*callbacks)


class ServiceClient:
	def __init__(self, client, clientid, path = ()):
		self._sc__client = client
		self._sc__path = tuple(path)
		self._sc__clientid = clientid

	def _sc__set_api_item(self, factory, data):
		try:
			name = data['name']
			doc = data.get('docstring', None)
		except (TypeError, AttributeError, KeyError):
			return
		if hasattr(self.__class__, name):
			return
		setattr(self, name, factory(self, name, doc, data))

	def _sc__update_api(self, data, recursive = False, callback = None):
		if data.get('sender', -1) != self._sc__clientid:
			# ignore bad sender
			return

		payload = data.get('payload', None)
		if not payload:
			return

		self.__doc__ = payload.get('docstring', '')

		try:
			for c, v in payload.get('constants', {}).items():
				if hasattr(self.__class__, c):
					continue
				setattr(self, c, v)
		except (AttributeError, TypeError):
			pass

		for bc in payload.get('broadcasts', ()):
			self._sc__set_api_item(client_broadcast, bc)

		for m in payload.get('methods', ()):
			self._sc__set_api_item(client_method, m)

		for ns in payload.get('namespaces', ()):
			if hasattr(self.__class__, ns):
				continue
			sc = ServiceClient(self._sc__client, self._sc__clientid, path = self._path + (ns,))
			setattr(self, ns, sc)
			if recursive:
				sc(recursive, callback)

	def _sc__update_api_callback(self, callback = None, recursive = False):
		def _update_api(data):
			self._sc__update_api(data, recursive, callback)
			if callback:
				callback(self, value)
		return _update_api

	def __call__(self, recursive = False, callback = None):
		callback = self._sc__update_api_callback(callback, recursive)
		res = self._sc__client.sc_introspect(self._sc__clientid, self._sc__path, 'namespace')
		res.bind(callback)
		return res

	def introspect(self, path, entity_type = None):
		return self._sc__client.sc_introspect(self._sc__clientid, self._sc__path + path, entity_type)
