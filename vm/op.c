#include "pub/const.h"
#include "op.h"
#include "coro.h"
#include "vmstack.h"
#include "vm.h"
#include "call.h"
#include "err.h"

#define OP_PROC_NAME(op) ivm_op_proc_##op
#define OP_PROC(op) ivm_action_t OP_PROC_NAME(op)(ivm_vmstate_t *__ivm_state__, ivm_coro_t *__ivm_coro__)
												  /* these arguments shouldn't be used directly */

#define CORO (__ivm_coro__)
#define RUNTIME (__ivm_coro__->runtime)
#define PC (__ivm_coro__->runtime->pc)
#define STATE (__ivm_state__)
#define CALL_STACK (__ivm_coro__->call_st)

#define STACK (__ivm_coro__->stack)
#define STACK_SIZE() (ivm_vmstack_size(__ivm_coro__->stack))
#define STACK_TOP() (ivm_vmstack_top(__ivm_coro__->stack))
#define STACK_POP() (ivm_vmstack_pop(__ivm_coro__->stack))
#define STACK_PUSH(obj) (ivm_vmstack_push(__ivm_coro__->stack, (obj)))

#define CALL_STACK_TOP() (ivm_call_stack_top(__ivm_coro__->call_st))
#define CHECK_STACK(req) IVM_ASSERT((STACK_SIZE() - ivm_caller_info_stackTop(CALL_STACK_TOP())) \
									 >= (req), \
									IVM_ERROR_MSG_INSUFFICIENT_STACK)

#define OP_MAPPING(op) { IVM_OP(op), OP_PROC_NAME(op) }

OP_PROC(NONE)
{
	printf("NONE\n");
	PC++;
	return IVM_ACTION_NONE;
}

OP_PROC(NEW_OBJ)
{
	STACK_PUSH(ivm_object_new(STATE));
	PC++;
	return IVM_ACTION_NONE;
}

OP_PROC(GET_SLOT)
{
	printf("GET_SLOT: no implementation\n");
	PC++;
	return IVM_ACTION_NONE;
}

OP_PROC(SET_SLOT)
{
	printf("SET_SLOT: no implementation\n");
	PC++;
	return IVM_ACTION_NONE;
}

OP_PROC(GET_CONTEXT_SLOT)
{
	printf("GET_CONTEXT_SLOT: no implementation\n");
	PC++;
	return IVM_ACTION_NONE;
}

OP_PROC(SET_CONTEXT_SLOT)
{
	printf("SET_CONTEXT_SLOT: no implementation\n");
	PC++;
	return IVM_ACTION_NONE;
}

OP_PROC(PRINT)
{
	ivm_object_t *obj;

	/* printf("%ld, %ld\n", STACK_SIZE(), ivm_caller_info_stackTop(CALL_STACK_TOP())); */

	CHECK_STACK(1);

	obj = STACK_POP();
	printf("print: %p\n", (void *)obj);
	PC++;

	return IVM_ACTION_NONE;
}

OP_PROC(INVOKE)
{
	printf("INVOKE: no implementation\n");
	PC++;
	return IVM_ACTION_NONE;
}

OP_PROC(TEST1)
{
#if 0
	ivm_exec_t *exec = ivm_exec_new();
	ivm_function_t *func = ivm_function_new(IVM_NULL, exec, IVM_INTSIG_NONE);

	ivm_exec_addCode(exec, IVM_OP(NEW_OBJ), 0);
	ivm_exec_addCode(exec, IVM_OP(PRINT), 0);
	ivm_exec_addCode(exec, IVM_OP(TEST2), 0);
	
	PC++;
	ivm_call_stack_push(CALL_STACK, ivm_function_invoke(func, CORO));
#endif

	PC++;

	return IVM_ACTION_NONE;
}

OP_PROC(TEST2)
{ 
	ivm_exec_t *exec = ivm_exec_new();
	ivm_function_t *func = ivm_function_new(IVM_NULL, exec, IVM_INTSIG_NONE);

	ivm_exec_addCode(exec, IVM_OP(NEW_OBJ), 0);
	ivm_exec_addCode(exec, IVM_OP(PRINT), 0);
	ivm_exec_addCode(exec, IVM_OP(TEST3), 0);
	printf("this is a test string\n");
	
	PC++;
	ivm_call_stack_push(CALL_STACK, ivm_function_invoke(func, CORO));

	return IVM_ACTION_NONE;
}

OP_PROC(TEST3)
{
	printf("morning!\n");
	PC++;
	return IVM_ACTION_NONE;
}

OP_PROC(LAST)
{
	printf("LAST\n");
	PC++;
	return IVM_ACTION_NONE;
}

static const
ivm_op_table_t
ivm_global_op_table[] = {
	OP_MAPPING(NONE),
	OP_MAPPING(NEW_OBJ),
	OP_MAPPING(GET_SLOT),
	OP_MAPPING(SET_SLOT),
	OP_MAPPING(GET_CONTEXT_SLOT),
	OP_MAPPING(SET_CONTEXT_SLOT),
	OP_MAPPING(PRINT),
	OP_MAPPING(INVOKE),
	OP_MAPPING(TEST1),
	OP_MAPPING(TEST2),
	OP_MAPPING(TEST3),
	OP_MAPPING(LAST)
};

ivm_op_proc_t
ivm_op_table_getProc(ivm_opcode_t op)
{
#if IVM_DEBUG
	IVM_ASSERT(op < IVM_OP_LAST,
			   IVM_ERROR_MSG_BAD_OP);
	IVM_ASSERT(ivm_global_op_table[op].op == op,
			   IVM_ERROR_MSG_BAD_OP_TABLE);
#endif
	
	return ivm_global_op_table[op].proc;
}