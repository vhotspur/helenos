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


/** Buffer size for HelenOS logger name.
 * Contains 'libinux' plus task id.
 */
#define LOGGER_NAME_SIZE 30

static char logger_name[LOGGER_NAME_SIZE + 1];
static log_level_t logger_level = LVL_NOTE;

void logger_init()
{
	snprintf(logger_name, LOGGER_NAME_SIZE, "inux-T%lld", (long long) task_get_id());
	log_init(logger_name);
}

void logger_set_level(log_level_t level)
{
	// For debugging purposes, log everything to file
	int rc = logctl_set_log_level(logger_name, LVL_DEBUG2);
	if (rc != EOK) {
		logger(LVL_WARN, "Failed to change default logging level: %s.", str_error(rc));
		return;
	}

	logger_level = level;
}

void logger(log_level_t level, const char *fmt, ...)
{
	va_list args;

	va_start(args, fmt);
	log_msgv(LOG_DEFAULT, level, fmt, args);
	va_end(args);

	if (level <= logger_level) {
		va_start(args, fmt);

		char *fmt_prefixed;
		int rc = asprintf(&fmt_prefixed, "[libinux %s]: %s\n", log_level_str(level), fmt);
		if (rc > 0) {
			vfprintf(stderr, fmt_prefixed, args);
			free(fmt_prefixed);
		} else {
			vfprintf(stderr, fmt, args);
		}

		fflush(stderr);

		va_end(args);
	}
}

/** @}
 */
