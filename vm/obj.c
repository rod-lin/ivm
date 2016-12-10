#include "pub/const.h"
#include "pub/com.h"
#include "pub/err.h"
#include "pub/vm.h"
#include "pub/inlines.h"

#include "std/mem.h"
#include "std/string.h"

#include "gc/gc.h"
#include "obj.h"
#include "instr.h"
#include "slot.h"

ivm_bool_t
ivm_object_toBool(ivm_object_t *obj,
				  ivm_vmstate_t *state)
{
	ivm_type_t *type = IVM_TYPE_OF(obj);
	ivm_bool_converter_t conv = type->to_bool;

	if (conv)
		return conv(obj, state);

	return type->const_bool;
}

void
ivm_object_setSlot(ivm_object_t *obj,
				   ivm_vmstate_t *state,
				   const ivm_string_t *key,
				   ivm_object_t *value)
{
	ivm_slot_table_t *slots;

	// IVM_ASSERT(obj, IVM_ERROR_MSG_OP_SLOT_OF_UNDEFINED("set"));

	slots = obj->slots;

	if (slots) {
		if (ivm_slot_table_isShared(slots)) {
			slots = obj->slots = ivm_slot_table_copyOnWrite(slots, state);
			IVM_WBOBJ_SLOT(state, obj, slots);
		}
	} else {
		slots = obj->slots = ivm_slot_table_newAt(state, IVM_OBJECT_GET(obj, GEN));
	}

	ivm_slot_table_setSlot(slots, state, key, value);

	return;
}

void
ivm_object_setSlot_r(ivm_object_t *obj,
					 ivm_vmstate_t *state,
					 const ivm_char_t *rkey,
					 ivm_object_t *value)
{
	ivm_slot_table_t *slots;

	// IVM_ASSERT(obj, IVM_ERROR_MSG_OP_SLOT_OF_UNDEFINED("set"));

	slots = obj->slots;

	if (slots) {
		if (ivm_slot_table_isShared(slots)) {
			slots = obj->slots = ivm_slot_table_copyOnWrite(slots, state);
			IVM_WBOBJ_SLOT(state, obj, slots);
		}
	} else {
		slots = obj->slots = ivm_slot_table_newAt(state, IVM_OBJECT_GET(obj, GEN));
	}

	ivm_slot_table_setSlot_r(slots, state, rkey, value);

	return;
}

void
ivm_object_setSlot_cc(ivm_object_t *obj,
					  struct ivm_vmstate_t_tag *state,
					  const ivm_string_t *key,
					  ivm_object_t *value,
					  ivm_instr_t *instr)
{
	ivm_slot_table_t *slots;

	// IVM_ASSERT(obj, IVM_ERROR_MSG_OP_SLOT_OF_UNDEFINED("set"));

	slots = obj->slots;

	if (slots) {
		if (ivm_slot_table_isShared(slots)) {
			slots = obj->slots = ivm_slot_table_copyOnWrite(slots, state);
			IVM_WBOBJ_SLOT(state, obj, slots);
		}
	} else {
		slots = obj->slots = ivm_slot_table_newAt(state, IVM_OBJECT_GET(obj, GEN));
	}

	ivm_slot_table_setSlot_cc(slots, state, key, value, instr);

	return;
}

IVM_INLINE
ivm_object_t *
_ivm_object_searchProtoSlot(ivm_object_t *obj,
							ivm_vmstate_t *state,
							const ivm_string_t *key)
{
	ivm_object_t *i = ivm_object_getProto(obj),
				 *ret = IVM_NULL;

	if (!i) return ret;

	/* no loop is allowed when setting proto */
	while (i) {
		if (i->slots) {
			ret = ivm_slot_getValue(
				ivm_slot_table_getSlot(i->slots, state, key),
				state
			);
		}
		if (ret) break;
		i = ivm_object_getProto(i);
	}

	return ret;
}

IVM_INLINE
ivm_object_t *
_ivm_object_searchProtoSlot_cc(ivm_object_t *obj,
							   ivm_vmstate_t *state,
							   const ivm_string_t *key,
							   ivm_instr_t *instr)
{
	ivm_object_t *i = ivm_object_getProto(obj),
				 *ret = IVM_NULL;

	if (!i) return ret;

	/* no loop is allowed when setting proto */
	while (i) {
		if (i->slots) {
			ret = ivm_slot_getValue(
				ivm_slot_table_getSlot_cc(i->slots, state, key, instr),
				state
			);
		}
		if (ret) break;
		i = ivm_object_getProto(i);
	}

	return ret;
}

