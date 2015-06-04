# encoding: utf-8
"""
Interprocess call (ipc) protocol for xmms2
"""

import os
import io
import struct
import math
import asyncio
import socket
from inspect import getmembers

try:
	io.BytesIO.getbuffer
except AttributeError:
	RuntimeError("Python version >= 3.2 required")

try:
	async = asyncio.ensure_future
except AttributeError: # ensure_future introduced in 3.4.4
	async = asyncio.async

__all__ = [
		'Message',
		'MessageResult',
		'Ipc',
		'IpcObject',
		]

from . import types

from .log import logger
log = logger.getChild('ipc')

class Message(io.BytesIO):
	"""
	IPC Message

	Header (bytes):
	 01 02 03 04 05 06 07 08 09 10 11 12 13 14 15 16
	+--.--.--.--+--.--.--.--+--.--.--.--+--.--.--.--+
	|  object   |  command  |  cookie   | data len  |
	+--'--'--'--+--'--'--'--+--'--'--'--+--'--'--'--+

	Followed by serialized data bytes
	"""
	OFFSET_OBJECT = 0
	OFFSET_COMMAND = 4
	OFFSET_COOKIE = 8
	OFFSET_LENGTH = 12
	OFFSET_DATA = 16

	_header = struct.Struct('!IIII')
	_U32 = struct.Struct('!I')
	_I32 = struct.Struct('!i')
	_I64 = struct.Struct('!q')
	_F = struct.Struct('!II')

	def __init__(self, obj = 0, cmd = 0, **kargs):
		super(Message, self).__init__()
		self.reset(obj, cmd)
		self._packing = False
		if 'data' in kargs:
			self.pack(kargs['data'])

	def reset(self, obj, cmd):
		""" Reset the message state. """
		self.seek(0)
		self.write(self._header.pack(obj, cmd, 0, 0))
		self.truncate()

	@property
	def object(self):
		""" Get the object id """
		o = self.unpack_uint32(self.OFFSET_OBJECT)
		return o

	@property
	def command(self):
		""" Get the command id """
		c = self.unpack_uint32(self.OFFSET_COMMAND)
		return c

	def _get_cookie(self):
		c = self.unpack_uint32(self.OFFSET_COOKIE)
		return c
	def _set_cookie(self, cookie):
		self.pack_uint32(cookie, self.OFFSET_COOKIE)
	def _del_cookie(self):
		self.pack_uint32(0, self.OFFSET_COOKIE)
	cookie = property(_get_cookie, _set_cookie, _del_cookie, """
		Get, set or delete the cookie id.
	""".strip())
	del _get_cookie, _set_cookie, _del_cookie

	@property
	def length(self):
		""" Get the length of the message serialized data. """
		l = self.unpack_uint32(self.OFFSET_LENGTH)
		return l

	def _update_length(self):
		""" Update the data length field from current buffer length. """
		_buffer = self.getbuffer()
		l = max(0, len(_buffer) - self.OFFSET_DATA)
		self._U32.pack_into(_buffer, self.OFFSET_LENGTH, l)

	def _pack_struct(self, st, args, offset = None):
		""" Serialize data based on struct infos, with an optional write offset. """
		if offset is None:
			self.write(st.pack(*args))
		else:
			st.pack_into(self.getbuffer(), offset, *args)
	def _unpack_struct(self, st, offset = None):
		""" Deserialize data based on struct infos, with an optional read offset. """
		if offset is None:
			return st.unpack(self.read(st.size))
		else:
			return st.unpack_from(self.getbuffer(), offset)

	def pack_uint32(self, i, offset = None):
		""" Serialize an unsigned 32 bits integer. """
		self._pack_struct(self._U32, (i,), offset)
	def unpack_uint32(self, offset = None):
		""" Deserialize an unsigned 32 bits integer. """
		i = self._unpack_struct(self._U32, offset)[0]
		return i

	def pack_int32(self, i, offset = None):
		""" Serialize a signed 32 bits integer. """
		self._pack_struct(self._I32, (i,), offset)
	def unpack_int32(self, offset = None):
		""" Deserialize a signed 32 bits integer. """
		i = self._unpack_struct(self._I32, offset)[0]
		return i

	def pack_int64(self, i, offset = None):
		""" Serialize a signed 64 bits integer. """
		self._pack_struct(self._I64, (i,), offset)
	def unpack_int64(self, offset = None):
		""" deserialize a signed 64 bits integer. """
		i = self._unpack_struct(self._I64, offset)[0]
		return i

	def pack_float(self, f, offset = None):
		""" Serialize a floating point value. """
		mantis, exp = math.frexp(f)
		if mantis > 0:
			mantis = int(mantis * (2**31 - 1))
		else:
			mantis = int(mantis * (2**31))
		self._pack_struct(self._F, (mantis, exp), offset)
	def unpack_float(self, offset = None):
		""" Deserialize a floating point value. """
		mantis, exp = self._unpack_struct(self._F, offset)
		if mantis < 0:
			mantis = mantis / (2**31 - 1)
		else:
			mantis = mantis / (2**31)
		f = mantis * 2**exp
		return f

	def pack_bytes(self, b, offset = None, nts = False):
		""" Serialize a bytes object. """
		if b is None:
			self.pack_uint32(0, offset)
			return

		l = len(b)
		if nts:
			l += 1

		self.pack_uint32(l, offset)
		if offset is not None:
			offset += self._U32.size
			p = self.tell()
			self.seek(offset)
		self.write(b)
		if nts:
			self.write(b'\x00')
		if offset is not None:
			self.seek(p)
	def unpack_bytes(self, offset = None, nts = False):
		""" Deserialize a bytes object. """
		l = self.unpack_uint32(offset)
		if not l:
			return None
		if nts:
			l -= 1
		if offset is not None:
			offset += self._U32.size
			p = self.tell()
			self.seek(offset)
		b = self.read(l)
		if offset is not None:
			self.seek(p)
		elif nts:
			self.read(1)
		return b

	def pack_string(self, s, offset = None):
		""" Serialize a string object using utf-8 encoding."""
		bs = str(s).encode('utf-8')
		self.pack_bytes(bs, offset, True)
	def unpack_string(self, offset = None):
		""" Deserialize a string object using utf-8 decoding. """
		bs = self.unpack_bytes(offset, True)
		s = bs.decode('utf-8')
		return s

	def pack_list(self, iterable):
		""" Serialize a list of values.
		If #iterable provides constraints informations, it is used to reduce serialized
		data length.
		"""
		restrict = getattr(iterable, '__restrictedtype__', types.NONE)
		self.pack_uint32(restrict)
		sizepos = self.tell()
		size = 0
		# size placeholder: This is required because the iterable may not be
		# resetable.
		self.pack_uint32(0)
		for value in iterable:
			size += 1
			vtype = types.typeof(value)
			if restrict != types.NONE and vtype != restrict:
				raise TypeError("Violation of type restriction: %r expected, got %r" %
						types.typename((restrict, vtype)))
			self.pack(value, restrict != types.NONE)
		# Now we know the size of the list.
		self.pack_uint32(size, sizepos)
	def unpack_list(self):
		""" Deserialize a list of values.

		Return a list or a RestrictedList instance, depending on serialized type
		constraints.
		"""
		restrict = self.unpack_uint32()
		size = self.unpack_uint32()

		if restrict != types.NONE:
			l = types.rlist(type=restrict)
		else:
			l = []

		while size > 0:
			v = self.unpack(restrict or None)
			l.append(v)
			size -= 1
		return l

	def pack_dict(self, dct):
		""" Serialize a dictionary-like object. """
		size = 0
		sizepos = self.tell()
		# Dict size place holder.
		self.pack_uint32(0)
		if hasattr(dct, 'keys') and hasattr(dct.keys, '__call__'):
			it = dct.keys()
		else:
			it = dct
		for k in it:
			size += 1
			self.pack_string(k)
			self.pack(dct[k])
		# Now, we know the size of the dict.
		self.pack_uint32(size, sizepos)
	def unpack_dict(self):
		""" Deserialize a dictionary. """
		size = self.unpack_uint32()
		d = dict()
		while size > 0:
			k = self.unpack_string()
			v = self.unpack()
			d[k] = v
			size -= 1
		return d

	def pack_collection(self, coll):
		""" Serialize a collection object. """
		# Late import to prevent cyclic import
		from .collections import Ids
		self.pack_uint32(coll.__type__)
		self.pack_dict(coll.attributes)
		self.pack_list(coll.ids or Ids())
		self.pack_list(coll.operands)
	def unpack_collection(self):
		""" Deserialize a collection object. """
		# Late import to prevent cyclic import
		from .collections import Collection
		ctype = self.unpack_uint32()
		attrs = self.unpack_dict()
		attrs['ids'] = self.unpack_list()
		attrs['operands'] = unpack_list()
		CollClass = Collection.get_class(ctype)
		if not CollClass:
			raise ValueError("Unknown collection type (%r)" % ctype)
		c = CollClass(**attrs)
		return c

	def pack(self, value, ignoretype = False, cleardata = False):
		""" Serialize an arbitrary value.

		An object serialized type is obtained by calling types.typeof().
		A custom object should provide __ipctype__ and implement __ipcpack__()
		in order to be serialized. (__ipctype__ may be sufficient if the object is
		api-compatible to that type)
		"""
		update_length = not self._packing

		if cleardata and update_length:
			self.seek(self.OFFSET_DATA)
			self.truncate()

		self._packing = True

		vtype = types.typeof(value)

		if vtype is not None and not ignoretype:
			self.pack_uint32(vtype)

		if hasattr(value, '__ipcpack__') and hasattr(value.__ipcpack__, '__call__'):
			# Allow any type to be serialized as one of the supported types.
			value.__ipcpack__(self)
		elif vtype == types.NONE:
			pass
		elif vtype == types.ERROR or vtype == types.STRING:
			self.pack_string(value)
		elif vtype == types.INTEGER:
			self.pack_int64(value)
		elif vtype == types.COLLECTION:
			self.pack_collection(value)
		elif vtype == types.BINARY:
			self.pack_bytes(value)
		elif vtype == types.LIST:
			self.pack_list(value)
		elif vtype == types.DICTIONARY:
			self.pack_dict(value)
		elif vtype == types.FLOAT:
			self.pack_float(value)
		else:
			raise ValueError("value of type %r can't be serialized" % value.__class__.__name__)
		if update_length:
			self._update_length()
			self._packing = False


	def unpack(self, forcetype = None, restart = False):
		""" Deserialize an arbitrary value. """
		if restart:
			self.seek(self.OFFSET_DATA)

		if forcetype is None:
			forcetype = self.unpack_uint32()

		if forcetype == types.NONE:
			return None
		elif forcetype == types.ERROR:
			err = self.unpack_string()
			return types.Error(err)
		elif forcetype == types.INTEGER:
			return self.unpack_int64()
		elif forcetype == types.STRING:
			return self.unpack_string()
		elif forcetype == types.COLLECTION:
			return self.unpack_collection()
		elif forcetype == types.BINARY:
			return self.unpack_bytes()
		elif forcetype == types.LIST:
			return self.unpack_list()
		elif forcetype == types.DICTIONARY:
			return self.unpack_dict()
		elif forcetype == types.FLOAT:
			return self.unpack_float()

		raise ValueError("Unsupported type (%r) from serialized data" % forcetype)

	def unpack_all(self):
		""" Unpack all values in the message payload. """
		endpos = self.length + self.OFFSET_DATA
		self.seek(self.OFFSET_DATA)
		args = []
		while self.tell() < endpos:
			args.append(self.unpack())
		return tuple(args)

	def write_into(self, writer):
		""" Write serialized message in a writable. """
		writer.write(self.getvalue())

	def __str__(self):
		return "<Message: {0.object}|{0.command}|{0.cookie}|{0.length}>".format(self)

	def __repr__(self):
		return "<Message: object={0.object}, command={0.command}, cookie={0.cookie}, len={0.length}, payload={1!r}>".format(self, self.unpack_all())

	@classmethod
	@asyncio.coroutine
	def new_from(cls, reader):
		""" Create a new message from data reader. """
		msg = cls()
		header = yield from reader.readexactly(msg.OFFSET_DATA)
		msg.seek(0)
		msg.write(header)
		l = msg.length
		if l > 0:
			data = yield from reader.readexactly(l)
			msg.write(data)
			msg.seek(msg.OFFSET_DATA)
		return msg


