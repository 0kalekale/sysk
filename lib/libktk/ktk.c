/*
libktk: work in process gui library, can be used as normal gui library or intermediate gui library
this program is forked from https://github.com/rxi/microui (05d7b46c9cf650dd0c5fbc83a9bebf87c80d02a5)
the original program was licensed under MIT 
new pieces of program in this fork is licensed under AGPL3
	- 2022 0kalekale
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ktk.h>

#define unused(x) ((void) (x))

#define expect(x) do {                                               \
    if (!(x)) {                                                      \
      fprintf(stderr, "Fatal error: %s:%d: assertion '%s' failed\n", \
        __FILE__, __LINE__, #x);                                     \
      abort();                                                       \
    }                                                                \
  } while (0)

#define push(stk, val) do {                                                 \
    expect((stk).idx < (int) (sizeof((stk).items) / sizeof(*(stk).items))); \
    (stk).items[(stk).idx] = (val);                                         \
    (stk).idx++; /* incremented after incase `val` uses this value */       \
  } while (0)

#define pop(stk) do {      \
    expect((stk).idx > 0); \
    (stk).idx--;           \
  } while (0)


static ktk_Rect unclipped_rect = { 0, 0, 0x1000000, 0x1000000 };

static ktk_Style default_style = {
  /* font | size | padding | spacing | indent */
  NULL, { 68, 10 }, 5, 4, 24,
  /* title_height | scrollbar_size | thumb_size */
  24, 12, 8,
  {
    { 230, 230, 230, 255 }, /* KTK_COLOR_TEXT */
    { 25,  25,  25,  255 }, /* KTK_COLOR_BORDER */
    { 50,  50,  50,  255 }, /* KTK_COLOR_WINDOWBG */
    { 25,  25,  25,  255 }, /* KTK_COLOR_TITLEBG */
    { 240, 240, 240, 255 }, /* KTK_COLOR_TITLETEXT */
    { 0,   0,   0,   0   }, /* KTK_COLOR_PANELBG */
    { 75,  75,  75,  255 }, /* KTK_COLOR_BUTTON */
    { 95,  95,  95,  255 }, /* KTK_COLOR_BUTTONHOVER */
    { 115, 115, 115, 255 }, /* KTK_COLOR_BUTTONFOCUS */
    { 30,  30,  30,  255 }, /* KTK_COLOR_BASE */
    { 35,  35,  35,  255 }, /* KTK_COLOR_BASEHOVER */
    { 40,  40,  40,  255 }, /* KTK_COLOR_BASEFOCUS */
    { 43,  43,  43,  255 }, /* KTK_COLOR_SCROLLBASE */
    { 30,  30,  30,  255 }  /* KTK_COLOR_SCROLLTHUMB */
  }
};


ktk_Vec2 ktk_vec2(int x, int y) {
  ktk_Vec2 res;
  res.x = x; res.y = y;
  return res;
}


ktk_Rect ktk_rect(int x, int y, int w, int h) {
  ktk_Rect res;
  res.x = x; res.y = y; res.w = w; res.h = h;
  return res;
}


ktk_Color ktk_color(int r, int g, int b, int a) {
  ktk_Color res;
  res.r = r; res.g = g; res.b = b; res.a = a;
  return res;
}


static ktk_Rect expand_rect(ktk_Rect rect, int n) {
  return ktk_rect(rect.x - n, rect.y - n, rect.w + n * 2, rect.h + n * 2);
}


static ktk_Rect intersect_rects(ktk_Rect r1, ktk_Rect r2) {
  int x1 = ktk_max(r1.x, r2.x);
  int y1 = ktk_max(r1.y, r2.y);
  int x2 = ktk_min(r1.x + r1.w, r2.x + r2.w);
  int y2 = ktk_min(r1.y + r1.h, r2.y + r2.h);
  if (x2 < x1) { x2 = x1; }
  if (y2 < y1) { y2 = y1; }
  return ktk_rect(x1, y1, x2 - x1, y2 - y1);
}


static int rect_overlaps_vec2(ktk_Rect r, ktk_Vec2 p) {
  return p.x >= r.x && p.x < r.x + r.w && p.y >= r.y && p.y < r.y + r.h;
}


static void draw_frame(ktk_Context *ctx, ktk_Rect rect, int colorid) {
  ktk_draw_rect(ctx, rect, ctx->style->colors[colorid]);
  if (colorid == KTK_COLOR_SCROLLBASE  ||
      colorid == KTK_COLOR_SCROLLTHUMB ||
      colorid == KTK_COLOR_TITLEBG) { return; }
  /* draw border */
  if (ctx->style->colors[KTK_COLOR_BORDER].a) {
    ktk_draw_box(ctx, expand_rect(rect, 1), ctx->style->colors[KTK_COLOR_BORDER]);
  }
}


void ktk_init(ktk_Context *ctx) {
  memset(ctx, 0, sizeof(*ctx));
  ctx->draw_frame = draw_frame;
  ctx->_style = default_style;
  ctx->style = &ctx->_style;
}


void ktk_begin(ktk_Context *ctx) {
  expect(ctx->text_width && ctx->text_height);
  ctx->command_list.idx = 0;
  ctx->root_list.idx = 0;
  ctx->scroll_target = NULL;
  ctx->hover_root = ctx->next_hover_root;
  ctx->next_hover_root = NULL;
  ctx->mouse_delta.x = ctx->mouse_pos.x - ctx->last_mouse_pos.x;
  ctx->mouse_delta.y = ctx->mouse_pos.y - ctx->last_mouse_pos.y;
  ctx->frame++;
}


static int compare_zindex(const void *a, const void *b) {
  return (*(ktk_Container**) a)->zindex - (*(ktk_Container**) b)->zindex;
}


