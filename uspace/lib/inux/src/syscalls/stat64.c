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

#include <stddef.h>
#include <stdio.h>
#include <ipc/vfs.h>
#include <vfs/vfs.h>
#include <errno.h>
#include <stacktrace.h>

#include "libinux.h"
#include "consts.h"
#include "syscalls.h"
#include "files.h"

typedef uint64_t dev_t;
typedef uint64_t ino_t;
typedef unsigned int mode_t;
typedef unsigned int nlink_t;
typedef unsigned int uid_t;
typedef unsigned int gid_t;
typedef int64_t off_t;
typedef long blksize_t;
typedef int64_t blkcnt_t;
typedef long time_t;

struct linux_timespec {
	time_t tv_sec;
	long tv_nsec;
};

struct linux_stat {
	dev_t     st_dev;         /* ID of device containing file */
	ino_t     st_ino;         /* Inode number */
	mode_t    st_mode;        /* File type and mode */
	nlink_t   st_nlink;       /* Number of hard links */
	uid_t     st_uid;         /* User ID of owner */
	gid_t     st_gid;         /* Group ID of owner */
	dev_t     st_rdev;        /* Device ID (if special file) */
	off_t     st_size;        /* Total size, in bytes */
	blksize_t st_blksize;     /* Block size for filesystem I/O */
	blkcnt_t  st_blocks;      /* Number of 512B blocks allocated */
	struct linux_timespec st_atim;  /* Time of last access */
	struct linux_timespec st_mtim;  /* Time of last modification */
	struct linux_timespec st_ctim;  /* Time of last status change */
};

DEFINE_LINUX_SYSCALL2(stat64, const char *, filename, struct linux_stat *, res)
{
	logger(LVL_DEBUG, "stat64(\"%s\", %p)", filename, res);
	
	int handle = vfs_lookup(filename, WALK_REGULAR);
	if (handle < 0) {
		return -LINUX_ENOENT;
	}

	struct stat st;
	int rc = vfs_stat(handle, &st);
	if (rc != EOK) {
		// FIXME: convert error code properly
		return -LINUX_ENOENT;
	}

	res->st_dev = 0;
	res->st_ino = 0;
	res->st_mode = 0777;
	res->st_nlink = st.is_directory ? 2 : 1;
	res->st_uid = 0;
	res->st_gid = 0;
	res->st_rdev = 0;
	res->st_size = (off_t) st.size;
	res->st_blksize = 512;
	res->st_blocks = 0;
	res->st_atim.tv_sec = 0;
	res->st_atim.tv_nsec = 0;
	res->st_mtim.tv_sec = 0;
	res->st_mtim.tv_nsec = 0;
	res->st_ctim.tv_sec = 0;
	res->st_ctim.tv_nsec = 0;
	
	vfs_put(handle);

	return 0;
}

/** @}
 */
