/*
** Copyright (c) 2020 rxi
**
** This library is free software; you can redistribute it and/or modify it
** under the terms of the MIT license. See `microui.c` for details.
*/

#ifndef MICROUI_H
#define MICROUI_H

#define KTK_VERSION "2.01"

#define KTK_COMMANDLIST_SIZE     (256 * 1024)
#define KTK_ROOTLIST_SIZE        32
#define KTK_CONTAINERSTACK_SIZE  32
#define KTK_CLIPSTACK_SIZE       32
#define KTK_IDSTACK_SIZE         32
#define KTK_LAYOUTSTACK_SIZE     16
#define KTK_CONTAINERPOOL_SIZE   48
#define KTK_TREENODEPOOL_SIZE    48
#define KTK_MAX_WIDTHS           16
#define KTK_REAL                 float
#define KTK_REAL_FMT             "%.3g"
#define KTK_SLIDER_FMT           "%.2f"
#define KTK_MAX_FMT              127

#define ktk_stack(T, n)          struct { int idx; T items[n]; }
#define ktk_min(a, b)            ((a) < (b) ? (a) : (b))
#define ktk_max(a, b)            ((a) > (b) ? (a) : (b))
#define ktk_clamp(x, a, b)       ktk_min(b, ktk_max(a, x))

enum {
  KTK_CLIP_PART = 1,
  KTK_CLIP_ALL
};

enum {
  KTK_COMMAND_JUMP = 1,
  KTK_COMMAND_CLIP,
  KTK_COMMAND_RECT,
  KTK_COMMAND_TEXT,
  KTK_COMMAND_ICON,
  KTK_COMMAND_MAX
};

enum {
  KTK_COLOR_TEXT,
  KTK_COLOR_BORDER,
  KTK_COLOR_WINDOWBG,
  KTK_COLOR_TITLEBG,
  KTK_COLOR_TITLETEXT,
  KTK_COLOR_PANELBG,
  KTK_COLOR_BUTTON,
  KTK_COLOR_BUTTONHOVER,
  KTK_COLOR_BUTTONFOCUS,
  KTK_COLOR_BASE,
  KTK_COLOR_BASEHOVER,
  KTK_COLOR_BASEFOCUS,
  KTK_COLOR_SCROLLBASE,
  KTK_COLOR_SCROLLTHUMB,
  KTK_COLOR_MAX
};

enum {
  KTK_ICON_CLOSE = 1,
  KTK_ICON_CHECK,
  KTK_ICON_COLLAPSED,
  KTK_ICON_EXPANDED,
  KTK_ICON_MAX
};

enum {
  KTK_RES_ACTIVE       = (1 << 0),
  KTK_RES_SUBMIT       = (1 << 1),
  KTK_RES_CHANGE       = (1 << 2)
};

enum {
  KTK_OPT_ALIGNCENTER  = (1 << 0),
  KTK_OPT_ALIGNRIGHT   = (1 << 1),
  KTK_OPT_NOINTERACT   = (1 << 2),
  KTK_OPT_NOFRAME      = (1 << 3),
  KTK_OPT_NORESIZE     = (1 << 4),
  KTK_OPT_NOSCROLL     = (1 << 5),
  KTK_OPT_NOCLOSE      = (1 << 6),
  KTK_OPT_NOTITLE      = (1 << 7),
  KTK_OPT_HOLDFOCUS    = (1 << 8),
  KTK_OPT_AUTOSIZE     = (1 << 9),
  KTK_OPT_POPUP        = (1 << 10),
  KTK_OPT_CLOSED       = (1 << 11),
  KTK_OPT_EXPANDED     = (1 << 12)
};

enum {
  KTK_MOUSE_LEFT       = (1 << 0),
  KTK_MOUSE_RIGHT      = (1 << 1),
  KTK_MOUSE_MIDDLE     = (1 << 2)
};

enum {
  KTK_KEY_SHIFT        = (1 << 0),
  KTK_KEY_CTRL         = (1 << 1),
  KTK_KEY_ALT          = (1 << 2),
  KTK_KEY_BACKSPACE    = (1 << 3),
  KTK_KEY_RETURN       = (1 << 4)
};


typedef struct ktk_Context ktk_Context;
typedef unsigned ktk_Id;
typedef KTK_REAL ktk_Real;
typedef void* ktk_Font;

typedef struct { int x, y; } ktk_Vec2;
typedef struct { int x, y, w, h; } ktk_Rect;
typedef struct { unsigned char r, g, b, a; } ktk_Color;
typedef struct { ktk_Id id; int last_update; } ktk_PoolItem;

