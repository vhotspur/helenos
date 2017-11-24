#
# Copyright (c) 2017 Vojtech Horky
# All rights reserved.
#
# Redistribution and use in source and binary forms, with or without
# modification, are permitted provided that the following conditions
# are met:
#
# - Redistributions of source code must retain the above copyright
#   notice, this list of conditions and the following disclaimer.
# - Redistributions in binary form must reproduce the above copyright
#   notice, this list of conditions and the following disclaimer in the
#   documentation and/or other materials provided with the distribution.
# - The name of the author may not be used to endorse or promote products
#   derived from this software without specific prior written permission.
#
# THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR
# IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
# OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
# IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT,
# INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
# NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
# DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
# THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
# (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
# THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
#

USPACE_PREFIX = ../..
LIBRARY = libinux
EXTRA_CFLAGS += \
	-Iinclude

SYSCALLS = \
	brk \
	close \
    exit \
    exit_group \
    fcntl \
    ioctl \
    llseek \
    madvise \
    mmap2 \
    open \
    prlimit \
    read \
    readv \
    rt_sigprocmask \
    rt_sigaction \
    times \
    write \
    writev

SOURCES = \
	src/files.c \
	src/libinux.c \
	src/logger.c \
	src/main.c \
	$(addprefix src/syscalls/, $(SYSCALLS))

LIBC_FILE = $(LIBC_PREFIX)/libc.a
LIBC_FIXED = libc4inux.a
LIBC_SYMBOLS = libc-symbols.list
LIBC_RENAME_SYMBOLS = libc-rename-symbols.list
LIBINUX_SYMBOLS = libinux-symbols.list
LIBINUX_RENAME_SYMBOLS = libinux-rename-symbols.list

LIBINUX_RENAMED = libinux-renamed.a
LIBINUX_STANDALONE = libinux-helenos.a
EXTRA_OUTPUT = $(LIBINUX_STANDALONE) $(LIBINUX_RENAMED) $(LIBC_FIXED)
EXTRA_CLEAN = $(LIBC_RENAME_SYMBOLS) $(LIBC_SYMBOLS) $(LIBINUX_RENAME_SYMBOLS) $(LIBINUX_SYMBOLS) include/syscall_def.h include/consts.h
PRE_DEPEND = include/syscall_def.h include/consts.h

include $(USPACE_PREFIX)/Makefile.common

include/syscall_def.h: gensyscalls.py syscalls.yml
	./gensyscalls.py $(UARCH) >$@

include/consts.h: genconsts.py consts.yml
	./genconsts.py $(UARCH) >$@

$(LIBC_FIXED): $(LIBC_FILE) $(LIBC_RENAME_SYMBOLS)
	$(OBJCOPY) --redefine-syms=$(LIBC_RENAME_SYMBOLS) $(LIBC_FILE) $@

$(LIBINUX_RENAMED): $(LIBRARY).a $(LIBC_RENAME_SYMBOLS) $(LIBINUX_RENAME_SYMBOLS)
	$(OBJCOPY) --redefine-syms=$(LIBC_RENAME_SYMBOLS) $(LIBRARY).a $@
	$(OBJCOPY) --redefine-syms=$(LIBC_RENAME_SYMBOLS) --redefine-syms=$(LIBINUX_RENAME_SYMBOLS) $(LIBRARY).a $@

$(LIBINUX_STANDALONE): $(LIBINUX_RENAMED) $(LIBC_FIXED)
	echo -e 'create $@\naddlib $(LIBINUX_RENAMED)\naddlib $(LIBC_FIXED)\nsave\nend' | $(AR) -M

$(LIBC_SYMBOLS): $(LIBC_FILE)
	$(NM) --extern-only --defined-only $(LIBC_FILE) | sed -n 's:[0-9a-fA-F]\{8,16\} . \(.*\):\1:p' | sort | uniq >$@

$(LIBINUX_SYMBOLS): $(LIBRARY).a
	$(NM) --extern-only --defined-only $(LIBRARY).a | sed -n 's:[0-9a-fA-F]\{8,16\} . \(.*\):\1:p' | sort | uniq | grep -v -e libinux_syscall_handler -e libinux_pre_main >$@

$(LIBC_RENAME_SYMBOLS): $(LIBC_SYMBOLS)
	grep -v '^__aeabi_[iul]' <$(LIBC_SYMBOLS) | sed 's/.*/& __helenos_libc_&/' >$@

$(LIBINUX_RENAME_SYMBOLS): $(LIBINUX_SYMBOLS)
	sed 's/.*/& __helenos_libinux_&/' <$(LIBINUX_SYMBOLS) >$@