void ktk_end(ktk_Context *ctx) {
  int i, n;
  /* check stacks */
  expect(ctx->container_stack.idx == 0);
  expect(ctx->clip_stack.idx      == 0);
  expect(ctx->id_stack.idx        == 0);
  expect(ctx->layout_stack.idx    == 0);

  /* handle scroll input */
  if (ctx->scroll_target) {
    ctx->scroll_target->scroll.x += ctx->scroll_delta.x;
    ctx->scroll_target->scroll.y += ctx->scroll_delta.y;
  }

  /* unset focus if focus id was not touched this frame */
  if (!ctx->updated_focus) { ctx->focus = 0; }
  ctx->updated_focus = 0;

  /* bring hover root to front if mouse was pressed */
  if (ctx->mouse_pressed && ctx->next_hover_root &&
      ctx->next_hover_root->zindex < ctx->last_zindex &&
      ctx->next_hover_root->zindex >= 0
  ) {
    ktk_bring_to_front(ctx, ctx->next_hover_root);
  }

  /* reset input state */
  ctx->key_pressed = 0;
  ctx->input_text[0] = '\0';
  ctx->mouse_pressed = 0;
  ctx->scroll_delta = ktk_vec2(0, 0);
  ctx->last_mouse_pos = ctx->mouse_pos;

  /* sort root containers by zindex */
  n = ctx->root_list.idx;
  qsort(ctx->root_list.items, n, sizeof(ktk_Container*), compare_zindex);

  /* set root container jump commands */
  for (i = 0; i < n; i++) {
    ktk_Container *cnt = ctx->root_list.items[i];
    /* if this is the first container then make the first command jump to it.
    ** otherwise set the previous container's tail to jump to this one */
    if (i == 0) {
      ktk_Command *cmd = (ktk_Command*) ctx->command_list.items;
      cmd->jump.dst = (char*) cnt->head + sizeof(ktk_JumpCommand);
    } else {
      ktk_Container *prev = ctx->root_list.items[i - 1];
      prev->tail->jump.dst = (char*) cnt->head + sizeof(ktk_JumpCommand);
    }
    /* make the last container's tail jump to the end of command list */
    if (i == n - 1) {
      cnt->tail->jump.dst = ctx->command_list.items + ctx->command_list.idx;
    }
  }
}


void ktk_set_focus(ktk_Context *ctx, ktk_Id id) {
  ctx->focus = id;
  ctx->updated_focus = 1;
}


/* 32bit fnv-1a hash */
#define HASH_INITIAL 2166136261

static void hash(ktk_Id *hash, const void *data, int size) {
  const unsigned char *p = data;
  while (size--) {
    *hash = (*hash ^ *p++) * 16777619;
  }
}


ktk_Id ktk_get_id(ktk_Context *ctx, const void *data, int size) {
  int idx = ctx->id_stack.idx;
  ktk_Id res = (idx > 0) ? ctx->id_stack.items[idx - 1] : HASH_INITIAL;
  hash(&res, data, size);
  ctx->last_id = res;
  return res;
}


void ktk_push_id(ktk_Context *ctx, const void *data, int size) {
  push(ctx->id_stack, ktk_get_id(ctx, data, size));
}


void ktk_pop_id(ktk_Context *ctx) {
  pop(ctx->id_stack);
}


void ktk_push_clip_rect(ktk_Context *ctx, ktk_Rect rect) {
  ktk_Rect last = ktk_get_clip_rect(ctx);
  push(ctx->clip_stack, intersect_rects(rect, last));
}


void ktk_pop_clip_rect(ktk_Context *ctx) {
  pop(ctx->clip_stack);
}


ktk_Rect ktk_get_clip_rect(ktk_Context *ctx) {
  expect(ctx->clip_stack.idx > 0);
  return ctx->clip_stack.items[ctx->clip_stack.idx - 1];
}


int ktk_check_clip(ktk_Context *ctx, ktk_Rect r) {
  ktk_Rect cr = ktk_get_clip_rect(ctx);
  if (r.x > cr.x + cr.w || r.x + r.w < cr.x ||
      r.y > cr.y + cr.h || r.y + r.h < cr.y   ) { return KTK_CLIP_ALL; }
  if (r.x >= cr.x && r.x + r.w <= cr.x + cr.w &&
      r.y >= cr.y && r.y + r.h <= cr.y + cr.h ) { return 0; }
  return KTK_CLIP_PART;
}


static void push_layout(ktk_Context *ctx, ktk_Rect body, ktk_Vec2 scroll) {
  ktk_Layout layout;
  int width = 0;
  memset(&layout, 0, sizeof(layout));
  layout.body = ktk_rect(body.x - scroll.x, body.y - scroll.y, body.w, body.h);
  layout.max = ktk_vec2(-0x1000000, -0x1000000);
  push(ctx->layout_stack, layout);
  ktk_layout_row(ctx, 1, &width, 0);
}


static ktk_Layout* get_layout(ktk_Context *ctx) {
  return &ctx->layout_stack.items[ctx->layout_stack.idx - 1];
}


static void pop_container(ktk_Context *ctx) {
  ktk_Container *cnt = ktk_get_current_container(ctx);
  ktk_Layout *layout = get_layout(ctx);
  cnt->content_size.x = layout->max.x - layout->body.x;
  cnt->content_size.y = layout->max.y - layout->body.y;
  /* pop container, layout and id */
  pop(ctx->container_stack);
  pop(ctx->layout_stack);
  ktk_pop_id(ctx);
}


ktk_Container* ktk_get_current_container(ktk_Context *ctx) {
  expect(ctx->container_stack.idx > 0);
  return ctx->container_stack.items[ ctx->container_stack.idx - 1 ];
}


static ktk_Container* get_container(ktk_Context *ctx, ktk_Id id, int opt) {
  ktk_Container *cnt;
  /* try to get existing container from pool */
  int idx = ktk_pool_get(ctx, ctx->container_pool, KTK_CONTAINERPOOL_SIZE, id);
  if (idx >= 0) {
    if (ctx->containers[idx].open || ~opt & KTK_OPT_CLOSED) {
      ktk_pool_update(ctx, ctx->container_pool, idx);
    }
    return &ctx->containers[idx];
  }
  if (opt & KTK_OPT_CLOSED) { return NULL; }
  /* container not found in pool: init new container */
  idx = ktk_pool_init(ctx, ctx->container_pool, KTK_CONTAINERPOOL_SIZE, id);
  cnt = &ctx->containers[idx];
  memset(cnt, 0, sizeof(*cnt));
  cnt->open = 1;
  ktk_bring_to_front(ctx, cnt);
  return cnt;
}