class MessageResult:
	def __init__(self, msg, ipc, return_type = None):
		self._cookie = msg.cookie
		self._type = msg.object and 'normal' or (msg.command and 'broadcast' or 'signal')
		self._ipc = ipc
		self._return_type = return_type
		self._callbacks = list()
		self._result = None

		# needed to request the signal again automatically if callbacks are still up.
		if self._type == 'signal':
			self._msg = msg

	def __len__(self):
		return len(self._callbacks)

	def __call__(self, result):
		self._result = result
		i = 0
		for i in range(len(self)):
			cb = self._callbacks.pop(0)
			try:
				r = cb(result)
			except Exception:
				log.exception("Unhandled exception in result callback")
				r = False
			if r is None or r:
				# Reinsert the callbacks.
				self._callbacks.append(cb)

		# Normal results receive at most one reply
		if not len(self):
			self._ipc.unwatch_result(self)
		elif self._type == 'signal':
			self._ipc.queue_msg(self._msg, override_cookie = False)

	@property
	def cookie(self):
		return self._cookie

	@property
	def type(self):
		return self._type

	def bind(self, *callbacks):
		for c in callbacks:
			if c in self._callbacks:
				continue
			self._callbacks.append(c)
		self._ipc.watch_result(self)

	def unbind(self, *callbacks):
		for c in callbacks:
			try:
				self._callbacks.remove(c)
			except ValueError:
				pass
		if not len(self):
			self.unwatch_result(self)

	@asyncio.coroutine
	def get(self):
		evt = self._ipc.create_event()
		def on_result(_v):
			evt.set()
		self.bind(on_result)
		yield from evt.wait()
		if isinstance(self._result, types.Error):
			raise self._result
		return self._result


