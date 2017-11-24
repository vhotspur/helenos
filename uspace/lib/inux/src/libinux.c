/*
 * Copyright (c) 2017 Vojtech Horky
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * - Redistributions of source code must retain the above copyright
 *   notice, this list of conditions and the following disclaimer.
 * - Redistributions in binary form must reproduce the above copyright
 *   notice, this list of conditions and the following disclaimer in the
 *   documentation and/or other materials provided with the distribution.
 * - The name of the author may not be used to endorse or promote products
 *   derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
 * OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
 * IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
 * NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
 * THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

/** @addtogroup libinux
 * @{
 */
#include "libinux.h"
#include "consts.h"
#include "syscalls.h"
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <io/log.h>
#include <stacktrace.h>

#define SYSCALL_LAST ((sysarg_t) -1)
//#define LOG_ALL_SYSCALLS

typedef struct {
	sysarg_t id;
	int size;
	const char *name;
	union {
		sysarg_t (*syscall0)(void);
		sysarg_t (*syscall1)(sysarg_t);
		sysarg_t (*syscall2)(sysarg_t, sysarg_t);
		sysarg_t (*syscall3)(sysarg_t, sysarg_t, sysarg_t);
		sysarg_t (*syscall4)(sysarg_t, sysarg_t, sysarg_t, sysarg_t);
		sysarg_t (*syscall5)(sysarg_t, sysarg_t, sysarg_t, sysarg_t, sysarg_t);
		sysarg_t (*syscall6)(sysarg_t, sysarg_t, sysarg_t, sysarg_t, sysarg_t, sysarg_t);
	};
} syscall_handler_t;

#undef LIBINUX_SYSCALL
#define LIBINUX_SYSCALL(syscall_name, syscall_id, syscall_arg_count) \
	extern DECLARE_LINUX_SYSCALL##syscall_arg_count(syscall_name);
#include "syscall_def.h"

static syscall_handler_t handlers[] = {
#undef LIBINUX_SYSCALL
#define LIBINUX_SYSCALL(syscall_name, syscall_id, syscall_arg_count) \
	{ \
		.id = syscall_id, \
		.size = syscall_arg_count, \
		.name = #syscall_name, \
		.syscall##syscall_arg_count = linux_syscall_wrapper_##syscall_name \
	},

#include "syscall_def.h"

	{ .id = SYSCALL_LAST }
};


static sysarg_t handle_unknown_syscall(sysarg_t id, int size,
	    sysarg_t *args)
{
	logger(LVL_ERROR, "unknown syscall %lld (with %d args)",
	    (long long) id, size);
	for (int i = 0; i < size; i++) {
		logger(LVL_DEBUG, "libinux:   arg #%d: 0x%llx", i + 1, (long long) args[i]);
	}
	
	return -LINUX_ENOSYS;
}

static sysarg_t handle_bad_invocation(sysarg_t id, int expected_size,
	    int actual_size, const char *name)
{
	return -LINUX_EINVAL;
}

sysarg_t libinux_syscall_handler(sysarg_t id, int size,
	    sysarg_t a1, sysarg_t a2, sysarg_t a3,
	    sysarg_t a4, sysarg_t a5, sysarg_t a6)
{
#ifdef LOG_ALL_SYSCALLS
	printf("libinux_syscall_handler(syscall=%lld, args=%d, "
	    "a1=0x%llx, a2=0x%llx, a3=0x%llx, "
	    "a4=0x%llx, a5=0x%llx, a6=0x%llx)\n",
	    (long long) id, size,
		(long long) a1, (long long) a2, (long long) a3,
		(long long) a4, (long long) a5, (long long) a6);
	stacktrace_print();
	fflush(stdout);
#endif


	syscall_handler_t *handler = NULL;
	for (syscall_handler_t *it = handlers; it->id != SYSCALL_LAST; it++) {
		if (it->id == id) {
			handler = it;
			break;
		}
	}
	if (handler == NULL) {
		sysarg_t args[] = { a1, a2, a3, a4, a5, a6 };
		return handle_unknown_syscall(id, size, args);
	}

	/*
	 * Receiving less arguments is certainly a problem. Getting more is
	 * actually okay if the expansion of an original macro appended zeros.
	 */
	if (size < handler->size) {
		return handle_bad_invocation(id, handler->size, size, handler->name);
	}

	sysarg_t res;

	switch (handler->size) {
	case 0:
		res = handler->syscall0();
		break;
	case 1:
		res = handler->syscall1(a1);
		break;
	case 2:
		res = handler->syscall2(a1, a2);
		break;
	case 3:
		res = handler->syscall3(a1, a2, a3);
		break;
	case 4:
		res = handler->syscall4(a1, a2, a3, a4);
		break;
	case 5:
		res = handler->syscall5(a1, a2, a3, a4, a5);
		break;
	case 6:
		res = handler->syscall6(a1, a2, a3, a4, a5, a6);
		break;
	default:
		assert(0 && "internal error - unexpected number of syscall parameters");
		res = -LINUX_ENOSYS;
	}

	logger(LVL_NOTE, "SYSCALL(syscall=%lld [%s]) = %llx",
	    (long long) id, handler->name, (long long) res);

	return res;
}


/** @}
 */
