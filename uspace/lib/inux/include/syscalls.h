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

#ifndef _LIBINUX_SYSCALLS_H
#define _LIBINUX_SYSCALLS_H

#include <types/common.h>

#define LIBINUX_SYSCALL(name, id, paramcount) /* empty */
#include "syscall_def.h"

#define DECLARE_LINUX_SYSCALL0(name) \
	sysarg_t linux_syscall_wrapper_##name(void)
#define DECLARE_LINUX_SYSCALL1(name) \
	sysarg_t linux_syscall_wrapper_##name(sysarg_t)
#define DECLARE_LINUX_SYSCALL2(name) \
	sysarg_t linux_syscall_wrapper_##name(sysarg_t, sysarg_t)
#define DECLARE_LINUX_SYSCALL3(name) \
	sysarg_t linux_syscall_wrapper_##name(sysarg_t, sysarg_t, sysarg_t)
#define DECLARE_LINUX_SYSCALL4(name) \
	sysarg_t linux_syscall_wrapper_##name(sysarg_t, sysarg_t, sysarg_t, sysarg_t)
#define DECLARE_LINUX_SYSCALL5(name) \
	sysarg_t linux_syscall_wrapper_##name(sysarg_t, sysarg_t, sysarg_t, sysarg_t, sysarg_t)
#define DECLARE_LINUX_SYSCALL6(name) \
	sysarg_t linux_syscall_wrapper_##name(sysarg_t, sysarg_t, sysarg_t, sysarg_t, sysarg_t, sysarg_t)



#define DEFINE_LINUX_SYSCALL0(name) \
	DECLARE_LINUX_SYSCALL0(name); \
	static sysarg_t linux_syscall_##name(void); \
	sysarg_t linux_syscall_wrapper_##name(void) { \
		return linux_syscall_##name(); \
	} \
	\
	sysarg_t linux_syscall_##name()

#define DEFINE_LINUX_SYSCALL1(name, p1t, p1n) \
	DECLARE_LINUX_SYSCALL1(name); \
	static sysarg_t linux_syscall_##name(p1t); \
	sysarg_t linux_syscall_wrapper_##name(sysarg_t a1) { \
		return linux_syscall_##name((p1t) a1); \
	} \
	\
	sysarg_t linux_syscall_##name(p1t p1n)

#define DEFINE_LINUX_SYSCALL2(name, p1t, p1n, p2t, p2n) \
	DECLARE_LINUX_SYSCALL2(name); \
	static sysarg_t linux_syscall_##name(p1t, p2t); \
	sysarg_t linux_syscall_wrapper_##name(sysarg_t a1, sysarg_t a2) { \
		return linux_syscall_##name((p1t) a1, (p2t) a2); \
	} \
	\
	sysarg_t linux_syscall_##name(p1t p1n, p2t p2n)

#define DEFINE_LINUX_SYSCALL3(name, p1t, p1n, p2t, p2n, p3t, p3n) \
	DECLARE_LINUX_SYSCALL3(name); \
	static sysarg_t linux_syscall_##name(p1t, p2t, p3t); \
	sysarg_t linux_syscall_wrapper_##name(sysarg_t a1, sysarg_t a2, sysarg_t a3) { \
		return linux_syscall_##name((p1t) a1, (p2t) a2, (p3t) a3); \
	} \
	\
	sysarg_t linux_syscall_##name(p1t p1n, p2t p2n, p3t p3n)

#define DEFINE_LINUX_SYSCALL4(name, p1t, p1n, p2t, p2n, p3t, p3n, p4t, p4n) \
	DECLARE_LINUX_SYSCALL4(name); \
	static sysarg_t linux_syscall_##name(p1t, p2t, p3t, p4t); \
	sysarg_t linux_syscall_wrapper_##name(sysarg_t a1, sysarg_t a2, sysarg_t a3, sysarg_t a4) { \
		return linux_syscall_##name((p1t) a1, (p2t) a2, (p3t) a3, (p4t) a4); \
	} \
	\
	sysarg_t linux_syscall_##name(p1t p1n, p2t p2n, p3t p3n, p4t p4n)

#define DEFINE_LINUX_SYSCALL5(name, p1t, p1n, p2t, p2n, p3t, p3n, p4t, p4n, p5t, p5n) \
	DECLARE_LINUX_SYSCALL5(name); \
	static sysarg_t linux_syscall_##name(p1t, p2t, p3t, p4t, p5t); \
	sysarg_t linux_syscall_wrapper_##name(sysarg_t a1, sysarg_t a2, sysarg_t a3, sysarg_t a4, sysarg_t a5) { \
		return linux_syscall_##name((p1t) a1, (p2t) a2, (p3t) a3, (p4t) a4, (p5t) a5); \
	} \
	\
	sysarg_t linux_syscall_##name(p1t p1n, p2t p2n, p3t p3n, p4t p4n, p5t p5n)

#define DEFINE_LINUX_SYSCALL6(name, p1t, p1n, p2t, p2n, p3t, p3n, p4t, p4n, p5t, p5n, p6t, p6n) \
	DECLARE_LINUX_SYSCALL6(name); \
	static sysarg_t linux_syscall_##name(p1t, p2t, p3t, p4t, p5t, p6t); \
	sysarg_t linux_syscall_wrapper_##name(sysarg_t a1, sysarg_t a2, sysarg_t a3, sysarg_t a4, sysarg_t a5, sysarg_t a6) { \
		return linux_syscall_##name((p1t) a1, (p2t) a2, (p3t) a3, (p4t) a4, (p5t) a5, (p6t) a6); \
	} \
	\
	sysarg_t linux_syscall_##name(p1t p1n, p2t p2n, p3t p3n, p4t p4n, p5t p5n, p6t p6n)

#endif

/** @}
 */
