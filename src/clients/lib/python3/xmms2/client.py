# encoding: utf-8

__all__ = [
		'Client',
		'Error',
		]

from os import getenv

from .ipc import asyncio, async
from .types import Error

from .log import logger

from . import ipcobjects
from inspect import getmembers

_excl = ('IPC_', 'C2C_', 'COLLECTION_TYPE_')
for _n, _c in getmembers(ipcobjects, lambda o: isinstance(o, int)):
	if any(map(lambda e: _n.startswith(e), _excl)):
		continue
	locals()[_n] = _c
del _excl, _n, _c

class Client:
	PROTOCOL_VERSION = ipcobjects.IPC_PROTOCOL_VERSION

	from .ipc import Ipc as ipc_factory

	@property
	def loop(self):
		return self._loop

	@property
	def id(self):
		return self._id

	def __init__(self, clientname = None, loop = None):
		self._clientname = clientname or self.__class__.__qualname__
		self._loop = loop or asyncio.get_event_loop()
		self._reset_state()

	def _reset_state(self):
		self.ipc = None
		self._id = None
		self._task = None
		self._bcsig_results = dict()
		self._ready_f = asyncio.Future(loop = self._loop)
		self._api = None
		self._apitask = None
		self._uri = None

	def _cancel_all(self):
		self._ready_f.cancel()
		if self._apitask:
			self._apitask.cancel()

	def _init_async_post(self):
		"""
		Called by _init_async once the connection is established,
		but before the ready state is set. Should call self._cancel_all()
		in case of error.
		Overridden by some mixins.
		"""
		pass

	@asyncio.coroutine
	def _init_async(self):
		uri = self._uri or getenv('XMMS_PATH')
		self.ipc = yield from self.ipc_factory.create(uri, self._loop)
		self._api = self.ipc.get_api()
		self._apitask = self.ipc.schedule() # start input listener
		self._apitask.add_done_callback(lambda f: self.disconnect())
		result = self._api.main.hello(self.PROTOCOL_VERSION, self._clientname)
		try:
			self._id = yield from result.get()
		except Error as e:
			logger.error('%s', e)
			self._cancel_all()
			return

		self._init_async_post()

		if not self._ready_f.done():
			self._ready_f.set_result(self._id)

	def ready(self):
		"""
		Called once the server replied to the hello command.
		Subclasses should override this method.
		"""
		pass

	def disconnect(self):
		"""
		Called when the connection is lost or closed.
		"""
		pass

	def schedule(self, *callbacks, uri = None):
		"""
		Schedule callbacks to run once the connection is ready.
		Connection initialization task is created the first time the function
		is called.
		The method accepts the keyword parameter #uri to define the server to
		connect to. If not provided, the uri is obtained from the environment
		variable XMMS_PATH if set, or fallback to internal default otherwise.
		This parameter is ignored after the first call to schedule().
		"""
		if not self._task and not self._ready_f.done():
			self._uri = uri
			self._task = async(self._init_async(), loop = self._loop)
			def done(f):
				if f.cancelled():
					self.disconnect()
				else:
					self.ready()
			self._ready_f.add_done_callback(done)

		for cb in callbacks:
			self._ready_f.add_done_callback(lambda f: f.cancelled() or cb())

		return self._task

	def _cancel_tasks(self):
		"""
		Cancel internal tasks and return a list of thoses cancelled.
		"""
		tasks = []
		if self._task and not self._task.done():
			self._task.cancel()
			tasks.append(self._task)
		if self.ipc:
			tasks.append(self.ipc.cancel())
		return tasks


	def shutdown(self, wait = False):
		"""
		Cancel all running tasks and exits
		"""
		if wait and self._loop.is_running():
			raise RuntimeError("Can only wait on tasks on an exited loop")

		wait_for = self._cancel_tasks()

		if wait:
			self._loop.run_until_complete(asyncio.wait(wait_for))
		self._reset_state()

		return (wait or None) and wait_for

	def _api_parse(self, target, ctx=''):
		"""
		Parse an api lookup expression.
		See self.bind() for supported patterns for #target.

		Return the tuple (obj, name, full, canon) where
		  - ctx is the context of the target ('', 'brodcast' or 'signal')
		  - obj is the object name
		  - name is the context-stripped function name
		  - full is the real function name
		  - canon is a canonical representation of the 
		"""
		_ctx, x, path = target.partition(':')
		if not path:
			if x:
				raise ValueError("Invalid target pattern")
			path = _ctx
		else:
			ctx = _ctx
		obj, x, name = path.partition('.')
		if not name:
			if x:
				raise ValueError("Invalid target pattern")
			name = obj
			obj = None
		full = '%s%s%s' % (name, ctx and '_' or '', ctx or '')
		if not obj:
			for obj in self._api:
				try:
					obj[full]
					obj = obj.__ipcname__
					break
				except AttributeError:
					pass
			else:
				raise AttributeError("Target '%s:%r' not found" % (ctx, path))
		canon = '%s:%s.%s' % (ctx, obj, name)
		return ctx, obj, name, full, canon

	def bind(self, target, *callbacks, type='broadcast'):
		"""
		Bind callbacks to a broadcast or a signal.
		Each event is registered automatically the first time a callback is
		bound to it.
		#target is of the form '[<ctx>:][[<object>].]<name>'
		If <ctx> is omitted, it fallbacks to #type ('broadcast' by default).
		"""
		type, obj, name, full, canon = self._api_parse(target, type)

		if type not in ('broadcast', 'signal'):
			raise ValueError("type must be either 'broadcast' or 'signal'")

		result = self._bcsig_results.get(canon)

		if not result:
			fun = self._api[obj][full]
			result = fun()
			self._bcsig_results[canon] = result

		result.bind(*callbacks)

	def unbind(self, target, *callbacks, type='broadcast'):
		"""
		Unbind callbacks attached to a broadcast or a signal.
		The current protocol doesn't allow for broadcast unregistration. So,
		to prevent garbage messages from the server, the result is kept alive
		for future bind() calls, even if all callbacks were unbound.
		"""
		type, obj, name, full, canon = self._api_parse(target, type)
		result = self._bcsig_results.get(canon)
		if result:
			result.unbind(*callbacks)
			if type != 'signal' and not result._callbacks:
				del self._bcsig_results[canon]

	def __call__(self, target, *args, **kargs):
		type, obj, name, full, canon = self._api_parse(target)
		return self._api[obj][full](*args, **kargs)

	def run(self):
		self.schedule()
		try:
			self.loop.run_forever()
		except KeyboardInterrupt:
			pass
		self.shutdown(True)
		self.loop.close()