ktk_Container* ktk_get_container(ktk_Context *ctx, const char *name) {
  ktk_Id id = ktk_get_id(ctx, name, strlen(name));
  return get_container(ctx, id, 0);
}


void ktk_bring_to_front(ktk_Context *ctx, ktk_Container *cnt) {
  cnt->zindex = ++ctx->last_zindex;
}


/*============================================================================
** pool
**============================================================================*/

int ktk_pool_init(ktk_Context *ctx, ktk_PoolItem *items, int len, ktk_Id id) {
  int i, n = -1, f = ctx->frame;
  for (i = 0; i < len; i++) {
    if (items[i].last_update < f) {
      f = items[i].last_update;
      n = i;
    }
  }
  expect(n > -1);
  items[n].id = id;
  ktk_pool_update(ctx, items, n);
  return n;
}


int ktk_pool_get(ktk_Context *ctx, ktk_PoolItem *items, int len, ktk_Id id) {
  int i;
  unused(ctx);
  for (i = 0; i < len; i++) {
    if (items[i].id == id) { return i; }
  }
  return -1;
}


void ktk_pool_update(ktk_Context *ctx, ktk_PoolItem *items, int idx) {
  items[idx].last_update = ctx->frame;
}


/*============================================================================
** input handlers
**============================================================================*/

void ktk_input_mousemove(ktk_Context *ctx, int x, int y) {
  ctx->mouse_pos = ktk_vec2(x, y);
}


void ktk_input_mousedown(ktk_Context *ctx, int x, int y, int btn) {
  ktk_input_mousemove(ctx, x, y);
  ctx->mouse_down |= btn;
  ctx->mouse_pressed |= btn;
}


void ktk_input_mouseup(ktk_Context *ctx, int x, int y, int btn) {
  ktk_input_mousemove(ctx, x, y);
  ctx->mouse_down &= ~btn;
}


void ktk_input_scroll(ktk_Context *ctx, int x, int y) {
  ctx->scroll_delta.x += x;
  ctx->scroll_delta.y += y;
}


void ktk_input_keydown(ktk_Context *ctx, int key) {
  ctx->key_pressed |= key;
  ctx->key_down |= key;
}


void ktk_input_keyup(ktk_Context *ctx, int key) {
  ctx->key_down &= ~key;
}


void ktk_input_text(ktk_Context *ctx, const char *text) {
  int len = strlen(ctx->input_text);
  int size = strlen(text) + 1;
  expect(len + size <= (int) sizeof(ctx->input_text));
  memcpy(ctx->input_text + len, text, size);
}


/*============================================================================
** commandlist
**============================================================================*/

ktk_Command* ktk_push_command(ktk_Context *ctx, int type, int size) {
  ktk_Command *cmd = (ktk_Command*) (ctx->command_list.items + ctx->command_list.idx);
  expect(ctx->command_list.idx + size < KTK_COMMANDLIST_SIZE);
  cmd->base.type = type;
  cmd->base.size = size;
  ctx->command_list.idx += size;
  return cmd;
}


int ktk_next_command(ktk_Context *ctx, ktk_Command **cmd) {
  if (*cmd) {
    *cmd = (ktk_Command*) (((char*) *cmd) + (*cmd)->base.size);
  } else {
    *cmd = (ktk_Command*) ctx->command_list.items;
  }
  while ((char*) *cmd != ctx->command_list.items + ctx->command_list.idx) {
    if ((*cmd)->type != KTK_COMMAND_JUMP) { return 1; }
    *cmd = (*cmd)->jump.dst;
  }
  return 0;
}


static ktk_Command* push_jump(ktk_Context *ctx, ktk_Command *dst) {
  ktk_Command *cmd;
  cmd = ktk_push_command(ctx, KTK_COMMAND_JUMP, sizeof(ktk_JumpCommand));
  cmd->jump.dst = dst;
  return cmd;
}


void ktk_set_clip(ktk_Context *ctx, ktk_Rect rect) {
  ktk_Command *cmd;
  cmd = ktk_push_command(ctx, KTK_COMMAND_CLIP, sizeof(ktk_ClipCommand));
  cmd->clip.rect = rect;
}


void ktk_draw_rect(ktk_Context *ctx, ktk_Rect rect, ktk_Color color) {
  ktk_Command *cmd;
  rect = intersect_rects(rect, ktk_get_clip_rect(ctx));
  if (rect.w > 0 && rect.h > 0) {
    cmd = ktk_push_command(ctx, KTK_COMMAND_RECT, sizeof(ktk_RectCommand));
    cmd->rect.rect = rect;
    cmd->rect.color = color;
  }
}


void ktk_draw_box(ktk_Context *ctx, ktk_Rect rect, ktk_Color color) {
  ktk_draw_rect(ctx, ktk_rect(rect.x + 1, rect.y, rect.w - 2, 1), color);
  ktk_draw_rect(ctx, ktk_rect(rect.x + 1, rect.y + rect.h - 1, rect.w - 2, 1), color);
  ktk_draw_rect(ctx, ktk_rect(rect.x, rect.y, 1, rect.h), color);
  ktk_draw_rect(ctx, ktk_rect(rect.x + rect.w - 1, rect.y, 1, rect.h), color);
}


