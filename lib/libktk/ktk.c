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
#include <raylib.h>

void ktk_init(int x, int y, char *window_name) {
  InitWindow(x, y, window_name);
} 

void ktk_sample_event_loop() {
  while (!WindowShouldClose()) {
    BeginDrawing();
    ClearBackground(RAYWHITE);
    DrawText("ktk_sample", 20, 200, 20, LIGHTGRAY);
    EndDrawing();
  }
}
 
void ktk_close() {
  CloseWindow();
}