typedef struct { int type, size; } ktk_BaseCommand;
typedef struct { ktk_BaseCommand base; void *dst; } ktk_JumpCommand;
typedef struct { ktk_BaseCommand base; ktk_Rect rect; } ktk_ClipCommand;
typedef struct { ktk_BaseCommand base; ktk_Rect rect; ktk_Color color; } ktk_RectCommand;
typedef struct { ktk_BaseCommand base; ktk_Font font; ktk_Vec2 pos; ktk_Color color; char str[1]; } ktk_TextCommand;
typedef struct { ktk_BaseCommand base; ktk_Rect rect; int id; ktk_Color color; } ktk_IconCommand;

typedef union {
  int type;
  ktk_BaseCommand base;
  ktk_JumpCommand jump;
  ktk_ClipCommand clip;
  ktk_RectCommand rect;
  ktk_TextCommand text;
  ktk_IconCommand icon;
} ktk_Command;

typedef struct {
  ktk_Rect body;
  ktk_Rect next;
  ktk_Vec2 position;
  ktk_Vec2 size;
  ktk_Vec2 max;
  int widths[KTK_MAX_WIDTHS];
  int items;
  int item_index;
  int next_row;
  int next_type;
  int indent;
} ktk_Layout;

typedef struct {
  ktk_Command *head, *tail;
  ktk_Rect rect;
  ktk_Rect body;
  ktk_Vec2 content_size;
  ktk_Vec2 scroll;
  int zindex;
  int open;
} ktk_Container;

typedef struct {
  ktk_Font font;
  ktk_Vec2 size;
  int padding;
  int spacing;
  int indent;
  int title_height;
  int scrollbar_size;
  int thumb_size;
  ktk_Color colors[KTK_COLOR_MAX];
} ktk_Style;

struct ktk_Context {
  /* callbacks */
  int (*text_width)(ktk_Font font, const char *str, int len);
  int (*text_height)(ktk_Font font);
  void (*draw_frame)(ktk_Context *ctx, ktk_Rect rect, int colorid);
  /* core state */
  ktk_Style _style;
  ktk_Style *style;
  ktk_Id hover;
  ktk_Id focus;
  ktk_Id last_id;
  ktk_Rect last_rect;
  int last_zindex;
  int updated_focus;
  int frame;
  ktk_Container *hover_root;
  ktk_Container *next_hover_root;
  ktk_Container *scroll_target;
  char number_edit_buf[KTK_MAX_FMT];
  ktk_Id number_edit;
  /* stacks */
  ktk_stack(char, KTK_COMMANDLIST_SIZE) command_list;
  ktk_stack(ktk_Container*, KTK_ROOTLIST_SIZE) root_list;
  ktk_stack(ktk_Container*, KTK_CONTAINERSTACK_SIZE) container_stack;
  ktk_stack(ktk_Rect, KTK_CLIPSTACK_SIZE) clip_stack;
  ktk_stack(ktk_Id, KTK_IDSTACK_SIZE) id_stack;
  ktk_stack(ktk_Layout, KTK_LAYOUTSTACK_SIZE) layout_stack;
  /* retained state pools */
  ktk_PoolItem container_pool[KTK_CONTAINERPOOL_SIZE];
  ktk_Container containers[KTK_CONTAINERPOOL_SIZE];
  ktk_PoolItem treenode_pool[KTK_TREENODEPOOL_SIZE];
  /* input state */
  ktk_Vec2 mouse_pos;
  ktk_Vec2 last_mouse_pos;
  ktk_Vec2 mouse_delta;
  ktk_Vec2 scroll_delta;
  int mouse_down;
  int mouse_pressed;
  int key_down;
  int key_pressed;
  char input_text[32];
};


ktk_Vec2 ktk_vec2(int x, int y);
ktk_Rect ktk_rect(int x, int y, int w, int h);
ktk_Color ktk_color(int r, int g, int b, int a);

void ktk_init(ktk_Context *ctx);
void ktk_begin(ktk_Context *ctx);
void ktk_end(ktk_Context *ctx);
void ktk_set_focus(ktk_Context *ctx, ktk_Id id);
ktk_Id ktk_get_id(ktk_Context *ctx, const void *data, int size);
void ktk_push_id(ktk_Context *ctx, const void *data, int size);
void ktk_pop_id(ktk_Context *ctx);
void ktk_push_clip_rect(ktk_Context *ctx, ktk_Rect rect);
void ktk_pop_clip_rect(ktk_Context *ctx);
ktk_Rect ktk_get_clip_rect(ktk_Context *ctx);
int ktk_check_clip(ktk_Context *ctx, ktk_Rect r);
ktk_Container* ktk_get_current_container(ktk_Context *ctx);
ktk_Container* ktk_get_container(ktk_Context *ctx, const char *name);
void ktk_bring_to_front(ktk_Context *ctx, ktk_Container *cnt);

int ktk_pool_init(ktk_Context *ctx, ktk_PoolItem *items, int len, ktk_Id id);
int ktk_pool_get(ktk_Context *ctx, ktk_PoolItem *items, int len, ktk_Id id);
void ktk_pool_update(ktk_Context *ctx, ktk_PoolItem *items, int idx);

