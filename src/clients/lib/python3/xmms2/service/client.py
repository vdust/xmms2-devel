# encoding: utf-8

__all__ = [
		'ServiceClientMixIn',
		]

from .local import lookup, isservice
from .errors import NotFound

from .. import ipcobjects
from ..types import Error

from ..ipc import asyncio, async

from ..log import logger
log = logger.getChild('service.client')

from inspect import getmembers
for _n, _c in getmembers(ipcobjects, lambda o: isinstance(o, int)):
	if _n.startswith('C2C_'):
		locals()[_n] = _c
del _n, _c

_keyname = lambda k: 'libxmmsclient-sc-'+k

COMMAND_KEY = _keyname('command')
ARGUMENTS_KEY = _keyname('args')

SC_COMMAND_CALL = 0
SC_COMMAND_BROADCAST_SUBSCRIBE = 1
SC_COMMAND_INTROSPECT = 2

SC_CALL_METHOD_KEY = _keyname('call-method')
SC_CALL_PARGS_KEY = _keyname('call-pargs')
SC_CALL_NARGS_KEY = _keyname('call-nargs')

SC_INTROSPECT_PATH_KEY = _keyname('introspect-path')
SC_INTROSPECT_TYPE_KEY = _keyname('introspect-type')
SC_INTROSPECT_KEYFILTER_KEY = _keyname('introspect-keyfilter')

_entity_type_map = dict(
	namespace = 0,
	method = 1,
	broadcast = 2
	)

class _Ignore(Exception): pass

class MessageError(Error):
	def __init__(self, msg='Unknown error'):
		super(MessageError, self).__init__("Erroneous c2c message: %s" % msg)

def _check_path(p, target):
	_not_str = lambda s: not isinstance(s, str)
	p = tuple(p)
	if not isinstance(p, (tuple, list)) or any(map(_not_str, p)):
		raise MessageError("%s path must be a list of strings" % target)
	return p

def _noop(*a, **kw):
	pass

