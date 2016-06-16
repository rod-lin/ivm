#ifndef _IVM_VM_VM_H_
#define _IVM_VM_VM_H_

#include "pub/com.h"
#include "pub/const.h"
#include "pub/type.h"

#include "std/pool.h"
#include "std/string.h"
#include "std/heap.h"

#include "gc/gc.h"
#include "obj.h"
#include "num.h"
#include "coro.h"
#include "context.h"

IVM_COM_HEADER

typedef struct ivm_vmstate_t_tag {
	ivm_heap_t *cur_heap;
	ivm_heap_t *empty_heap;

	ivm_size_t cur_coro;
	ivm_coro_list_t *coro_list;

	ivm_type_list_t *type_list;
	ivm_func_list_t *func_list;

	ivm_function_pool_t *func_pool;
	ivm_context_pool_t *ct_pool;
	ivm_frame_pool_t *fr_pool;

	ivm_string_pool_t *const_pool;

	ivm_int_t gc_flag; /* gc flag:
						  > 0: open
						  = 0: closed
						  < 0: locked */
	ivm_collector_t *gc;
} ivm_vmstate_t;

#define IVM_VMSTATE_GET_CUR_CORO(state) (ivm_coro_list_at((state)->coro_list, (state)->cur_coro))
#define IVM_VMSTATE_GET_CORO_LIST(state) ((state)->coro_list)
#define IVM_VMSTATE_GET_TYPE_LIST(state) ((state)->type_list)
#define IVM_VMSTATE_GET_CONST_POOL(state) ((state)->const_pool)
#define IVM_VMSTATE_GET_CUR_HEAP(state) ((state)->cur_heap)
#define IVM_VMSTATE_GET_EMPTY_HEAP(state) ((state)->empty_heap)

#define IVM_VMSTATE_GET(obj, member) IVM_GET((obj), IVM_VMSTATE, member)
#define IVM_VMSTATE_SET(obj, member, val) IVM_SET((obj), IVM_VMSTATE, member, (val))

ivm_vmstate_t *
ivm_vmstate_new();

void
ivm_vmstate_free(ivm_vmstate_t *state);

#define ivm_vmstate_isGCFlagOpen(state) ((state)->gc_flag > 0)

#define ivm_vmstate_openGCFlag(state) ((state)->gc_flag < 0 ? 0 : ((state)->gc_flag = 1))
#define ivm_vmstate_closeGCFlag(state) ((state)->gc_flag < 0 ? 0 : ((state)->gc_flag = 0))

#define ivm_vmstate_lockGCFlag(state) ((state)->gc_flag = -1)
#define ivm_vmstate_unlockGCFlag(state) ((state)->gc_flag = 0)

#if 0

#define ivm_vmstate_curHeap(state) ((state)->heap_count > 0 ? (state)->heaps[(state)->heap_count - 1] : IVM_NULL)
void
ivm_vmstate_addHeap(ivm_vmstate_t *state);
ivm_heap_t *
ivm_vmstate_findHeap(ivm_vmstate_t *state, ivm_object_t *obj);

#endif

IVM_INLINE
void *
ivm_vmstate_alloc(ivm_vmstate_t *state, ivm_size_t size)
{
	ivm_bool_t add_block = IVM_FALSE;
	void *ret = ivm_heap_alloc_c((state)->cur_heap, size, &add_block);

	if (add_block) {
		ivm_vmstate_openGCFlag(state);
	}

	return ret;
}

IVM_INLINE
void
ivm_vmstate_swapHeap(ivm_vmstate_t *state)
{
	ivm_heap_t *tmp = state->cur_heap;
	state->cur_heap = state->empty_heap;
	state->empty_heap = tmp;

	return;
}

/* function pool */
#if IVM_USE_FUNCTION_POOL

#define ivm_vmstate_allocFunc(state) \
	(ivm_function_pool_alloc((state)->func_pool))
#define ivm_vmstate_dumpFunc(state, func) \
	(ivm_function_pool_dump((state)->func_pool, (func)))

#else

#define ivm_vmstate_allocFunc(state) \
	(MEM_ALLOC(sizeof(ivm_function_t), ivm_function_t *))
#define ivm_vmstate_dumpFunc(state, func) \
	(MEM_FREE(func))

#endif

/* frame pool */
#if IVM_USE_FRAME_POOL

#define ivm_vmstate_allocFrame(state) \
	(ivm_frame_pool_alloc((state)->fr_pool))
#define ivm_vmstate_dumpFrame(state, fr) \
	(ivm_frame_pool_dump((state)->fr_pool, (fr)))

#else

#define ivm_vmstate_allocFrame(state) \
	(MEM_ALLOC(sizeof(ivm_frame_t), ivm_frame_t *))
#define ivm_vmstate_dumpFrame(state, fr) \
	(MEM_FREE(fr))

#endif

#if 0

ivm_object_t *
ivm_vmstate_newObject(ivm_vmstate_t *state);
void
ivm_vmstate_freeObject(ivm_vmstate_t *state, ivm_object_t *obj);

#endif

#define ivm_vmstate_registerType(state, type) (ivm_type_list_register((state)->type_list, (type)))
#define ivm_vmstate_getType(state, tag) (ivm_type_list_at((state)->type_list, (tag)))
#define ivm_vmstate_getTypeProto(state, tag) \
	(ivm_type_getProto(ivm_vmstate_getType((state), (tag))))
#define ivm_vmstate_getTypeHeader(state, tag) \
	(ivm_type_getHeader(ivm_vmstate_getType((state), (tag))))

#define ivm_vmstate_registerFunc(state, exec) (ivm_func_list_register((state)->func_list, (exec)))
#define ivm_vmstate_getFunc(state, id) (ivm_func_list_at((state)->func_list, (id)))

#define ivm_vmstate_addDesLog(state, obj) (ivm_collector_addDesLog((state)->gc, (obj)))

#if 0 && IVM_DEBUG

#define ivm_vmstate_checkGC(state) (ivm_collector_collect((state)->gc, (state), (state)->cur_heap))

#else

#define ivm_vmstate_checkGC(state) \
	ivm_vmstate_isGCFlagOpen(state)

#define ivm_vmstate_doGC(state) \
	ivm_collector_collect((state)->gc, (state), (state)->cur_heap); \
	ivm_vmstate_closeGCFlag(state);

#endif

#define ivm_vmstate_addCoro(state, coro) (ivm_coro_list_add((state)->coro_list, (coro)))

void
ivm_vmstate_schedule(ivm_vmstate_t *state);

/*
ivm_object_t *
ivm_vmstate_execute();
*/

IVM_COM_END

#endif