void ktk_draw_text(ktk_Context *ctx, ktk_Font font, const char *str, int len,
  ktk_Vec2 pos, ktk_Color color)
{
  ktk_Command *cmd;
  ktk_Rect rect = ktk_rect(
    pos.x, pos.y, ctx->text_width(font, str, len), ctx->text_height(font));
  int clipped = ktk_check_clip(ctx, rect);
  if (clipped == KTK_CLIP_ALL ) { return; }
  if (clipped == KTK_CLIP_PART) { ktk_set_clip(ctx, ktk_get_clip_rect(ctx)); }
  /* add command */
  if (len < 0) { len = strlen(str); }
  cmd = ktk_push_command(ctx, KTK_COMMAND_TEXT, sizeof(ktk_TextCommand) + len);
  memcpy(cmd->text.str, str, len);
  cmd->text.str[len] = '\0';
  cmd->text.pos = pos;
  cmd->text.color = color;
  cmd->text.font = font;
  /* reset clipping if it was set */
  if (clipped) { ktk_set_clip(ctx, unclipped_rect); }
}


void ktk_draw_icon(ktk_Context *ctx, int id, ktk_Rect rect, ktk_Color color) {
  ktk_Command *cmd;
  /* do clip command if the rect isn't fully contained within the cliprect */
  int clipped = ktk_check_clip(ctx, rect);
  if (clipped == KTK_CLIP_ALL ) { return; }
  if (clipped == KTK_CLIP_PART) { ktk_set_clip(ctx, ktk_get_clip_rect(ctx)); }
  /* do icon command */
  cmd = ktk_push_command(ctx, KTK_COMMAND_ICON, sizeof(ktk_IconCommand));
  cmd->icon.id = id;
  cmd->icon.rect = rect;
  cmd->icon.color = color;
  /* reset clipping if it was set */
  if (clipped) { ktk_set_clip(ctx, unclipped_rect); }
}


/*============================================================================
** layout
**============================================================================*/

enum { RELATIVE = 1, ABSOLUTE = 2 };


void ktk_layout_begin_column(ktk_Context *ctx) {
  push_layout(ctx, ktk_layout_next(ctx), ktk_vec2(0, 0));
}


void ktk_layout_end_column(ktk_Context *ctx) {
  ktk_Layout *a, *b;
  b = get_layout(ctx);
  pop(ctx->layout_stack);
  /* inherit position/next_row/max from child layout if they are greater */
  a = get_layout(ctx);
  a->position.x = ktk_max(a->position.x, b->position.x + b->body.x - a->body.x);
  a->next_row = ktk_max(a->next_row, b->next_row + b->body.y - a->body.y);
  a->max.x = ktk_max(a->max.x, b->max.x);
  a->max.y = ktk_max(a->max.y, b->max.y);
}


void ktk_layout_row(ktk_Context *ctx, int items, const int *widths, int height) {
  ktk_Layout *layout = get_layout(ctx);
  if (widths) {
    expect(items <= KTK_MAX_WIDTHS);
    memcpy(layout->widths, widths, items * sizeof(widths[0]));
  }
  layout->items = items;
  layout->position = ktk_vec2(layout->indent, layout->next_row);
  layout->size.y = height;
  layout->item_index = 0;
}


void ktk_layout_width(ktk_Context *ctx, int width) {
  get_layout(ctx)->size.x = width;
}


void ktk_layout_height(ktk_Context *ctx, int height) {
  get_layout(ctx)->size.y = height;
}


void ktk_layout_set_next(ktk_Context *ctx, ktk_Rect r, int relative) {
  ktk_Layout *layout = get_layout(ctx);
  layout->next = r;
  layout->next_type = relative ? RELATIVE : ABSOLUTE;
}


ktk_Rect ktk_layout_next(ktk_Context *ctx) {
  ktk_Layout *layout = get_layout(ctx);
  ktk_Style *style = ctx->style;
  ktk_Rect res;

  if (layout->next_type) {
    /* handle rect set by `ktk_layout_set_next` */
    int type = layout->next_type;
    layout->next_type = 0;
    res = layout->next;
    if (type == ABSOLUTE) { return (ctx->last_rect = res); }

  } else {
    /* handle next row */
    if (layout->item_index == layout->items) {
      ktk_layout_row(ctx, layout->items, NULL, layout->size.y);
    }

    /* position */
    res.x = layout->position.x;
    res.y = layout->position.y;

    /* size */
    res.w = layout->items > 0 ? layout->widths[layout->item_index] : layout->size.x;
    res.h = layout->size.y;
    if (res.w == 0) { res.w = style->size.x + style->padding * 2; }
    if (res.h == 0) { res.h = style->size.y + style->padding * 2; }
    if (res.w <  0) { res.w += layout->body.w - res.x + 1; }
    if (res.h <  0) { res.h += layout->body.h - res.y + 1; }

    layout->item_index++;
  }

  /* update position */
  layout->position.x += res.w + style->spacing;
  layout->next_row = ktk_max(layout->next_row, res.y + res.h + style->spacing);

  /* apply body offset */
  res.x += layout->body.x;
  res.y += layout->body.y;

  /* update max position */
  layout->max.x = ktk_max(layout->max.x, res.x + res.w);
  layout->max.y = ktk_max(layout->max.y, res.y + res.h);

  return (ctx->last_rect = res);
}


/*============================================================================
** controls
**============================================================================*/

static int in_hover_root(ktk_Context *ctx) {
  int i = ctx->container_stack.idx;
  while (i--) {
    if (ctx->container_stack.items[i] == ctx->hover_root) { return 1; }
    /* only root containers have their `head` field set; stop searching if we've
    ** reached the current root container */
    if (ctx->container_stack.items[i]->head) { break; }
  }
  return 0;
}


void ktk_draw_control_frame(ktk_Context *ctx, ktk_Id id, ktk_Rect rect,
  int colorid, int opt)
{
  if (opt & KTK_OPT_NOFRAME) { return; }
  colorid += (ctx->focus == id) ? 2 : (ctx->hover == id) ? 1 : 0;
  ctx->draw_frame(ctx, rect, colorid);
}