class Ipc:
	DEFAULT_HOST = '127.0.0.1'
	DEFAULT_PORT = 9667
	DEFAULT_UNIX_PATH = '/tmp/xmms-ipc-%s' % os.getenv('USER')

	message_factory = Message
	result_factory = MessageResult

	@classmethod
	@asyncio.coroutine
	def conn_tcp(cls, netloc, path, loop):
		"""
		tcp://host[:port]
		"""
		host, x, port = netloc.partition(':')
		host = host or cls.DEFAULT_HOST
		port = int(port) if port else cls.DEFAULT_PORT
		return asyncio.open_connection(host, port, loop = loop)

	@classmethod
	@asyncio.coroutine
	def conn_unix(cls, netloc, path, loop):
		"""
		unix:///path/to/socket.ipc
		"""
		path = path or cls.DEFAULT_UNIX_PATH
		return asyncio.open_unix_connection(path, loop = loop)
	if not hasattr(socket, 'AF_UNIX'): # unix socket not supported
		del conn_unix

	@classmethod
	@asyncio.coroutine
	def conn_default(cls, netloc, path, loop):
		try:
			factory = cls.conn_unix
		except AttributeError:
			factory = cls.conn_tcp
		return factory('', '', loop)

	@classmethod
	@asyncio.coroutine
	def create(cls, uri = None, loop = None):
		from urllib.parse import urlparse
		result = urlparse(uri or 'default:')
		factory = getattr(cls, 'conn_'+result.scheme, None)
		if not factory:
			raise NotImplementedError("%s scheme not supported" % result.scheme)
		conn_coro = factory(result.netloc, result.path, loop)
		reader, writer = yield from conn_coro
		return cls(reader, writer, loop)

	def cookie_ids(self):
		cookie = 0
		while True:
			cookie += 1
			yield cookie

	def __init__(self, reader, writer, loop = None):
		self._reader = reader
		self._writer = writer
		self._loop = loop
		self.cookies = self.cookie_ids()
		self._results = dict()
		self._task = None
		self._on_unknown = list()

	def watch_result(self, result):
		self._results[result._cookie] = result

	def unwatch_result(self, result):
		try:
			del self._results[result._cookie]
		except KeyError:
			pass

	def create_event(self):
		return asyncio.Event(loop = self._loop)

	def queue_msg(self, msg, override_cookie = True):
		if override_cookie or not msg.cookie:
			msg.cookie = next(self.cookies)
		log.debug(">> Message queued: {!r}".format(msg))
		msg.write_into(self._writer)
		return msg.cookie

	@asyncio.coroutine
	def send_msg(self, msg):
		self.queue_msg(msg)
		yield from self._writer.drain()

	@asyncio.coroutine
	def read_msg(self):
		msg = yield from self.message_factory.new_from(self._reader)
		return msg

	@asyncio.coroutine
	def handle_next(self):
		msg = yield from self.read_msg()
		cookie = msg.cookie
		res = self._results.get(cookie)
		if res:
			log.debug("<< Handling message: %r", msg)
			res(msg.unpack())
		else:
			log.debug("!! Unwatched message: %r", msg)

	@asyncio.coroutine
	def handle(self):
		try:
			while True:
				yield from self.handle_next()
		except asyncio.CancelledError:
			pass
		except Exception: # Internal flow exceptions derive from BaseException directly.
			log.exception("Error in ipc input handler")
			self.cancel()

	def schedule(self):
		if not self._task:
			self._task = async(self.handle(), loop = self._loop)
			self._task.add_done_callback(lambda f: self.close())
		return self._task

	def cancel(self):
		task = self._task
		if task:
			task.cancel()
			self._task = None
		return task

	def close(self):
		self._writer.close()
		self._reader = None
		self._writer = None

	def get_api(self, api_factory = None):
		if not api_factory:
			from .ipcobjects import Api
			api_factory = Api
		return api_factory(self)