ivm_object_t *
ivm_object_getSlot(ivm_object_t *obj,
				   ivm_vmstate_t *state,
				   const ivm_string_t *key)
{
	ivm_object_t *ret = IVM_NULL;
	ivm_slot_table_t *slots;

	// IVM_ASSERT(obj, IVM_ERROR_MSG_OP_SLOT_OF_UNDEFINED("get"));

	slots = obj->slots;

	if (slots) {
		ret = ivm_slot_getValue(
			ivm_slot_table_getSlot(slots, state, key),
			state
		);
	}

	if (!ret) {
		ret = _ivm_object_searchProtoSlot(obj, state, key);
	}

	return ret;
}

ivm_object_t *
ivm_object_getSlot_r(ivm_object_t *obj,
					 ivm_vmstate_t *state,
					 const ivm_char_t *rkey)
{
	ivm_object_t *ret = IVM_NULL;
	const ivm_string_t *key;
	ivm_slot_table_t *slots;

	// IVM_ASSERT(obj, IVM_ERROR_MSG_OP_SLOT_OF_UNDEFINED("get"));

	key = ivm_vmstate_constantize_r(state, rkey);

	slots = obj->slots;

	if (slots) {
		ret = ivm_slot_getValue(
			ivm_slot_table_getSlot(slots, state, key),
			state
		);
	}

	if (!ret) {
		ret = _ivm_object_searchProtoSlot(obj, state, key);
	}

	return ret;
}

ivm_object_t *
ivm_object_getSlot_cc(ivm_object_t *obj,
					  ivm_vmstate_t *state,
					  const ivm_string_t *key,
					  ivm_instr_t *instr)
{
	ivm_object_t *ret = IVM_NULL;
	ivm_slot_table_t *slots;

	// IVM_ASSERT(obj, IVM_ERROR_MSG_OP_SLOT_OF_UNDEFINED("get"));

	slots = obj->slots;
	if (slots) {
		ret = ivm_slot_getValue(
			ivm_slot_table_getSlot_cc(slots, state, key, instr),
			state
		);
	}

	if (!ret) {
		ret = _ivm_object_searchProtoSlot_cc(obj, state, key, instr);
	}

	return ret;
}

ivm_bool_t
ivm_object_setEmptySlot_r(ivm_object_t *obj,
						  ivm_vmstate_t *state,
						  const ivm_char_t *rkey,
						  ivm_object_t *value)
{
	ivm_slot_table_t *slots;

	slots = obj->slots;

	if (slots) {
		if (ivm_slot_table_isShared(slots)) {
			slots = obj->slots = ivm_slot_table_copyOnWrite(slots, state);
			IVM_WBOBJ_SLOT(state, obj, slots);
		}
	} else {
		slots = obj->slots = ivm_slot_table_newAt(state, IVM_OBJECT_GET(obj, GEN));
	}

	return ivm_slot_table_setEmptySlot_r(slots, state, rkey, value);
}

void
ivm_object_setOop(ivm_object_t *obj,
				  ivm_vmstate_t *state,
				  ivm_int_t op,
				  ivm_object_t *func)
{
	ivm_slot_table_t *slots = obj->slots;

	if (slots) {
		if (ivm_slot_table_isShared(slots)) {
			slots = obj->slots = ivm_slot_table_copyOnWrite(slots, state);
			IVM_WBOBJ_SLOT(state, obj, slots);
		}
	} else {
		slots = obj->slots = ivm_slot_table_newAt(state, IVM_OBJECT_GET(obj, GEN));
	}

	ivm_object_markOop(obj);
	ivm_slot_table_setOop(slots, state, op, func);

	return;
}

void
ivm_object_printSlots(ivm_object_t *obj)
{
	ivm_slot_table_iterator_t iter;
	ivm_size_t ecount = 0; /* empty count */

	IVM_TRACE("IVM_DEBUG slots in object %p(slot table using %s)\n",
			  (void *)obj, obj->slots && obj->slots->mark.sub.is_hash ? "hash table" : "list");

	if (obj->slots) {
		IVM_SLOT_TABLE_EACHPTR(obj->slots, iter) {
			if (IVM_SLOT_TABLE_ITER_GET_KEY(iter)) {
				IVM_TRACE("\tkey %s: %p\n",
						  ivm_string_trimHead(IVM_SLOT_TABLE_ITER_GET_KEY(iter)),
						  (void *)IVM_SLOT_TABLE_ITER_GET_VAL(iter));
			} else {
				ecount++;
			}
		}

		if (obj->slots->mark.sub.is_hash) {
			IVM_TRACE("\thash table load factor: %f\n",
					  (double)ecount / obj->slots->size);
		}
	} else {
		IVM_TRACE("\tnone\n");
	}

	return;
}