void ktk_input_mousemove(ktk_Context *ctx, int x, int y);
void ktk_input_mousedown(ktk_Context *ctx, int x, int y, int btn);
void ktk_input_mouseup(ktk_Context *ctx, int x, int y, int btn);
void ktk_input_scroll(ktk_Context *ctx, int x, int y);
void ktk_input_keydown(ktk_Context *ctx, int key);
void ktk_input_keyup(ktk_Context *ctx, int key);
void ktk_input_text(ktk_Context *ctx, const char *text);

ktk_Command* ktk_push_command(ktk_Context *ctx, int type, int size);
int ktk_next_command(ktk_Context *ctx, ktk_Command **cmd);
void ktk_set_clip(ktk_Context *ctx, ktk_Rect rect);
void ktk_draw_rect(ktk_Context *ctx, ktk_Rect rect, ktk_Color color);
void ktk_draw_box(ktk_Context *ctx, ktk_Rect rect, ktk_Color color);
void ktk_draw_text(ktk_Context *ctx, ktk_Font font, const char *str, int len, ktk_Vec2 pos, ktk_Color color);
void ktk_draw_icon(ktk_Context *ctx, int id, ktk_Rect rect, ktk_Color color);

void ktk_layout_row(ktk_Context *ctx, int items, const int *widths, int height);
void ktk_layout_width(ktk_Context *ctx, int width);
void ktk_layout_height(ktk_Context *ctx, int height);
void ktk_layout_begin_column(ktk_Context *ctx);
void ktk_layout_end_column(ktk_Context *ctx);
void ktk_layout_set_next(ktk_Context *ctx, ktk_Rect r, int relative);
ktk_Rect ktk_layout_next(ktk_Context *ctx);

void ktk_draw_control_frame(ktk_Context *ctx, ktk_Id id, ktk_Rect rect, int colorid, int opt);
void ktk_draw_control_text(ktk_Context *ctx, const char *str, ktk_Rect rect, int colorid, int opt);
int ktk_mouse_over(ktk_Context *ctx, ktk_Rect rect);
void ktk_update_control(ktk_Context *ctx, ktk_Id id, ktk_Rect rect, int opt);

#define ktk_button(ctx, label)             ktk_button_ex(ctx, label, 0, KTK_OPT_ALIGNCENTER)
#define ktk_textbox(ctx, buf, bufsz)       ktk_textbox_ex(ctx, buf, bufsz, 0)
#define ktk_slider(ctx, value, lo, hi)     ktk_slider_ex(ctx, value, lo, hi, 0, KTK_SLIDER_FMT, KTK_OPT_ALIGNCENTER)
#define ktk_number(ctx, value, step)       ktk_number_ex(ctx, value, step, KTK_SLIDER_FMT, KTK_OPT_ALIGNCENTER)
#define ktk_header(ctx, label)             ktk_header_ex(ctx, label, 0)
#define ktk_begin_treenode(ctx, label)     ktk_begin_treenode_ex(ctx, label, 0)
#define ktk_begin_window(ctx, title, rect) ktk_begin_window_ex(ctx, title, rect, 0)
#define ktk_begin_panel(ctx, name)         ktk_begin_panel_ex(ctx, name, 0)

void ktk_text(ktk_Context *ctx, const char *text);
void ktk_label(ktk_Context *ctx, const char *text);
int ktk_button_ex(ktk_Context *ctx, const char *label, int icon, int opt);
int ktk_checkbox(ktk_Context *ctx, const char *label, int *state);
int ktk_textbox_raw(ktk_Context *ctx, char *buf, int bufsz, ktk_Id id, ktk_Rect r, int opt);
int ktk_textbox_ex(ktk_Context *ctx, char *buf, int bufsz, int opt);
int ktk_slider_ex(ktk_Context *ctx, ktk_Real *value, ktk_Real low, ktk_Real high, ktk_Real step, const char *fmt, int opt);
int ktk_number_ex(ktk_Context *ctx, ktk_Real *value, ktk_Real step, const char *fmt, int opt);
int ktk_header_ex(ktk_Context *ctx, const char *label, int opt);
int ktk_begin_treenode_ex(ktk_Context *ctx, const char *label, int opt);
void ktk_end_treenode(ktk_Context *ctx);
int ktk_begin_window_ex(ktk_Context *ctx, const char *title, ktk_Rect rect, int opt);
void ktk_end_window(ktk_Context *ctx);
void ktk_open_popup(ktk_Context *ctx, const char *name);
int ktk_begin_popup(ktk_Context *ctx, const char *name);
void ktk_end_popup(ktk_Context *ctx);
void ktk_begin_panel_ex(ktk_Context *ctx, const char *name, int opt);
void ktk_end_panel(ktk_Context *ctx);

#endif
