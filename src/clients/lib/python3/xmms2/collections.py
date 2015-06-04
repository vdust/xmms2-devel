# encoding: utf-8

__all__ = [
		'Reference',
		'Universe',
		'Union',
		'Intersection',
		'Complement',
		'Has',
		'Equals',
		'NotEqual',
		'Match',
		'Token',
		'Smaller',
		'SmallerEqual',
		'Greater',
		'GreaterEqual',
		'Order',
		'Mediaset',
		'Limit',
		'IDList',
		'Queue',
		'PShuffle',
		]

from . import types
from . import ipcobjects # Needed for collection types
from inspect import getmembers

class Attributes(dict):
	pass

class Operands(types.RestrictedList):
	__restrictedtype__ = types.COLLECTION
	def __init__(self, a=None, limits=(0, -1)):
		super(Operands, self).__init__()
		self._limits = tuple(limits)
		if a:
			self.extend(a)

class Ids(types.RestrictedList):
	__restrictedtype__ = types.INTEGER
	def __init__(self, a=None):
		super(Ids, self).__init__()
		if a:
			self.extend(a)


class Collection:
	# Copy collection type constants from ipcobjects
	for _name, _const in getmembers(ipcobjects, lambda o: isinstance(o, int)):
		if name.startswith('COLLECTION_TYPE_') and name != 'COLLECTION_TYPE_LAST':
			locals()[name[16:]] = _const
	del _name, _const

	__collections__ = dict()

	__iscollection__ = True # For type checking
	__type__ = None
	__ids__ = False
	__operands__ = (0, -1)

	def __init__(self, **attrs):
		if self.__type__ is None:
			raise RuntimeError("Can't instanciate %r directly" % self.__class__.__name__)

		self._operands = Operands(attrs.pop('operands', ()), limits = self.__operands__)

		ids = attrs.pop('ids', ())
		if self.__ids__:
			self._ids = Ids(map(int, ids or ()))
		else:
			self._ids = None

		self._attributes = Attributes(**attrs)

	def _get_attributes(self):
		return self._attributes
	def _set_attributes(self, d):
		self._attributes.clear()
		self._attributes.update(v)
	def _del_attributes(self):
		self._attributes.clear()
	attributes = property(_get_attributes, _set_attributes, _del_attributes)
	del _get_attributes, _set_attributes, _del_attributes

	def _get_operands(self):
		return self._operands
	def _set_operands(self, ops):
		self._operands.clear()
		self._operands.extend(ops)
	def _del_operands(self):
		self._operands.clear()
	operands = property(_get_operands, _set_operands, _del_operands)
	del _get_operands, _set_operands, _del_operands

	def _get_ids(self):
		return self._ids
	def _set_ids(self, ids):
		if self._ids is None:
			raise TypeError("Can't set idlist for this type of collection")
		self._ids.clear()
		self._ids.extend(ids)
	def _del_ids(self):
		if self._ids is None:
			raise TypeError("Can't set idlist for this type of collection")
		self._ids.clear()
	ids = property(_get_ids, _set_ids, _del_ids)
	del _get_ids, _set_ids, _del_ids

	def copy(self):
		return self.__class__(
				ids = self._ids,
				operands = [o.copy() for o in self._operands],
				**self._attributes)

	def __repr__(self):
		decl = []
		operands = []
		if self._ids is not None:
			decl.append("ids=%r" % self.ids)
		for o in self.operands:
			operands.append(repr(o))
		if operands:
			decl.append("operands=[%s]" % ", ".join(operands))
		for k, v in self.attributes.items():
			decl.append("%s=%s" % (k, repr(v)))
		return "%s(%s)" % (self.__class__.__name__, ", ".join(decl))

	def __or__(self, other): # |
		if not (isinstance(self, Collection) and isinstance(other, Collection)):
			raise TypeError("Unsupported operand type(s) for |: %r and %r"
					% (self.__class__.__name__, other.__class__.__name__))
		u = Union()
		for op in (self, other):
			if isinstance(op, Union):
				u.operands.extend(op.operands)
			else:
				u.operands.append(op)
		return u

	def __and__(self, other): # &
		if not (isinstance(self, Collection) and isinstance(other, Collection)):
			raise TypeError("Unsupported operand type(s) for &: %r and %r"
					% (self.__class__.__name__, other.__class__.__name__))
		i = Intersection()
		for op in (self, other):
			if isinstance(op, Intersection):
				i.operands.extend(op.operands)
			else:
				i.operands.append(op)
		return i

	def __invert__(self): #~
		if isinstance(self, Complement):
			return self.operands[0]
		return Complement(self)

	@classmethod
	def _register(cls, collclass):
		if collclass.__type__ is not None:
			cls.__collections__[collclass.__type__] = collclass
		return collclass

	@classmethod
	def get_class(cls, type):
		return cls.__collections__.get(type)


