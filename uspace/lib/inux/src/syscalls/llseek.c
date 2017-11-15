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

#include "syscalls.h"
#include "consts.h"
#include "files.h"
#include <stdlib.h>

#ifdef LIBINUX_SYSCALL_DEFINED__llseek

typedef sysarg_t (*seek_op_t)(openedfile_t *, off64_t, int);

static sysarg_t seek_file(openedfile_t *, off64_t, int);
static sysarg_t seek_na(openedfile_t *, off64_t, int);

static seek_op_t seek_ops[FILE_TYPE_LAST] = {
	[ FILE_TYPE_STDIN ] = seek_na,
	[ FILE_TYPE_STDOUTS ] = seek_na,
	[ FILE_TYPE_REGULAR_FILE ] = seek_file
};

sysarg_t seek_file(openedfile_t *file, off64_t offset, int origin)
{
	if (origin == LINUX_SEEK_SET) {
		file->vfs.position = offset;
	} else if (origin == LINUX_SEEK_CUR) {
		if (offset < 0) {
			if ((aoff64_t)(-offset) > file->vfs.position) {
				return -LINUX_EINVAL;
			}
		}
		file->vfs.position += offset;
	} else {
		assert(origin == LINUX_SEEK_END);
		// TODO
		return -LINUX_ENOSYS;
	}

	return file->vfs.position;
}

sysarg_t seek_na(openedfile_t *file, off64_t offset, int origin)
{
	return -LINUX_EBADF;
}


DEFINE_LINUX_SYSCALL5(_llseek, int, fd, unsigned long, high, unsigned long, low, void *, ignored, unsigned int, origin)
{
	if (!((origin == LINUX_SEEK_SET) || (origin == LINUX_SEEK_CUR) || (origin == LINUX_SEEK_END))) {
		return -LINUX_EINVAL;
	}

	openedfile_t *file = openedfile_get_by_fd(fd);
	if (file == NULL) {
		return -LINUX_EBADF;
	}

	seek_op_t op = seek_ops[file->type];
	return op(file, ((off64_t) high << 32) | low, origin);
}

#endif

/** @}
 */
