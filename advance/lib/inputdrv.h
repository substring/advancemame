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
 *
 * In addition, as a special exception, Andrea Mazzoleni
 * gives permission to link the code of this program with
 * the MAME library (or with modified versions of MAME that use the
 * same license as MAME), and distribute linked combinations including
 * the two.  You must obey the GNU General Public License in all
 * respects for all of the code used other than MAME.  If you modify
 * this file, you may extend this exception to your version of the
 * file, but you are not obligated to do so.  If you do not wish to
 * do so, delete this exception statement from your version.
 */

/** \file
 * Input drivers.
 */

/** \addtogroup Input */
/*@{*/

#ifndef __INPUTDRV_H
#define __INPUTDRV_H

#include "device.h"
#include "conf.h"

#include <assert.h>

#ifdef __cplusplus
extern "C" {
#endif

/***************************************************************************/
/* Codes */

#define INPUTB_TAB 9
#define INPUTB_ENTER 13
#define INPUTB_ESC 27
#define INPUTB_SPACE 32
#define INPUTB_UP 256
#define INPUTB_DOWN 257
#define INPUTB_LEFT 258
#define INPUTB_RIGHT 259
#define INPUTB_HOME 264
#define INPUTB_END 265
#define INPUTB_PGUP 266
#define INPUTB_PGDN 267
#define INPUTB_F1 268
#define INPUTB_F2 269
#define INPUTB_F3 270
#define INPUTB_F4 271
#define INPUTB_F5 272
#define INPUTB_F6 273
#define INPUTB_F7 274
#define INPUTB_F8 275
#define INPUTB_F9 276
#define INPUTB_F10 277
#define INPUTB_DEL 278
#define INPUTB_INS 279
#define INPUTB_BACKSPACE 280
#define INPUTB_MAX 512

/***************************************************************************/
/* Driver */

#define INPUT_DRIVER_FLAGS_USER_BIT0 0x10000
#define INPUT_DRIVER_FLAGS_USER_MASK 0xFFFF0000

/**
 * Input adv_driver.
 * This struct abstract all the adv_driver funtionalities.
 */
typedef struct inputb_driver_struct {
	const char* name; /**< Name of the adv_driver */
	const adv_device* device_map; /**< List of supported devices */

	/** Load the configuration options. Call before init() */
	adv_error (*load)(adv_conf* context);

	/** Register the load options. Call before load(). */
	void (*reg)(adv_conf* context);

	adv_error (*init)(int device_id); /**< Initialize the adv_driver */
	void (*done)(void); /**< Deinitialize the adv_driver */

	unsigned (*flags)(void); /**< Get the capabilities of the adv_driver */

	adv_bool (*hit)(void);
	unsigned (*get)(void);
} inputb_driver;

#define INPUT_DRIVER_MAX 8

struct inputb_state_struct {
	adv_bool is_initialized_flag;
	adv_bool is_active_flag;
	unsigned driver_mac;
	inputb_driver* driver_map[INPUT_DRIVER_MAX];
	inputb_driver* driver_current;
	char name[DEVICE_NAME_MAX];
};

extern struct inputb_state_struct inputb_state;

void inputb_reg(adv_conf* config_context, adv_bool auto_detect);
void inputb_reg_driver(adv_conf* config_context, inputb_driver* adv_driver);
adv_error inputb_load(adv_conf* config_context);
adv_error inputb_init(void);
void inputb_done(void);
void inputb_abort(void);

static inline adv_bool inputb_hit(void) {
	assert( inputb_state.is_active_flag );

	return inputb_state.driver_current->hit();
}

static inline unsigned inputb_get(void) {
	assert( inputb_state.is_active_flag );

	return inputb_state.driver_current->get();
}

/**
 * Get the adv_driver/adv_device name.
 * \return Pointer at a static buffer.
 */
static inline const char* inputb_name(void) {
	return inputb_state.name;
}

#ifdef __cplusplus
}
#endif

#endif

/*@}*/