void ktk_draw_control_text(ktk_Context *ctx, const char *str, ktk_Rect rect,
  int colorid, int opt)
{
  ktk_Vec2 pos;
  ktk_Font font = ctx->style->font;
  int tw = ctx->text_width(font, str, -1);
  ktk_push_clip_rect(ctx, rect);
  pos.y = rect.y + (rect.h - ctx->text_height(font)) / 2;
  if (opt & KTK_OPT_ALIGNCENTER) {
    pos.x = rect.x + (rect.w - tw) / 2;
  } else if (opt & KTK_OPT_ALIGNRIGHT) {
    pos.x = rect.x + rect.w - tw - ctx->style->padding;
  } else {
    pos.x = rect.x + ctx->style->padding;
  }
  ktk_draw_text(ctx, font, str, -1, pos, ctx->style->colors[colorid]);
  ktk_pop_clip_rect(ctx);
}


int ktk_mouse_over(ktk_Context *ctx, ktk_Rect rect) {
  return rect_overlaps_vec2(rect, ctx->mouse_pos) &&
    rect_overlaps_vec2(ktk_get_clip_rect(ctx), ctx->mouse_pos) &&
    in_hover_root(ctx);
}


void ktk_update_control(ktk_Context *ctx, ktk_Id id, ktk_Rect rect, int opt) {
  int mouseover = ktk_mouse_over(ctx, rect);

  if (ctx->focus == id) { ctx->updated_focus = 1; }
  if (opt & KTK_OPT_NOINTERACT) { return; }
  if (mouseover && !ctx->mouse_down) { ctx->hover = id; }

  if (ctx->focus == id) {
    if (ctx->mouse_pressed && !mouseover) { ktk_set_focus(ctx, 0); }
    if (!ctx->mouse_down && ~opt & KTK_OPT_HOLDFOCUS) { ktk_set_focus(ctx, 0); }
  }

  if (ctx->hover == id) {
    if (ctx->mouse_pressed) {
      ktk_set_focus(ctx, id);
    } else if (!mouseover) {
      ctx->hover = 0;
    }
  }
}


void ktk_text(ktk_Context *ctx, const char *text) {
  const char *start, *end, *p = text;
  int width = -1;
  ktk_Font font = ctx->style->font;
  ktk_Color color = ctx->style->colors[KTK_COLOR_TEXT];
  ktk_layout_begin_column(ctx);
  ktk_layout_row(ctx, 1, &width, ctx->text_height(font));
  do {
    ktk_Rect r = ktk_layout_next(ctx);
    int w = 0;
    start = end = p;
    do {
      const char* word = p;
      while (*p && *p != ' ' && *p != '\n') { p++; }
      w += ctx->text_width(font, word, p - word);
      if (w > r.w && end != start) { break; }
      w += ctx->text_width(font, p, 1);
      end = p++;
    } while (*end && *end != '\n');
    ktk_draw_text(ctx, font, start, end - start, ktk_vec2(r.x, r.y), color);
    p = end + 1;
  } while (*end);
  ktk_layout_end_column(ctx);
}


void ktk_label(ktk_Context *ctx, const char *text) {
  ktk_draw_control_text(ctx, text, ktk_layout_next(ctx), KTK_COLOR_TEXT, 0);
}


int ktk_button_ex(ktk_Context *ctx, const char *label, int icon, int opt) {
  int res = 0;
  ktk_Id id = label ? ktk_get_id(ctx, label, strlen(label))
                   : ktk_get_id(ctx, &icon, sizeof(icon));
  ktk_Rect r = ktk_layout_next(ctx);
  ktk_update_control(ctx, id, r, opt);
  /* handle click */
  if (ctx->mouse_pressed == KTK_MOUSE_LEFT && ctx->focus == id) {
    res |= KTK_RES_SUBMIT;
  }
  /* draw */
  ktk_draw_control_frame(ctx, id, r, KTK_COLOR_BUTTON, opt);
  if (label) { ktk_draw_control_text(ctx, label, r, KTK_COLOR_TEXT, opt); }
  if (icon) { ktk_draw_icon(ctx, icon, r, ctx->style->colors[KTK_COLOR_TEXT]); }
  return res;
}


int ktk_checkbox(ktk_Context *ctx, const char *label, int *state) {
  int res = 0;
  ktk_Id id = ktk_get_id(ctx, &state, sizeof(state));
  ktk_Rect r = ktk_layout_next(ctx);
  ktk_Rect box = ktk_rect(r.x, r.y, r.h, r.h);
  ktk_update_control(ctx, id, r, 0);
  /* handle click */
  if (ctx->mouse_pressed == KTK_MOUSE_LEFT && ctx->focus == id) {
    res |= KTK_RES_CHANGE;
    *state = !*state;
  }
  /* draw */
  ktk_draw_control_frame(ctx, id, box, KTK_COLOR_BASE, 0);
  if (*state) {
    ktk_draw_icon(ctx, KTK_ICON_CHECK, box, ctx->style->colors[KTK_COLOR_TEXT]);
  }
  r = ktk_rect(r.x + box.w, r.y, r.w - box.w, r.h);
  ktk_draw_control_text(ctx, label, r, KTK_COLOR_TEXT, 0);
  return res;
}


