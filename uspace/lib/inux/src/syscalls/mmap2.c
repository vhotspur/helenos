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

#include <stdint.h>
#include <stdio.h>
#include <as.h>
#include <errno.h>
#include <offset.h>

#include "libinux.h"
#include "syscalls.h"
#include "consts.h"


DEFINE_LINUX_SYSCALL6(mmap2, void *, addr, size_t, len,
		int, prot, int, flags,
		int, fd, off64_t, off_pg) // TODO: is off64_t correct here?
{
	logger(LVL_DEBUG, "mmap2(addr=%p, len=%zu, prot=0x%x, flags=0x%x, fd=%d, off_pg=%lld)",
	    addr, len, prot, flags, fd, (long long) off_pg);
	
	if ((flags & LINUX_MAP_ANONYMOUS) != LINUX_MAP_ANONYMOUS) {
		return -LINUX_ENOSYS;
	}

	if ((flags & LINUX_MAP_PRIVATE) != LINUX_MAP_PRIVATE) {
		return -LINUX_ENOSYS;
	}

	if ((flags & LINUX_MAP_FIXED) == LINUX_MAP_FIXED) {
		return -LINUX_ENOMEM;
	}

	if (len == 0) {
		return -LINUX_EINVAL;
	}

	uint8_t *result = as_area_create(AS_AREA_ANY, len, AS_AREA_READ | AS_AREA_WRITE | AS_AREA_CACHEABLE, 0);
	logger(LVL_DEBUG2, "  mmap2::as_area_create() = %p", result);
	if (result == AS_MAP_FAILED) {
		return -LINUX_ENOMEM;
	}

	return (sysarg_t) result;
}

/** @}
 */
