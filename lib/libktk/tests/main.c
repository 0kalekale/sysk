#include <SDL2/SDL.h>
#include <stdio.h>
#include "renderer.h"
#include <ktk.h>


static  char logbuf[64000];
static   int logbuf_updated = 0;
static float bg[3] = { 90, 95, 100 };


static void write_log(const char *text) {
  if (logbuf[0]) { strcat(logbuf, "\n"); }
  strcat(logbuf, text);
  logbuf_updated = 1;
}


static void test_window(ktk_Context *ctx) {
  /* do window */
  if (ktk_begin_window(ctx, "Demo Window", ktk_rect(40, 40, 300, 450))) {
    ktk_Container *win = ktk_get_current_container(ctx);
    win->rect.w = ktk_max(win->rect.w, 240);
    win->rect.h = ktk_max(win->rect.h, 300);

    /* window info */
    if (ktk_header(ctx, "Window Info")) {
      ktk_Container *win = ktk_get_current_container(ctx);
      char buf[64];
      ktk_layout_row(ctx, 2, (int[]) { 54, -1 }, 0);
      ktk_label(ctx,"Position:");
      sprintf(buf, "%d, %d", win->rect.x, win->rect.y); ktk_label(ctx, buf);
      ktk_label(ctx, "Size:");
      sprintf(buf, "%d, %d", win->rect.w, win->rect.h); ktk_label(ctx, buf);
    }

    /* labels + buttons */
    if (ktk_header_ex(ctx, "Test Buttons", KTK_OPT_EXPANDED)) {
      ktk_layout_row(ctx, 3, (int[]) { 86, -110, -1 }, 0);
      ktk_label(ctx, "Test buttons 1:");
      if (ktk_button(ctx, "Button 1")) { write_log("Pressed button 1"); }
      if (ktk_button(ctx, "Button 2")) { write_log("Pressed button 2"); }
      ktk_label(ctx, "Test buttons 2:");
      if (ktk_button(ctx, "Button 3")) { write_log("Pressed button 3"); }
      if (ktk_button(ctx, "Popup")) { ktk_open_popup(ctx, "Test Popup"); }
      if (ktk_begin_popup(ctx, "Test Popup")) {
        ktk_button(ctx, "Hello");
        ktk_button(ctx, "World");
        ktk_end_popup(ctx);
      }
    }

    /* tree */
    if (ktk_header_ex(ctx, "Tree and Text", KTK_OPT_EXPANDED)) {
      ktk_layout_row(ctx, 2, (int[]) { 140, -1 }, 0);
      ktk_layout_begin_column(ctx);
      if (ktk_begin_treenode(ctx, "Test 1")) {
        if (ktk_begin_treenode(ctx, "Test 1a")) {
          ktk_label(ctx, "Hello");
          ktk_label(ctx, "world");
          ktk_end_treenode(ctx);
        }
        if (ktk_begin_treenode(ctx, "Test 1b")) {
          if (ktk_button(ctx, "Button 1")) { write_log("Pressed button 1"); }
          if (ktk_button(ctx, "Button 2")) { write_log("Pressed button 2"); }
          ktk_end_treenode(ctx);
        }
        ktk_end_treenode(ctx);
      }
      if (ktk_begin_treenode(ctx, "Test 2")) {
        ktk_layout_row(ctx, 2, (int[]) { 54, 54 }, 0);
        if (ktk_button(ctx, "Button 3")) { write_log("Pressed button 3"); }
        if (ktk_button(ctx, "Button 4")) { write_log("Pressed button 4"); }
        if (ktk_button(ctx, "Button 5")) { write_log("Pressed button 5"); }
        if (ktk_button(ctx, "Button 6")) { write_log("Pressed button 6"); }
        ktk_end_treenode(ctx);
      }
      if (ktk_begin_treenode(ctx, "Test 3")) {
        static int checks[3] = { 1, 0, 1 };
        ktk_checkbox(ctx, "Checkbox 1", &checks[0]);
        ktk_checkbox(ctx, "Checkbox 2", &checks[1]);
        ktk_checkbox(ctx, "Checkbox 3", &checks[2]);
        ktk_end_treenode(ctx);
      }
      ktk_layout_end_column(ctx);

      ktk_layout_begin_column(ctx);
      ktk_layout_row(ctx, 1, (int[]) { -1 }, 0);
      ktk_text(ctx, "Lorem ipsum dolor sit amet, consectetur adipiscing "
        "elit. Maecenas lacinia, sem eu lacinia molestie, mi risus faucibus "
        "ipsum, eu varius magna felis a nulla.");
      ktk_layout_end_column(ctx);
    }

    /* background color sliders */
    if (ktk_header_ex(ctx, "Background Color", KTK_OPT_EXPANDED)) {
      ktk_layout_row(ctx, 2, (int[]) { -78, -1 }, 74);
      /* sliders */
      ktk_layout_begin_column(ctx);
      ktk_layout_row(ctx, 2, (int[]) { 46, -1 }, 0);
      ktk_label(ctx, "Red:");   ktk_slider(ctx, &bg[0], 0, 255);
      ktk_label(ctx, "Green:"); ktk_slider(ctx, &bg[1], 0, 255);
      ktk_label(ctx, "Blue:");  ktk_slider(ctx, &bg[2], 0, 255);
      ktk_layout_end_column(ctx);
      /* color preview */
      ktk_Rect r = ktk_layout_next(ctx);
      ktk_draw_rect(ctx, r, ktk_color(bg[0], bg[1], bg[2], 255));
      char buf[32];
      sprintf(buf, "#%02X%02X%02X", (int) bg[0], (int) bg[1], (int) bg[2]);
      ktk_draw_control_text(ctx, buf, r, KTK_COLOR_TEXT, KTK_OPT_ALIGNCENTER);
    }

    ktk_end_window(ctx);
  }
}