int ktk_textbox_raw(ktk_Context *ctx, char *buf, int bufsz, ktk_Id id, ktk_Rect r,
  int opt)
{
  int res = 0;
  ktk_update_control(ctx, id, r, opt | KTK_OPT_HOLDFOCUS);

  if (ctx->focus == id) {
    /* handle text input */
    int len = strlen(buf);
    int n = ktk_min(bufsz - len - 1, (int) strlen(ctx->input_text));
    if (n > 0) {
      memcpy(buf + len, ctx->input_text, n);
      len += n;
      buf[len] = '\0';
      res |= KTK_RES_CHANGE;
    }
    /* handle backspace */
    if (ctx->key_pressed & KTK_KEY_BACKSPACE && len > 0) {
      /* skip utf-8 continuation bytes */
      while ((buf[--len] & 0xc0) == 0x80 && len > 0);
      buf[len] = '\0';
      res |= KTK_RES_CHANGE;
    }
    /* handle return */
    if (ctx->key_pressed & KTK_KEY_RETURN) {
      ktk_set_focus(ctx, 0);
      res |= KTK_RES_SUBMIT;
    }
  }

  /* draw */
  ktk_draw_control_frame(ctx, id, r, KTK_COLOR_BASE, opt);
  if (ctx->focus == id) {
    ktk_Color color = ctx->style->colors[KTK_COLOR_TEXT];
    ktk_Font font = ctx->style->font;
    int textw = ctx->text_width(font, buf, -1);
    int texth = ctx->text_height(font);
    int ofx = r.w - ctx->style->padding - textw - 1;
    int textx = r.x + ktk_min(ofx, ctx->style->padding);
    int texty = r.y + (r.h - texth) / 2;
    ktk_push_clip_rect(ctx, r);
    ktk_draw_text(ctx, font, buf, -1, ktk_vec2(textx, texty), color);
    ktk_draw_rect(ctx, ktk_rect(textx + textw, texty, 1, texth), color);
    ktk_pop_clip_rect(ctx);
  } else {
    ktk_draw_control_text(ctx, buf, r, KTK_COLOR_TEXT, opt);
  }

  return res;
}


static int number_textbox(ktk_Context *ctx, ktk_Real *value, ktk_Rect r, ktk_Id id) {
  if (ctx->mouse_pressed == KTK_MOUSE_LEFT && ctx->key_down & KTK_KEY_SHIFT &&
      ctx->hover == id
  ) {
    ctx->number_edit = id;
    sprintf(ctx->number_edit_buf, KTK_REAL_FMT, *value);
  }
  if (ctx->number_edit == id) {
    int res = ktk_textbox_raw(
      ctx, ctx->number_edit_buf, sizeof(ctx->number_edit_buf), id, r, 0);
    if (res & KTK_RES_SUBMIT || ctx->focus != id) {
      *value = strtod(ctx->number_edit_buf, NULL);
      ctx->number_edit = 0;
    } else {
      return 1;
    }
  }
  return 0;
}


int ktk_textbox_ex(ktk_Context *ctx, char *buf, int bufsz, int opt) {
  ktk_Id id = ktk_get_id(ctx, &buf, sizeof(buf));
  ktk_Rect r = ktk_layout_next(ctx);
  return ktk_textbox_raw(ctx, buf, bufsz, id, r, opt);
}


int ktk_slider_ex(ktk_Context *ctx, ktk_Real *value, ktk_Real low, ktk_Real high,
  ktk_Real step, const char *fmt, int opt)
{
  char buf[KTK_MAX_FMT + 1];
  ktk_Rect thumb;
  int x, w, res = 0;
  ktk_Real last = *value, v = last;
  ktk_Id id = ktk_get_id(ctx, &value, sizeof(value));
  ktk_Rect base = ktk_layout_next(ctx);

  /* handle text input mode */
  if (number_textbox(ctx, &v, base, id)) { return res; }

  /* handle normal mode */
  ktk_update_control(ctx, id, base, opt);

  /* handle input */
  if (ctx->focus == id &&
      (ctx->mouse_down | ctx->mouse_pressed) == KTK_MOUSE_LEFT)
  {
    v = low + (ctx->mouse_pos.x - base.x) * (high - low) / base.w;
    if (step) { v = (((v + step / 2) / step)) * step; }
  }
  /* clamp and store value, update res */
  *value = v = ktk_clamp(v, low, high);
  if (last != v) { res |= KTK_RES_CHANGE; }

  /* draw base */
  ktk_draw_control_frame(ctx, id, base, KTK_COLOR_BASE, opt);
  /* draw thumb */
  w = ctx->style->thumb_size;
  x = (v - low) * (base.w - w) / (high - low);
  thumb = ktk_rect(base.x + x, base.y, w, base.h);
  ktk_draw_control_frame(ctx, id, thumb, KTK_COLOR_BUTTON, opt);
  /* draw text  */
  sprintf(buf, fmt, v);
  ktk_draw_control_text(ctx, buf, base, KTK_COLOR_TEXT, opt);

  return res;
}


int ktk_number_ex(ktk_Context *ctx, ktk_Real *value, ktk_Real step,
  const char *fmt, int opt)
{
  char buf[KTK_MAX_FMT + 1];
  int res = 0;
  ktk_Id id = ktk_get_id(ctx, &value, sizeof(value));
  ktk_Rect base = ktk_layout_next(ctx);
  ktk_Real last = *value;

  /* handle text input mode */
  if (number_textbox(ctx, value, base, id)) { return res; }

  /* handle normal mode */
  ktk_update_control(ctx, id, base, opt);

  /* handle input */
  if (ctx->focus == id && ctx->mouse_down == KTK_MOUSE_LEFT) {
    *value += ctx->mouse_delta.x * step;
  }
  /* set flag if value changed */
  if (*value != last) { res |= KTK_RES_CHANGE; }

  /* draw base */
  ktk_draw_control_frame(ctx, id, base, KTK_COLOR_BASE, opt);
  /* draw text  */
  sprintf(buf, fmt, *value);
  ktk_draw_control_text(ctx, buf, base, KTK_COLOR_TEXT, opt);

  return res;
}