class ServiceClientMixIn:
	"""
	MixIn class to handle local and remote c2c APIs.
	Use it like this:

		from xmms2.client import Client
		from xmms2.service.client import ServiceClientMixIn

		class MyClient(ServiceClientMixIn, Client): pass

	Note: ServiceClientMixIn must be listed before Client for proper methods
	chaining with super()

	__service_factory__ can be set in subclass to instanciate automatically a
	service on ipc initialization. It can be any callable as long as it
	accepts the client instance as unique argument and return an instance of
	Service (or subclass) or an iterable that returns Service instzances.
	"""

	__service_factory__ = None

	def _reset_state(self):
		super(ServiceClientMixIn, self)._reset_state()
		self._services = []
		self._services_initialized = False
		self._services_broadcasts_subscribers = dict()
		self._service_async_method_calls = set()

	def _init_async_post(self):
		super(ServiceClientMixIn, self)._init_async_post()
		if self.__service_factory__:
			service = self.__service_factory__(self)
			self.service_add(service)

	def _cancel_tasks(self):
		tasks = super(ServiceClientMixIn, self)._cancel_tasks()
		for f in self._service_async_method_calls:
			if not f.done():
				f.cancel()
				tasks.append(f)
		return tasks

	def _service_init(self):
		if not self._services_initialized:
			self.bind('courier.message', self._on_c2c_message)
			self._services_initialized = True

	def _on_c2c_message(self, message):
		if isinstance(message, Exception):
			log.warning("%s", message)
			return True # ignore exceptions

		try:
			reply = self._on_c2c_message_build_reply(message)
		except _Ignore:
			return True
		except Error as e:
			reply = e
		except Exception as e:
			reply = Error("Unexpected error: %s" % e)
			log.exception("Unexpected error while handling c2c message:")

		self._api.courier.reply(message['id'],
			ipcobjects.C2C_REPLY_POLICY_NO_REPLY,
			reply).bind(_noop)

		return True

	def _on_c2c_message_build_reply(self, message):
		error = None
		msgid = message['id']
		if not isinstance(msgid, int) or msgid <= 0:
			raise _Ignore()
		try:
			_src = 'message'
			payload = message['payload']
			_src = 'payload'
			cmd = payload[COMMAND_KEY]
			args = payload[ARGUMENTS_KEY]
			cmd = int(cmd)
		except TypeError as e:
			raise MessageError("%s must be dictionaries." % _src) from None
		except KeyError as e:
			raise MessageError("%r missing in %s" % (e.args[0], _src)) from None
		except ValueError as e:
			raise MessageError("Command must be an integer." % e) from None

		if cmd == SC_COMMAND_CALL:
			return self._service_c2c_call(msgid, args)
		elif cmd == SC_COMMAND_BROADCAST_SUBSCRIBE:
			return self._service_c2c_broadcast_subcribed(msgid, args)
		elif cmd == SC_COMMAND_INTROSPECT:
			return self._service_c2c_introspect(args)
		else:
			raise MessageError("Unknown command: %d" % cmd)

	def _service_c2c_call(self, msgid, args):
		path = args.get(SC_CALL_METHOD_KEY, None)
		pargs = args.get(SC_CALL_PARGS_KEY, ())
		nargs = args.get(SC_CALL_NARGS_KEY, {})

		if not path:
			raise Error("Erroneous c2c message: missing method path" % e.args)

		path = _check_path(path, "method")

		try:
			entity, method = self.service_lookup(path)
		except NotFound as e:
			raise Error("%s" % e) from None

		if not hasattr(method, '__call__') or entity != 'method':
			raise Error("%s is not a method" % ".".join(path))

		if not isinstance(pargs, (tuple, list)):
			raise MessageError("method pargs must be a list")

		if not isinstance(nargs, dict):
			raise MessageError("method nargs must be a dict")

		ret = method(*pargs, **nargs)

		if asyncio.iscoroutine(method):
			#Coroutines will be executed asynchronously.
			task = async(self._service_c2c_call_async(msgid, ret), loop=self.loop)
			self._service_async_method_calls.add(task)
			raise _Ignore()
		else:
			return ret

	@asyncio.coroutine
	def _service_c2c_call_async(self, msgid, coro):
		try:
			reply = yield from coro
		except Error as e:
			reply = e
		except asyncio.CancelledError:
			# Will occur only in cases the client exits.
			raise
		except Exception as e:
			reply = Error("Unexpected error: %s" % e)
			log.exception("Unexpected error during asynchronous method call:")

		self._api.courier.reply(msgid,
			ipcobjects.C2C_REPLY_POLICY_NO_REPLY,
			reply)

		try:
			self._service_async_method_calls.remove(asyncio.Task.current_task())
		except KeyError:
			pass

	def _service_c2c_broadcast_subscribed(self, msgid, path):
		if msgid <= 0:
			return

		path = _check_path(path, "broadcast")
		try:
			entity, broadcast = self.service_lookup(path)
		except NotFound as e:
			raise Error("%s" % e) from None
		if entity != 'broadcast':
			raise Error("%s is not a broadcast" % ".".join(path))

		_set = self._services_broadcasts_subscribers.get(path, set())
		self._services_broadcasts_substribers[path] = _set
		_set.add(msgid)

	def _service_c2c_introspect(self, args):
		path = args.get(SC_INTROSPECT_PATH_KEY, None)
		etype = args.get(SC_INTROSPECT_TYPE_KEY, None)
		keyfilter = args.get(SC_INTROSPECT_KEYFILTER_KEY, ())

		path = _check_path(path, "entity")
		try:
			entity, object = self.service_lookup(path)
		except NotFound as e:
			raise Error("%s" % e) from None

		if entity == 'constant':
			# reference C implementation for introspection doesn't support
			# direct access to constants values. One need to use keyfilter to
			# retrieve it.
			raise Error("Constants have no introspection data.")

		if etype is not None and _entity_type_map.get(entity) != etype:
			raise Error("Entity type mismatch")

		# All introspectable objects have this method.
		reply = object._sc__introspect_data()

		for k in keyfilter or ():
			reply = reply[k]

		return reply

	def service_emit_broadcast(self, path, value):
		_set = self._services_broadcasts_subscribers.get(path, set())
		for msgid in _set:
		    self._api.courier.reply(msgid,
				ipcobjects.C2C_REPLY_POLICY_NO_REPLY,
				value)

	def service_ready(self):
		self._service_init()
		if self._api:
			self._api.courier.ready().bind(_noop)

	def service_add(self, service, first = False):
		if service is None:
			return

		if isservice(service):
			service = (service,)

		inserted = set()
		i = 0
		need_ready = False

		for s in service:
			if s._sc__client is not self:
				raise RuntimeError("Client bound to service doesn't match current client.")

		for s in service:
			if s in inserted: # Already inserted. Ignore.
				continue

			if s in self._services:
				if first:
					self._services.remove(s)
				else:
					continue

			if first:
				self._services.insert(i, s)
				inserted.add(s)
				i += 1
			else:
				self._services.append(s)
			need_ready = True

		if need_ready:
			self.service_ready()

	def service_remove(self, service):
		if service is None or not self._services:
			return

		if isservice(service):
			service = (service,)

		need_ready = False
		for s in service:
			try:
				self._services.remove(s)
			except ValueError:
				continue
			need_ready = True

		if need_ready:
			if not self._services_initialized:
				self.bind('courier.message', self._on_c2c_message)
				self._services_initialized = True
			self.service_ready()

	def service_lookup(self, path):
		for s in self._services:
			try:
				return lookup(s, path)
			except NotFound:
				continue
		raise NotFound("%s not found" % ".".join(path))

	def sc_call(self, clientid, path, args=(), kargs=dict()):
		msg = {
			COMMAND_KEY: SC_COMMAND_CALL,
			ARGUMENTS_KEY: {
				SC_CALL_METHOD_KEY: path,
				SC_CALL_PARGS_KEY: args,
				SC_CALL_NARGS_KEY: kargs,
			},
		}

		return self._api.courier.send_message(clientid, C2C_REPLY_POLICY_SINGLE_REPLY, msg)

	def sc_introspect(self, clientid, path, entity_type = None):
		args = dict()
		path = list(path)

		if entity_type == 'constant':
			if not path:
				raise ValueError("Path required for constant lookup")
			etype = _entity_type_map['namespace']
			args[SC_INTROSPECT_KEYFILTER_KEY] = ['constants', path[-1]]
			path = path[:-1]
		elif entity_type is None:
			etype = None
		else:
			try:
				etype = _entity_type_map[entity_type]
			except KeyError:
				raise ValueError("Invalid entity_type. Must be one of: constant, %s" %
						", ".join(map(repr, _entity_type_map))
						) from None

		if etype:
			args[SC_INTROSPECT_TYPE_KEY] = etype

		args[SC_INTROSPECT_PATH_KEY] = path

		msg = {
			COMMAND_KEY: SC_COMMAND_INTROSPECT,
			ARGUMENTS_KEY: args,
		}

		return self._api.courier.send_message(clientid, C2C_REPLY_POLICY_SINGLE_REPLY, msg)

	def sc_broadcast_subscribe(clientid, path):
		path = list(path)

		if not path:
			raise ValueError("Path must not be empty")

		msg = {
			COMMMAND_KEY: SC_COMMAND_BROADCAST_SUBSCRIBE,
			ARGUMENTS_KEY: list(path),
		}

		return self._api.courier.send_message(clientid, C2C_REPLY_POLICY_MULTI_REPLY, msg)

	def __contains__(self, path):
		try:
			self.service_lookup(path)
			return True
		except NotFound:
			return False


