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

#include "update.h"
#include "video.h"

#include <assert.h>

/**
 * Position for drawing.
 */
static unsigned update_offset;

/**
 * Page in drawing.
 */
static unsigned update_page;

/**
 * It's draw possible.
 */
static adv_bool is_update_draw_allowed;

/**
 * Pages used.
 */
static unsigned update_page_max;

/**
 * Inizialize the update system.
 * This function must be called after the mode was set.
 * \param max_buffer Max number of buffer to use.
 *   - 1 A single buffer is used.
 *   - 2 A double buffer is used if the current video mode supports the asyncronous page set.
 *   - 3 A triple buffer is used if the current video mode supports the syncronous page set.
 */
void update_init(unsigned max_buffer) {
	unsigned pages = (video_virtual_y() * video_bytes_per_scanline()) / video_bytes_per_page();
	if (pages > max_buffer)
		pages = max_buffer;

	update_page = 0;

	if (pages >= 3 && (video_flags() & MODE_FLAGS_SCROLL_SYNC)) {
		update_page_max = 3;
	} else if (pages >= 2 && (video_flags() & MODE_FLAGS_SCROLL_ASYNC)) {
		update_page_max = 2;
	} else {
		update_page_max = 1;
	}

	is_update_draw_allowed = 0;
}

/**
 * Deinizialize the update system.
 */
void update_done(void) {
	assert( is_update_draw_allowed == 0 );

	/* display the first page */
	if (update_page_max!=1) {
		video_display_set_async(0,0);
	}
}

/**
 * Get the number of pages.
 */
unsigned update_page_max_get(void) {
	return update_page_max;
}

/**
 * Get the horizontal position of the page in drawing.
 */
unsigned update_x_get(void) {
	unsigned x;
	assert( is_update_draw_allowed );
	x = (update_offset % video_bytes_per_scanline()) / video_bytes_per_pixel();
	if (video_is_unchained())
		x *= 4;
	return x;
}

/**
 * Get the vertical position of the page in drawing.
 */
unsigned update_y_get(void) {
	assert( is_update_draw_allowed );
	return update_offset / video_bytes_per_scanline();
}

/**
 * Get the number of the page in drawing.
 */
unsigned update_page_get(void) {
	assert( is_update_draw_allowed );
	return update_page;
}

/**
 * Start drawing to page.
 * After this call you can call update_x_get() and update_y_get().
 */
void update_start(void) {
	assert( is_update_draw_allowed == 0 );
	is_update_draw_allowed = 1;
	/* compute coordinate for drawing */
	if (update_page_max!=1) {
		update_offset = update_page * video_bytes_per_page();
	} else {
		update_offset = 0;
	}

	video_write_lock();
}

/**
 * End drawing a page.
 * \param x,y,size_x,size_y Updated range of the screeb. These coordinates are absolute in the screen, not relative at the page.
 * \param wait_retrace If a wait is required.
 */
void update_stop(unsigned x, unsigned y, unsigned size_x, unsigned size_y, adv_bool wait_retrace) {
	assert( is_update_draw_allowed );
	is_update_draw_allowed = 0;

	video_write_unlock(x, y, size_x, size_y);

	if (update_page_max!=1) {
		unsigned offset;
		offset = update_page * video_bytes_per_page();
		video_display_set_async(offset, wait_retrace);

		/* next page to draw */
		++update_page;
		if (update_page == update_page_max)
			update_page = 0;
	} else {
		if (wait_retrace)
			video_wait_vsync();
	}
}
