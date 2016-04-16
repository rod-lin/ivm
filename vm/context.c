#include "pub/mem.h"
#include "context.h"
#include "err.h"

#define GET_CONTEXT(chain_sub) ((chain_sub)->ct)

typedef struct ivm_ctchain_sub_t_tag ivm_ctchain_sub_t;

static
ivm_ctchain_sub_t *
ivm_ctchain_sub_new(ivm_context_t *ct)
{
	ivm_ctchain_sub_t *ret = MEM_ALLOC(sizeof(*ret));

	IVM_ASSERT(ret, IVM_ERROR_MSG_FAILED_ALLOC_NEW("sub context chain"));

	ret->ct = ct;
	ret->outer = ret->inner = IVM_NULL;

	return ret;
}

static
void
ivm_ctchain_sub_free(ivm_ctchain_sub_t *chain_sub)
{
	MEM_FREE(chain_sub);
	return;
}

#define FOREACH(i, chain) for ((i) = (chain)->head; (i); (i) = (i)->inner)
#define RFOREACH(i, chain) for ((i) = (chain)->tail; (i); (i) = (i)->outer)

ivm_ctchain_t *
ivm_ctchain_new()
{
	ivm_ctchain_t *ret = MEM_ALLOC(sizeof(*ret));

	IVM_ASSERT(ret, IVM_ERROR_MSG_FAILED_ALLOC_NEW("context chain"));

	ret->head = ret->tail = IVM_NULL;

	return ret;
}

void
ivm_ctchain_free(ivm_ctchain_t *chain)
{
	ivm_ctchain_sub_t *i, *tmp;

	if (chain) {
		for (i = chain->head; i;) {
			tmp = i;
			i = i->inner;
			ivm_ctchain_sub_free(tmp);
		}
		MEM_FREE(chain);
	}

	return;
}

void
ivm_ctchain_addContext(ivm_ctchain_t *chain,
					   ivm_context_t *ct)
{
	ivm_ctchain_sub_t *n_sub = ivm_ctchain_sub_new(ct);

	if (!chain->tail) {
		chain->head = chain->tail = n_sub;
	} else {
		chain->tail->inner = n_sub;
		n_sub->outer = chain->tail;
		chain->tail = n_sub;
	}

	return;
}

void
ivm_ctchain_removeContext(ivm_ctchain_t *chain,
						  ivm_context_t *ct)
{
	ivm_ctchain_sub_t *i;

	RFOREACH (i, chain) {
		if (i->ct == ct) {
			if (i->outer)
				i->outer->inner = i->inner;

			if (i->inner)
				i->inner->outer = i->outer;

			if (i == chain->head)
				chain->head = i->inner;

			if (i == chain->tail)
				chain->tail = i->outer;

			ivm_ctchain_sub_free(i);

			break;
		}
	}

	return;
}

ivm_object_t *
ivm_ctchain_search(ivm_ctchain_t *chain,
				   struct ivm_vmstate_t_tag *state,
				   const ivm_char_t *key)
{
	ivm_object_t *ret = IVM_NULL;
	ivm_ctchain_sub_t *i;

	RFOREACH (i, chain) {
		if ((ret = ivm_object_getSlotValue(GET_CONTEXT(i), state, key))
			!= IVM_NULL)
			break;
	}

	return ret;
}

ivm_ctchain_t *
ivm_ctchain_clone(ivm_ctchain_t *chain)
{
	ivm_ctchain_t *ret;
	ivm_ctchain_sub_t *i;

	if (!chain)
		return IVM_NULL;

	ret = ivm_ctchain_new();

	FOREACH (i, chain) {
		ivm_ctchain_addContext(ret, GET_CONTEXT(i));
	}

	return ret;
}