static void log_window(ktk_Context *ctx) {
  if (ktk_begin_window(ctx, "Log Window", ktk_rect(350, 40, 300, 200))) {
    /* output text panel */
    ktk_layout_row(ctx, 1, (int[]) { -1 }, -25);
    ktk_begin_panel(ctx, "Log Output");
    ktk_Container *panel = ktk_get_current_container(ctx);
    ktk_layout_row(ctx, 1, (int[]) { -1 }, -1);
    ktk_text(ctx, logbuf);
    ktk_end_panel(ctx);
    if (logbuf_updated) {
      panel->scroll.y = panel->content_size.y;
      logbuf_updated = 0;
    }

    /* input textbox + submit button */
    static char buf[128];
    int submitted = 0;
    ktk_layout_row(ctx, 2, (int[]) { -70, -1 }, 0);
    if (ktk_textbox(ctx, buf, sizeof(buf)) & KTK_RES_SUBMIT) {
      ktk_set_focus(ctx, ctx->last_id);
      submitted = 1;
    }
    if (ktk_button(ctx, "Submit")) { submitted = 1; }
    if (submitted) {
      write_log(buf);
      buf[0] = '\0';
    }

    ktk_end_window(ctx);
  }
}


static int uint8_slider(ktk_Context *ctx, unsigned char *value, int low, int high) {
  static float tmp;
  ktk_push_id(ctx, &value, sizeof(value));
  tmp = *value;
  int res = ktk_slider_ex(ctx, &tmp, low, high, 0, "%.0f", KTK_OPT_ALIGNCENTER);
  *value = tmp;
  ktk_pop_id(ctx);
  return res;
}


static void style_window(ktk_Context *ctx) {
  static struct { const char *label; int idx; } colors[] = {
    { "text:",         KTK_COLOR_TEXT        },
    { "border:",       KTK_COLOR_BORDER      },
    { "windowbg:",     KTK_COLOR_WINDOWBG    },
    { "titlebg:",      KTK_COLOR_TITLEBG     },
    { "titletext:",    KTK_COLOR_TITLETEXT   },
    { "panelbg:",      KTK_COLOR_PANELBG     },
    { "button:",       KTK_COLOR_BUTTON      },
    { "buttonhover:",  KTK_COLOR_BUTTONHOVER },
    { "buttonfocus:",  KTK_COLOR_BUTTONFOCUS },
    { "base:",         KTK_COLOR_BASE        },
    { "basehover:",    KTK_COLOR_BASEHOVER   },
    { "basefocus:",    KTK_COLOR_BASEFOCUS   },
    { "scrollbase:",   KTK_COLOR_SCROLLBASE  },
    { "scrollthumb:",  KTK_COLOR_SCROLLTHUMB },
    { NULL }
  };

  if (ktk_begin_window(ctx, "Style Editor", ktk_rect(350, 250, 300, 240))) {
    int sw = ktk_get_current_container(ctx)->body.w * 0.14;
    ktk_layout_row(ctx, 6, (int[]) { 80, sw, sw, sw, sw, -1 }, 0);
    for (int i = 0; colors[i].label; i++) {
      ktk_label(ctx, colors[i].label);
      uint8_slider(ctx, &ctx->style->colors[i].r, 0, 255);
      uint8_slider(ctx, &ctx->style->colors[i].g, 0, 255);
      uint8_slider(ctx, &ctx->style->colors[i].b, 0, 255);
      uint8_slider(ctx, &ctx->style->colors[i].a, 0, 255);
      ktk_draw_rect(ctx, ktk_layout_next(ctx), ctx->style->colors[i]);
    }
    ktk_end_window(ctx);
  }
}