@Collection._register
class Reference(Collection):
	__type__ = Collection.REFERENCE
	__operands__ = (0, 0)

	def __init__(self, reference = "All Media", namespace = "Collections", **attrs):
		attrs.update(
				reference = reference,
				namespace = namespace
				)
		super(Reference, self).__init__(**attrs)
		if reference == "All media":
			self.__class__ = Universe

@Collection._register
class Universe(Reference):
	__type__ = Collection.UNIVERSE
	def __init__(self):
		super(Universe, self).__init__()


class Filter(Collection):
	__operands__ = (1, 1)
	def __init__(self, parent = None, **attrs):
		if 'operands' in attrs:
			operands = attrs.pop('operands')
		elif parent is None:
			operands = [Universe()]
		else:
			operands = [parent]

		super(Filter, self).__init__(
				operands = operands,
				**attrs)

@Collection._register
class Equals(Filter):
	__type__ = Collection.EQUALS

@Collection._register
class NotEqual(Filter):
	__type__ = Collection.NOTEQUAL

@Collection._register
class Smaller(Filter):
	__type__ = Collection.SMALLER

@Collection._register
class SmallerEqual(Filter):
	__type__ = Collection.SMALLEREQ

@Collection._register
class Greater(Filter):
	__type__ = Collection.GREATER

@Collection._register
class GreaterEqual(Filter):
	__type__ = Collection.GREATEREQ

@Collection._register
class Match(Filter):
	__type__ = Collection.MATCH

@Collection._register
class Token(Filter):
	__type__ = Collection.TOKEN

@Collection._register
class Has(Filter):
	__type__ = Collection.TOKEN


@Collection._register
class Order(Collection):
	__type__ = Collection.ORDER
	__operands__ = (1, 1)

	def __init__(self, operand = None, field = None, direction = 0, **attrs):
		if isinstance(direction, int):
			attrs['direction'] = 'ASC' if direction > 0 else 'DESC'
		else:
			direction = str(direction).upper()
			if direction not in ('ASC', 'DESC'):
				raise TypeError("'direction must be an integer or one of ASC, DESC")
			attrs['direction'] = direction

		attrs['operands'] = attrs.get('operands', [operand or Universe()])

		if field == 'id':
			attrs['type'] = 'id'
		elif field:
			attrs['field'] = str(field)
			attrs['type'] = 'value'
		else:
			attrs['type'] = attrs.get('type', 'random')

		if attrs['type'] == 'random':
			del attrs['direction']

		super(Order, self).__init__(**attrs)


@Collection._register
class Mediaset(Collection):
	__type__ = Collection.MEDIASET
	__operands__ = (1, 1)

	def __init__(self, operand = None, **attrs):
		attrs['operands'] = attrs.get('operands', [operand or Universe()])
		super(Mediaset, self).__init__(**attrs)


@Collection._register
class Limit(Collection):
	__type__ = Collection.LIMIT
	__operands__ = (1, 1)

	def __init__(self, operand = None, start = 0, length = 0, **attrs):
		attrs.update(
				operands = attrs.get('operands', [operand or Universe()]),
				start = int(start),
				length = int(length)
				)
		super(Limit, self).__init__(**attrs)


@Collection._register
class IDList(Collection):
	__type__ = Collection.IDLIST
	__operands__ = (0, 0)
	__ids__ = True

	def __init__(self, ids = None, **attrs):
		type = attrs.get('type', 'list')
		attrs.update(
				type = type,
				ids = ids
				)
		if type == 'queue':
			self.__class__ = Queue
		elif type == 'pshuffle':
			self.__class__ = PShuffle
		super(IDList, self).__init__(**attrs)

	def __iadd__(self, iterable):
		self.ids.extend(iterable)
		return self

class Queue(IDList):
	def __init__(self, ids = None, **attrs):
		attrs.update(
				type = attrs.get('type', 'queue')
				)
		super(Queue, self).__init__(**attrs)

class PShuffle(IDList):
	__operands__ = (1, 1)

	def __init__(self, ids = None, parent = None, **attrs):
		attrs.update(
				type = attrs.get('type', 'pshuffle'),
				operands = attrs.get('operands', [parent or Universe()])
				)
		super(PShuffle, self).__init__(self, ids, **attrs)


@Collection._register
class Union(Collection):
	__type__ = Collection.UNION

	def __init__(self, *_a, **attrs):
		attrs['operands'] = attrs.get('operands', list(_a))
		super(Union, self).__init__(**attrs)


@Collection._register
class Intersection(Collection):
	__type__ = Collection.INTERSECTION

	def __init__(self, *_a, **attrs):
		attrs['operands'] = attrs.get('operands', list(_a))
		super(Union, self).__init__(**attrs)


@Collection._register
class Complement(Collection):
	__type__ = Collection.COMPLEMENT
	__operands__ = (1, 1)

	def __init__(self, operand = None, **attrs):
		attrs['operands'] = attrs.get('operands', [operand or Universe()])
		super(Complement, self).__init__(**attrs)
