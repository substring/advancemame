/*
 * This file is part of the Advance project.
 *
 * Copyright (C) 1999-2002 Andrea Mazzoleni
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details. 
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */

#include "os.h"
#include "conf.h"
#include "inputall.h"
#include "target.h"

#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <string.h>
#include <ctype.h>

static int done;

void sigint(int signum) {
	done = 1;
}

void run(void) {
	printf("Press ESC or Break to exit\n\r");

	signal(SIGINT,sigint);

	while (!done) {

		if (inputb_hit()) {
			unsigned k = inputb_get();

			if (k > 32 && k < 256)
				printf("- %d (%c)\n\r",k,(char)k);
			else
				printf("- %d ()\n\r",k);

			if (k == 27)
				done = 1;
		}

		os_poll();
		target_idle();
	}
}

static void error_callback(void* context, enum conf_callback_error error, const char* file, const char* tag, const char* valid, const char* desc, ...) {
	va_list arg;
	va_start(arg, desc);
	vfprintf(stderr, desc, arg);
	fprintf(stderr, "\n");
	if (valid)
		fprintf(stderr, "%s\n", valid);
	va_end(arg);
}

void os_signal(int signum) {
	os_default_signal(signum);
}

int os_main(int argc, char* argv[]) {
	adv_conf* context;
        const char* section_map[1];

	context = conf_init();

	if (os_init(context) != 0)
		goto err_conf;

	inputb_reg(context, 1);
	inputb_reg_driver_all(context);

	if (conf_input_args_load(context, 0, "", &argc, argv, error_callback, 0) != 0)
		goto err_os;

	if (argc > 1) {
		fprintf(stderr,"Unknown argument '%s'\n",argv[1]);
		goto err_os;
	}

	section_map[0] = "";
	conf_section_set(context, section_map, 1);

	if (inputb_load(context) != 0)
		goto err_os;

	if (os_inner_init("AdvanceINPUT") != 0)
		goto err_os;

	if (inputb_init() != 0)
		goto err_os_inner;

	run();

	inputb_done();
	os_inner_done();
	os_done();
	conf_done(context);

	return EXIT_SUCCESS;

err_os_inner:
	os_inner_done();
err_os:
	os_done();
err_conf:
	conf_done(context);
	return EXIT_FAILURE;

}