static int header(ktk_Context *ctx, const char *label, int istreenode, int opt) {
  ktk_Rect r;
  int active, expanded;
  ktk_Id id = ktk_get_id(ctx, label, strlen(label));
  int idx = ktk_pool_get(ctx, ctx->treenode_pool, KTK_TREENODEPOOL_SIZE, id);
  int width = -1;
  ktk_layout_row(ctx, 1, &width, 0);

  active = (idx >= 0);
  expanded = (opt & KTK_OPT_EXPANDED) ? !active : active;
  r = ktk_layout_next(ctx);
  ktk_update_control(ctx, id, r, 0);

  /* handle click */
  active ^= (ctx->mouse_pressed == KTK_MOUSE_LEFT && ctx->focus == id);

  /* update pool ref */
  if (idx >= 0) {
    if (active) { ktk_pool_update(ctx, ctx->treenode_pool, idx); }
           else { memset(&ctx->treenode_pool[idx], 0, sizeof(ktk_PoolItem)); }
  } else if (active) {
    ktk_pool_init(ctx, ctx->treenode_pool, KTK_TREENODEPOOL_SIZE, id);
  }

  /* draw */
  if (istreenode) {
    if (ctx->hover == id) { ctx->draw_frame(ctx, r, KTK_COLOR_BUTTONHOVER); }
  } else {
    ktk_draw_control_frame(ctx, id, r, KTK_COLOR_BUTTON, 0);
  }
  ktk_draw_icon(
    ctx, expanded ? KTK_ICON_EXPANDED : KTK_ICON_COLLAPSED,
    ktk_rect(r.x, r.y, r.h, r.h), ctx->style->colors[KTK_COLOR_TEXT]);
  r.x += r.h - ctx->style->padding;
  r.w -= r.h - ctx->style->padding;
  ktk_draw_control_text(ctx, label, r, KTK_COLOR_TEXT, 0);

  return expanded ? KTK_RES_ACTIVE : 0;
}


int ktk_header_ex(ktk_Context *ctx, const char *label, int opt) {
  return header(ctx, label, 0, opt);
}


int ktk_begin_treenode_ex(ktk_Context *ctx, const char *label, int opt) {
  int res = header(ctx, label, 1, opt);
  if (res & KTK_RES_ACTIVE) {
    get_layout(ctx)->indent += ctx->style->indent;
    push(ctx->id_stack, ctx->last_id);
  }
  return res;
}


void ktk_end_treenode(ktk_Context *ctx) {
  get_layout(ctx)->indent -= ctx->style->indent;
  ktk_pop_id(ctx);
}