static void process_frame(ktk_Context *ctx) {
  ktk_begin(ctx);
  style_window(ctx);
  log_window(ctx);
  test_window(ctx);
  ktk_end(ctx);
}



static const char button_map[256] = {
  [ SDL_BUTTON_LEFT   & 0xff ] =  KTK_MOUSE_LEFT,
  [ SDL_BUTTON_RIGHT  & 0xff ] =  KTK_MOUSE_RIGHT,
  [ SDL_BUTTON_MIDDLE & 0xff ] =  KTK_MOUSE_MIDDLE,
};

static const char key_map[256] = {
  [ SDLK_LSHIFT       & 0xff ] = KTK_KEY_SHIFT,
  [ SDLK_RSHIFT       & 0xff ] = KTK_KEY_SHIFT,
  [ SDLK_LCTRL        & 0xff ] = KTK_KEY_CTRL,
  [ SDLK_RCTRL        & 0xff ] = KTK_KEY_CTRL,
  [ SDLK_LALT         & 0xff ] = KTK_KEY_ALT,
  [ SDLK_RALT         & 0xff ] = KTK_KEY_ALT,
  [ SDLK_RETURN       & 0xff ] = KTK_KEY_RETURN,
  [ SDLK_BACKSPACE    & 0xff ] = KTK_KEY_BACKSPACE,
};


static int text_width(ktk_Font font, const char *text, int len) {
  if (len == -1) { len = strlen(text); }
  return r_get_text_width(text, len);
}

static int text_height(ktk_Font font) {
  return r_get_text_height();
}


int main(int argc, char **argv) {
  /* init SDL and renderer */
  SDL_Init(SDL_INIT_EVERYTHING);
  r_init();

  /* init microui */
  ktk_Context *ctx = malloc(sizeof(ktk_Context));
  ktk_init(ctx);
  ctx->text_width = text_width;
  ctx->text_height = text_height;

  /* main loop */
  for (;;) {
    /* handle SDL events */
    SDL_Event e;
    while (SDL_PollEvent(&e)) {
      switch (e.type) {
        case SDL_QUIT: exit(EXIT_SUCCESS); break;
        case SDL_MOUSEMOTION: ktk_input_mousemove(ctx, e.motion.x, e.motion.y); break;
        case SDL_MOUSEWHEEL: ktk_input_scroll(ctx, 0, e.wheel.y * -30); break;
        case SDL_TEXTINPUT: ktk_input_text(ctx, e.text.text); break;

        case SDL_MOUSEBUTTONDOWN:
        case SDL_MOUSEBUTTONUP: {
          int b = button_map[e.button.button & 0xff];
          if (b && e.type == SDL_MOUSEBUTTONDOWN) { ktk_input_mousedown(ctx, e.button.x, e.button.y, b); }
          if (b && e.type ==   SDL_MOUSEBUTTONUP) { ktk_input_mouseup(ctx, e.button.x, e.button.y, b);   }
          break;
        }

        case SDL_KEYDOWN:
        case SDL_KEYUP: {
          int c = key_map[e.key.keysym.sym & 0xff];
          if (c && e.type == SDL_KEYDOWN) { ktk_input_keydown(ctx, c); }
          if (c && e.type ==   SDL_KEYUP) { ktk_input_keyup(ctx, c);   }
          break;
        }
      }
    }

    /* process frame */
    process_frame(ctx);

    /* render */
    r_clear(ktk_color(bg[0], bg[1], bg[2], 255));
    ktk_Command *cmd = NULL;
    while (ktk_next_command(ctx, &cmd)) {
      switch (cmd->type) {
        case KTK_COMMAND_TEXT: r_draw_text(cmd->text.str, cmd->text.pos, cmd->text.color); break;
        case KTK_COMMAND_RECT: r_draw_rect(cmd->rect.rect, cmd->rect.color); break;
        case KTK_COMMAND_ICON: r_draw_icon(cmd->icon.id, cmd->icon.rect, cmd->icon.color); break;
        case KTK_COMMAND_CLIP: r_set_clip_rect(cmd->clip.rect); break;
      }
    }
    r_present();
  }

  return 0;
}


