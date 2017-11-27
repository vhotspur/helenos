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
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <task.h>
#include <io/log.h>
#include <io/logctl.h>
#include <errno.h>
#include <str_error.h>

#define MAXIMUM_EXPECTED_ARGUMENT_COUNT 64

extern void __helenos_libc___pre_main(void *, int *, char ***);
extern void __helenos_libc____DEBUG(const char *);

static char *argv_with_envp[MAXIMUM_EXPECTED_ARGUMENT_COUNT];


void libinux_pre_main(void *pcb_ptr, int *argc_out, char ***argv_out)
{
	int argc_all;
	char **argv_all;
	__helenos_libc___pre_main(pcb_ptr, &argc_all, &argv_all);

	char **argv = NULL;
	if (argc_all + 4 > MAXIMUM_EXPECTED_ARGUMENT_COUNT) {
		argv = calloc(sizeof(char **), argc_all + 4);
	} else {
		argv = argv_with_envp;
	}

	logger_init();

	log_level_t default_level = LVL_WARN;

	int argc = 0;
	for (int i = 0; i < argc_all; i++) {
		if (str_test_prefix(argv_all[i], "libinux=")) {
			log_level_t level;
			int rc = log_level_from_str(argv_all[i] + 8, &level);
			if (rc != EOK) {
				logger(LVL_WARN, "Ignoring unknown logging level `%s'.", argv_all[i]);
				continue;
			}
			default_level = level;
			continue;
		}
		if (str_test_prefix(argv_all[i], "libinux_st=")) {
			int value = (int) strtol(argv_all[i] + 11, NULL, 10);
			logger_shall_print_stacktrace = !!value;
		}


		argv[argc] = argv_all[i];
		argc++;
	}

	logger_set_level(default_level);

	logger(LVL_NOTE, "This is libinux on HelenOS!");

	*argc_out = argc;
	*argv_out = argv;
}


/** @}
 */
