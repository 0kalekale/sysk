/*
    ktk: A simple X11 user interface toolkit
    Copyright (C) 2022 0kalekale

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU Affero General Public License as
    published by the Free Software Foundation, either version 3 of the
    License, or (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Affero General Public License for more details.

    You should have received a copy of the GNU Affero General Public License
    along with this program.  If not, see <https://www.gnu.org/licenses/>.
*/
#include <xcb/xcb.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <string.h>
#include <unistd.h>

xcb_connection_t *c;
xcb_screen_t *s;
xcb_window_t w;
xcb_gcontext_t g;
xcb_generic_event_t *e;

void ktk_init()  {
	c = xcb_connect(NULL, NULL);
	if (xcb_connection_has_error(c) > 0) {
			fprintf(stderr, "[ERROR] : could not open display");	
			exit(1);
		}

	s = xcb_setup_roots_iterator(xcb_get_setup(c)).data;
}

void ktk_create_window(int x, int y) {
	uint32_t mask = XCB_CW_BACK_PIXEL | XCB_CW_EVENT_MASK;
	
	uint32_t values[2];
	values[0] = s->white_pixel;
	values[1] = XCB_EVENT_MASK_EXPOSURE | XCB_EVENT_MASK_KEY_PRESS;
	
	w = xcb_generate_id(c);	
	xcb_create_window (c, XCB_COPY_FROM_PARENT, w, s->root, 10, 10, x, y, 1, XCB_WINDOW_CLASS_INPUT_OUTPUT, s->root_visual, mask, values);
	
	mask = XCB_GC_FOREGROUND | XCB_GC_GRAPHICS_EXPOSURES;
	values[0] = s->black_pixel;
	values[1] = 0;

	g = xcb_generate_id(c);
	xcb_create_gc(c, g, w, mask, values);
} 

void ktk_set_window_name(const char *window_name) {
	xcb_change_property(c, XCB_PROP_MODE_REPLACE, w, XCB_ATOM_WM_NAME, XCB_ATOM_STRING, 8, strlen(window_name), window_name);
	 xcb_flush(c);
}
 
void ktk_close() {
	xcb_disconnect(c);
}