if __name__ == '__main__':
	from .. import client
	from .local import *
	from .remote import *

	import logging
	logging.basicConfig()
	logger.setLevel(logging.DEBUG)

	class ServiceFinder(Namespace):
		version = service_constant(1)

		def __init__(self, *a, **kw):
			super(ServiceFinder, self).__init__(*a, **kw)
			self._sc__service_clients = dict()
			self._sc__client.bind('courier.ready', self._sc__client_add)
			self._sc__client.bind('ipc_manager.client_disconnected', self._sc__client_remove)
		def _sc__client_add(self, id):
			if id not in self._sc__service_clients:
				self._sc__service_clients[id] = ServiceClient(self._sc__client, id)

		def _sc__client_remove(self, id):
			if id in self._sc__service_clients:
				del self._sc__service_clients[id]

		@service_method(
			[ method_arg('path', 'list'),
			  method_arg('type', 'string'),
			],
			[ method_arg('timeout', 'float', default_value = 10.0) ],
		)
		@asyncio.coroutine
		def who(self, path, timeout = 10.0):
			tasks = []
			for service in self._sc__service_clients.values():
				t = async(service.introspect(path, 'namespace').get())
				tasks.append(t)

			done, pending = yield from asyncio.wait(tasks, timeout=timeout)
			for t in pending:
				t.cancel()
			yield from asyncio.wait(pending) # ensure clean up
			ids = set()
			for t in done:
				if t.cancelled() or t.exception():
					continue
				res = t.result()
				if not isinstance(res, dict):
					continue
				if not 'id' in res:
					continue
				if 'payload' in res and not isinstance(res['payload'], Exception):
					ids.add(res['id'])
			return sorted(ids)

	class Example(ServiceClientMixIn, client.Client):
		class __service_factory__(Service):
			class org(Namespace):
				class xmms2(Namespace):
					ServiceFinder = ServiceFinder

	cli = Example('org.xmms2.client.ServiceFinder')
	cli.run()