# test client
if __name__ == '__main__':
	import logging
	logging.basicConfig()
	logger.setLevel(logging.WARNING)

	def check_error(f):
		def _wrap(*a): #trick to support both direct methods and class methods
			if isinstance(a[-1], Exception):
				print("!!! Error [%s]: %s" % (f.__name__, a[-1]), flush = True)
				return
			return f(*a)
		_wrap.__name__ = f.__name__
		_wrap.__doc__ = f.__doc__
		return _wrap

	class MonitoringClient(Client):
		STATUS = {
			PLAYBACK_STATUS_STOP: 'stopped',
			PLAYBACK_STATUS_PLAY: 'playing',
			PLAYBACK_STATUS_PAUSE: 'paused',
			}

		def ready(self):
			print("=== Connected with client id #%d" % self.id, flush = True)

			# Bind broadcasts
			self.bind('main.quit', lambda res: self.disconnect())
			self.bind('playback.status', self.playback_status)
			self.bind('playback.current_id', self.playback_id)

			# Query initial infos
			self('main.stats').bind(self.stats)
			self('playback.status').bind(self.playback_status)
			self('playback.current_id').bind(self.playback_id)

		def disconnect(self):
			if self.loop.is_running():
				self.loop.stop()

		@check_error
		def stats(self, stats):
			print("Server stats:")
			for k, v, in stats.items():
				print("\t%s = %r" % (k, v))
			print(flush = True)

		@check_error
		def playback_status(self, status):
			status = self.STATUS.get(status, 'unknown (%r)' % (status,))
			print(">>> playback status: %s" % (status,), flush = True)

		@check_error
		def playback_id(self, id):
			if id == 0:
				return
			print(">>> current id: %d" % id, flush = True)
			self("medialib.get_info", id).bind(self.media_info)

		@check_error
		def media_info(self, infos):
			for k, d in infos.items():
				for s, v in d.items():
					print("\t[%s] %s = %r" % (s, k, v))
			print(flush = True)


	client = MonitoringClient('org.xmms2.python.MonitoringClient')
	client.run()