#define scrollbar(ctx, cnt, b, cs, x, y, w, h)                              \
  do {                                                                      \
    /* only add scrollbar if content size is larger than body */            \
    int maxscroll = cs.y - b->h;                                            \
                                                                            \
    if (maxscroll > 0 && b->h > 0) {                                        \
      ktk_Rect base, thumb;                                                  \
      ktk_Id id = ktk_get_id(ctx, "!scrollbar" #y, 11);                       \
                                                                            \
      /* get sizing / positioning */                                        \
      base = *b;                                                            \
      base.x = b->x + b->w;                                                 \
      base.w = ctx->style->scrollbar_size;                                  \
                                                                            \
      /* handle input */                                                    \
      ktk_update_control(ctx, id, base, 0);                                  \
      if (ctx->focus == id && ctx->mouse_down == KTK_MOUSE_LEFT) {           \
        cnt->scroll.y += ctx->mouse_delta.y * cs.y / base.h;                \
      }                                                                     \
      /* clamp scroll to limits */                                          \
      cnt->scroll.y = ktk_clamp(cnt->scroll.y, 0, maxscroll);                \
                                                                            \
      /* draw base and thumb */                                             \
      ctx->draw_frame(ctx, base, KTK_COLOR_SCROLLBASE);                      \
      thumb = base;                                                         \
      thumb.h = ktk_max(ctx->style->thumb_size, base.h * b->h / cs.y);       \
      thumb.y += cnt->scroll.y * (base.h - thumb.h) / maxscroll;            \
      ctx->draw_frame(ctx, thumb, KTK_COLOR_SCROLLTHUMB);                    \
                                                                            \
      /* set this as the scroll_target (will get scrolled on mousewheel) */ \
      /* if the mouse is over it */                                         \
      if (ktk_mouse_over(ctx, *b)) { ctx->scroll_target = cnt; }             \
    } else {                                                                \
      cnt->scroll.y = 0;                                                    \
    }                                                                       \
  } while (0)


static void scrollbars(ktk_Context *ctx, ktk_Container *cnt, ktk_Rect *body) {
  int sz = ctx->style->scrollbar_size;
  ktk_Vec2 cs = cnt->content_size;
  cs.x += ctx->style->padding * 2;
  cs.y += ctx->style->padding * 2;
  ktk_push_clip_rect(ctx, *body);
  /* resize body to make room for scrollbars */
  if (cs.y > cnt->body.h) { body->w -= sz; }
  if (cs.x > cnt->body.w) { body->h -= sz; }
  /* to create a horizontal or vertical scrollbar almost-identical code is
  ** used; only the references to `x|y` `w|h` need to be switched */
  scrollbar(ctx, cnt, body, cs, x, y, w, h);
  scrollbar(ctx, cnt, body, cs, y, x, h, w);
  ktk_pop_clip_rect(ctx);
}


static void push_container_body(
  ktk_Context *ctx, ktk_Container *cnt, ktk_Rect body, int opt
) {
  if (~opt & KTK_OPT_NOSCROLL) { scrollbars(ctx, cnt, &body); }
  push_layout(ctx, expand_rect(body, -ctx->style->padding), cnt->scroll);
  cnt->body = body;
}


static void begin_root_container(ktk_Context *ctx, ktk_Container *cnt) {
  push(ctx->container_stack, cnt);
  /* push container to roots list and push head command */
  push(ctx->root_list, cnt);
  cnt->head = push_jump(ctx, NULL);
  /* set as hover root if the mouse is overlapping this container and it has a
  ** higher zindex than the current hover root */
  if (rect_overlaps_vec2(cnt->rect, ctx->mouse_pos) &&
      (!ctx->next_hover_root || cnt->zindex > ctx->next_hover_root->zindex)
  ) {
    ctx->next_hover_root = cnt;
  }
  /* clipping is reset here in case a root-container is made within
  ** another root-containers's begin/end block; this prevents the inner
  ** root-container being clipped to the outer */
  push(ctx->clip_stack, unclipped_rect);
}


static void end_root_container(ktk_Context *ctx) {
  /* push tail 'goto' jump command and set head 'skip' command. the final steps
  ** on initing these are done in ktk_end() */
  ktk_Container *cnt = ktk_get_current_container(ctx);
  cnt->tail = push_jump(ctx, NULL);
  cnt->head->jump.dst = ctx->command_list.items + ctx->command_list.idx;
  /* pop base clip rect and container */
  ktk_pop_clip_rect(ctx);
  pop_container(ctx);
}


int ktk_begin_window_ex(ktk_Context *ctx, const char *title, ktk_Rect rect, int opt) {
  ktk_Rect body;
  ktk_Id id = ktk_get_id(ctx, title, strlen(title));
  ktk_Container *cnt = get_container(ctx, id, opt);
  if (!cnt || !cnt->open) { return 0; }
  push(ctx->id_stack, id);

  if (cnt->rect.w == 0) { cnt->rect = rect; }
  begin_root_container(ctx, cnt);
  rect = body = cnt->rect;

  /* draw frame */
  if (~opt & KTK_OPT_NOFRAME) {
    ctx->draw_frame(ctx, rect, KTK_COLOR_WINDOWBG);
  }

  /* do title bar */
  if (~opt & KTK_OPT_NOTITLE) {
    ktk_Rect tr = rect;
    tr.h = ctx->style->title_height;
    ctx->draw_frame(ctx, tr, KTK_COLOR_TITLEBG);

    /* do title text */
    if (~opt & KTK_OPT_NOTITLE) {
      ktk_Id id = ktk_get_id(ctx, "!title", 6);
      ktk_update_control(ctx, id, tr, opt);
      ktk_draw_control_text(ctx, title, tr, KTK_COLOR_TITLETEXT, opt);
      if (id == ctx->focus && ctx->mouse_down == KTK_MOUSE_LEFT) {
        cnt->rect.x += ctx->mouse_delta.x;
        cnt->rect.y += ctx->mouse_delta.y;
      }
      body.y += tr.h;
      body.h -= tr.h;
    }

    /* do `close` button */
    if (~opt & KTK_OPT_NOCLOSE) {
      ktk_Id id = ktk_get_id(ctx, "!close", 6);
      ktk_Rect r = ktk_rect(tr.x + tr.w - tr.h, tr.y, tr.h, tr.h);
      tr.w -= r.w;
      ktk_draw_icon(ctx, KTK_ICON_CLOSE, r, ctx->style->colors[KTK_COLOR_TITLETEXT]);
      ktk_update_control(ctx, id, r, opt);
      if (ctx->mouse_pressed == KTK_MOUSE_LEFT && id == ctx->focus) {
        cnt->open = 0;
      }
    }
  }

  push_container_body(ctx, cnt, body, opt);

  /* do `resize` handle */
  if (~opt & KTK_OPT_NORESIZE) {
    int sz = ctx->style->title_height;
    ktk_Id id = ktk_get_id(ctx, "!resize", 7);
    ktk_Rect r = ktk_rect(rect.x + rect.w - sz, rect.y + rect.h - sz, sz, sz);
    ktk_update_control(ctx, id, r, opt);
    if (id == ctx->focus && ctx->mouse_down == KTK_MOUSE_LEFT) {
      cnt->rect.w = ktk_max(96, cnt->rect.w + ctx->mouse_delta.x);
      cnt->rect.h = ktk_max(64, cnt->rect.h + ctx->mouse_delta.y);
    }
  }

  /* resize to content size */
  if (opt & KTK_OPT_AUTOSIZE) {
    ktk_Rect r = get_layout(ctx)->body;
    cnt->rect.w = cnt->content_size.x + (cnt->rect.w - r.w);
    cnt->rect.h = cnt->content_size.y + (cnt->rect.h - r.h);
  }

  /* close if this is a popup window and elsewhere was clicked */
  if (opt & KTK_OPT_POPUP && ctx->mouse_pressed && ctx->hover_root != cnt) {
    cnt->open = 0;
  }

  ktk_push_clip_rect(ctx, cnt->body);
  return KTK_RES_ACTIVE;
}


void ktk_end_window(ktk_Context *ctx) {
  ktk_pop_clip_rect(ctx);
  end_root_container(ctx);
}


void ktk_open_popup(ktk_Context *ctx, const char *name) {
  ktk_Container *cnt = ktk_get_container(ctx, name);
  /* set as hover root so popup isn't closed in begin_window_ex()  */
  ctx->hover_root = ctx->next_hover_root = cnt;
  /* position at mouse cursor, open and bring-to-front */
  cnt->rect = ktk_rect(ctx->mouse_pos.x, ctx->mouse_pos.y, 1, 1);
  cnt->open = 1;
  ktk_bring_to_front(ctx, cnt);
}


int ktk_begin_popup(ktk_Context *ctx, const char *name) {
  int opt = KTK_OPT_POPUP | KTK_OPT_AUTOSIZE | KTK_OPT_NORESIZE |
            KTK_OPT_NOSCROLL | KTK_OPT_NOTITLE | KTK_OPT_CLOSED;
  return ktk_begin_window_ex(ctx, name, ktk_rect(0, 0, 0, 0), opt);
}


void ktk_end_popup(ktk_Context *ctx) {
  ktk_end_window(ctx);
}


void ktk_begin_panel_ex(ktk_Context *ctx, const char *name, int opt) {
  ktk_Container *cnt;
  ktk_push_id(ctx, name, strlen(name));
  cnt = get_container(ctx, ctx->last_id, opt);
  cnt->rect = ktk_layout_next(ctx);
  if (~opt & KTK_OPT_NOFRAME) {
    ctx->draw_frame(ctx, cnt->rect, KTK_COLOR_PANELBG);
  }
  push(ctx->container_stack, cnt);
  push_container_body(ctx, cnt, cnt->rect, opt);
  ktk_push_clip_rect(ctx, cnt->body);
}


void ktk_end_panel(ktk_Context *ctx) {
  ktk_pop_clip_rect(ctx);
  pop_container(ctx);
}