class IpcObjectManager(type):
	def __new__(cls, name, bases, namespace, ipcname=None, ipcid=None, **kw):
		Obj = type.__new__(cls, name, bases, namespace)
		if ipcname:
			Obj.__ipcname__ = ipcname
		if ipcid:
			Obj.__ipcid__ = ipcid
		return Obj

	def __init__(cls, *a, **ka): # Required because of keyword arguments
		return type.__init__(cls, *a)

class IpcObject(metaclass=IpcObjectManager):
	def __init__(self, ipc):
		if not (getattr(self, '__ipcname__', None) and getattr(self, '__ipcid__', None)):
			raise NotImplementedError("Can't instanciate abstract class")
		self.ipc = ipc

	def __getitem__(self, key):
		return getattr(self, key)

class ApiBase:
	def __init__(self):
		self._revlist = {
			'0': ("<sig/bc>", "unknown"),
			# Special case for signals subscribe functions
			'0:32': ("<signal>", ""),
			# Special case for broadcasts subscribe functions
			'0:33': ("<broadcast>", "")
		}

		for o in self: # List IPC objects
			self._revlist['%s' % o.__class__.__ipcid__] = (o.__class__.__name__, "unknown")
			for k, m in getmembers(o, lambda x: hasattr(x, '__revid__')):
				self._revlist[m.__revid__] = (o.__class__.__name__, m.__name__)

	def api_info(self, msg):
		""" Get a human readable representation of the message.
		Don't include payload contents (except for broadcasts and signals)

		Example:
		header(hex): 00000004 00000020 00000009 00000000
		=> Playback.start [cookie=9, len=0]
		"""
		oid = msg.object
		cid = msg.command
		obj, cmd = self._revlist.get("%s:%s" % (oid, cid), (None, None))
		if obj is None:
			obj = self._revlist.get("%s" % oid, ("Unknown", "unknown"))

		return "{0}.{1} [cookie={2}, len={3}]".format(obj, cmd, msg.cookie, msg.length)
