#ifndef _IVM_VM_INLINE_CONTEXT_H_
#define _IVM_VM_INLINE_CONTEXT_H_

#include "pub/const.h"
#include "pub/com.h"
#include "pub/vm.h"

#include "std/string.h"

#include "../obj.h"
#include "../context.h"

IVM_COM_HEADER

#define GET_CONTEXT(chain_sub) ((chain_sub)->slots)

#define ivm_ctchain_free(chain, state) (ivm_vmstate_dumpContext((state), (chain)))

IVM_INLINE
ivm_ctchain_t *
ivm_context_pool_alloc(ivm_context_pool_t *pool, ivm_int_t len)
{
	ivm_ctchain_t *ret;

	if (len < IVM_CONTEXT_POOL_MAX_CACHE_LEN) {
		ret = (ivm_ctchain_t *)
			  ivm_ptpool_alloc(pool->pools[len]);
	} else {
		ret = MEM_ALLOC(ivm_ctchain_getSize(len),
						ivm_ctchain_t *);
		IVM_ASSERT(ret, IVM_ERROR_MSG_FAILED_ALLOC_NEW("context chain"));
	}

	ivm_ref_init(ret);
	ret->len = len;

	return ret;
}

IVM_INLINE
void
ivm_context_pool_dump(ivm_context_pool_t *pool,
					  ivm_ctchain_t *chain)
{
	if (chain && !ivm_ref_dec(chain)) {
		if (chain->len < IVM_CONTEXT_POOL_MAX_CACHE_LEN) {
			ivm_ptpool_dump(pool->pools[chain->len], chain);
		} else {
			MEM_FREE(chain);
		}
	}

	return;
}

IVM_INLINE
void
ivm_context_pool_dumpAll(ivm_context_pool_t *pool)
{
	ivm_ptpool_t **i, **end;

	for (i = pool->pools,
		 end = i + IVM_CONTEXT_POOL_MAX_CACHE_LEN + 1;
		 i != end; i++) {
		ivm_ptpool_dumpAll(*i);
	}

	return;
}

IVM_INLINE
ivm_object_t *
ivm_ctchain_search_cc(ivm_ctchain_t *chain,
					  ivm_vmstate_t *state,
					  const ivm_string_t *key,
					  ivm_instr_cache_t *cache)
{
	ivm_object_t *ret = IVM_NULL;
	ivm_context_t *i, *end;

	for (i = ivm_ctchain_contextStart(chain),
		 end = i + chain->len;
		 i != end; i++) {
#if IVM_USE_INLINE_CACHE
		ret = ivm_context_getSlot_cc(i, state, key, cache);
#else
		ret = ivm_context_getSlot(i, state, key);
#endif
		if (ret) break;
	}

	return ret;
}

IVM_INLINE
ivm_bool_t
ivm_ctchain_setExistSlot_cc(ivm_ctchain_t *chain,
							struct ivm_vmstate_t_tag *state,
							const ivm_string_t *key,
							ivm_object_t *val,
							ivm_instr_cache_t *cache)
{
	ivm_bool_t ret = IVM_FALSE;
	ivm_context_t *i, *end;

	for (i = ivm_ctchain_contextStart(chain),
		 end = i + chain->len;
		 i != end; i++) {

#if IVM_USE_INLINE_CACHE
		ret = ivm_context_setExistSlot_cc(i, state, key, val, cache);
#else
		ret = ivm_context_setExistSlot(i, state, key, val, cache);
#endif

		if (ret) break;
	}

	return ret;
}

#undef GET_CONTEXT

IVM_COM_END

#endif
