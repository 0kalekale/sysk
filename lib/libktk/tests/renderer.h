#ifndef RENDERER_H
#define RENDERER_H

#include <ktk.h>

void r_init(void);
void r_draw_rect(ktk_Rect rect, ktk_Color color);
void r_draw_text(const char *text, ktk_Vec2 pos, ktk_Color color);
void r_draw_icon(int id, ktk_Rect rect, ktk_Color color);
 int r_get_text_width(const char *text, int len);
 int r_get_text_height(void);
void r_set_clip_rect(ktk_Rect rect);
void r_clear(ktk_Color color);
void r_present(void);

#endif
