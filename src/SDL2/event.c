#include <SDL.h>

#include "quasi88.h"
#include "keyboard.h" // KEY88_xxx syms
#include "event.h" // quasi88_key

// globals...
int use_cmdkey = FALSE;
int keyboard_type = 1; // never changed in SDL2
int use_joydevice = FALSE;
int show_fps = FALSE;
char* file_keyboard = NULL; // not used in SDL2 version

// internals...
void numlock_setup(int enabled) {
  // FIXME: whatever switching this does
}

// callbacks...
void event_init() {

}

#define SDL_TO_PC88_KEY(keysdl, key88) if(keysym.sym == keysdl) { return key88; }

int keydown_to_key88(SDL_Keysym keysym) {
  // TODO: use a map for this (or a macro)
  if(keysym.sym >= SDLK_0 && keysym.sym <= SDLK_9) {
    return KEY88_0 + (keysym.sym - SDLK_0); // hack
  }

  SDL_TO_PC88_KEY(SDLK_RETURN, KEY88_RETURNL);
  SDL_TO_PC88_KEY(SDLK_BACKSPACE, KEY88_DEL);
  SDL_TO_PC88_KEY(SDLK_SPACE, KEY88_SPACE);
  SDL_TO_PC88_KEY(SDLK_KP_ENTER, KEY88_RETURNR);

  SDL_TO_PC88_KEY(SDLK_a, KEY88_A);
  SDL_TO_PC88_KEY(SDLK_b, KEY88_B);
  SDL_TO_PC88_KEY(SDLK_c, KEY88_C);
  SDL_TO_PC88_KEY(SDLK_d, KEY88_D);
  SDL_TO_PC88_KEY(SDLK_e, KEY88_E);
  SDL_TO_PC88_KEY(SDLK_f, KEY88_F);
  SDL_TO_PC88_KEY(SDLK_g, KEY88_G);
  SDL_TO_PC88_KEY(SDLK_h, KEY88_H);
  SDL_TO_PC88_KEY(SDLK_i, KEY88_I);
  SDL_TO_PC88_KEY(SDLK_j, KEY88_J);
  SDL_TO_PC88_KEY(SDLK_k, KEY88_K);
  SDL_TO_PC88_KEY(SDLK_l, KEY88_L);
  SDL_TO_PC88_KEY(SDLK_m, KEY88_M);
  SDL_TO_PC88_KEY(SDLK_n, KEY88_N);
  SDL_TO_PC88_KEY(SDLK_o, KEY88_O);
  SDL_TO_PC88_KEY(SDLK_p, KEY88_P);
  SDL_TO_PC88_KEY(SDLK_q, KEY88_Q);
  SDL_TO_PC88_KEY(SDLK_r, KEY88_R);
  SDL_TO_PC88_KEY(SDLK_s, KEY88_S);
  SDL_TO_PC88_KEY(SDLK_t, KEY88_T);
  SDL_TO_PC88_KEY(SDLK_u, KEY88_U);
  SDL_TO_PC88_KEY(SDLK_v, KEY88_V);
  SDL_TO_PC88_KEY(SDLK_w, KEY88_W);
  SDL_TO_PC88_KEY(SDLK_x, KEY88_X);
  SDL_TO_PC88_KEY(SDLK_y, KEY88_Y);
  SDL_TO_PC88_KEY(SDLK_z, KEY88_Z);
  SDL_TO_PC88_KEY(SDLK_COMMA, KEY88_COMMA);
  SDL_TO_PC88_KEY(SDLK_PERIOD, KEY88_PERIOD);
  SDL_TO_PC88_KEY(SDLK_SLASH, KEY88_SLASH);
  SDL_TO_PC88_KEY(SDLK_SEMICOLON, KEY88_SEMICOLON);
  SDL_TO_PC88_KEY(SDLK_QUOTE, KEY88_COLON);
  // TODO: what about ]?

  SDL_TO_PC88_KEY(SDLK_ESCAPE, KEY88_ESC);
  SDL_TO_PC88_KEY(SDLK_BACKQUOTE, KEY88_STOP); // not 100% sure this is a good idea, but my mac doesn't have a pause key

  SDL_TO_PC88_KEY(SDLK_LCTRL, KEY88_CTRL);
  SDL_TO_PC88_KEY(SDLK_CAPSLOCK, KEY88_CAPS);
  SDL_TO_PC88_KEY(SDLK_LSHIFT, KEY88_SHIFT);
  SDL_TO_PC88_KEY(SDLK_RSHIFT, KEY88_SHIFT);
  SDL_TO_PC88_KEY(SDLK_MINUS, KEY88_MINUS);
  SDL_TO_PC88_KEY(SDLK_EQUALS, KEY88_CARET);
  SDL_TO_PC88_KEY(SDLK_BACKSLASH, KEY88_YEN);

  SDL_TO_PC88_KEY(SDLK_TAB, KEY88_TAB);
  SDL_TO_PC88_KEY(SDLK_PAGEUP, KEY88_ROLLDOWN);
  SDL_TO_PC88_KEY(SDLK_PAGEDOWN, KEY88_ROLLUP);

  SDL_TO_PC88_KEY(SDLK_F1, KEY88_F1);
  SDL_TO_PC88_KEY(SDLK_F2, KEY88_F2);
  SDL_TO_PC88_KEY(SDLK_F3, KEY88_F3);
  SDL_TO_PC88_KEY(SDLK_F4, KEY88_F4);
  SDL_TO_PC88_KEY(SDLK_F5, KEY88_F5);
  SDL_TO_PC88_KEY(SDLK_F6, KEY88_F6);
  SDL_TO_PC88_KEY(SDLK_F7, KEY88_F7);
  SDL_TO_PC88_KEY(SDLK_F8, KEY88_F8);
  SDL_TO_PC88_KEY(SDLK_F9, KEY88_F9);
  SDL_TO_PC88_KEY(SDLK_F10, KEY88_F10);
  SDL_TO_PC88_KEY(SDLK_F11, KEY88_F12); // status
  SDL_TO_PC88_KEY(SDLK_F12, KEY88_F12); // menu

  // TODO: think about doing a mode here to switch between cursor
  // and numpad emulation, since almost all games use numpad
  SDL_TO_PC88_KEY(SDLK_UP, KEY88_UP);
  SDL_TO_PC88_KEY(SDLK_DOWN, KEY88_DOWN);
  SDL_TO_PC88_KEY(SDLK_LEFT, KEY88_LEFT);
  SDL_TO_PC88_KEY(SDLK_RIGHT, KEY88_RIGHT);

  SDL_TO_PC88_KEY(SDLK_KP_0, KEY88_KP_0);
  SDL_TO_PC88_KEY(SDLK_KP_1, KEY88_KP_1);
  SDL_TO_PC88_KEY(SDLK_KP_2, KEY88_KP_2);
  SDL_TO_PC88_KEY(SDLK_KP_3, KEY88_KP_3);
  SDL_TO_PC88_KEY(SDLK_KP_4, KEY88_KP_4);
  SDL_TO_PC88_KEY(SDLK_KP_5, KEY88_KP_5);
  SDL_TO_PC88_KEY(SDLK_KP_6, KEY88_KP_6);
  SDL_TO_PC88_KEY(SDLK_KP_7, KEY88_KP_7);
  SDL_TO_PC88_KEY(SDLK_KP_8, KEY88_KP_8);
  SDL_TO_PC88_KEY(SDLK_KP_9, KEY88_KP_9);

  printf("Unknown key %s\n", SDL_GetKeyName(keysym.sym));
  return KEY88_Z;
}

void event_update() {
  SDL_Event event;
  SDL_PollEvent(&event);

  int key88 = 0;

  switch(event.type) {
    case SDL_QUIT:
      exit(0); // FIXME make this actually work
      break;
    case SDL_KEYDOWN:
    case SDL_KEYUP:
      key88 = keydown_to_key88(event.key.keysym);
      quasi88_key(key88, (event.type == SDL_KEYDOWN));
      break;
  }
}

void event_exit() {
  // FIXME: joystick_exit
}

void event_numlock_off() {
  numlock_setup(FALSE);
}

int event_numlock_on() {
  numlock_setup(TRUE);
  return TRUE;
}

int event_get_joystick_num() {
  return 0; // FIXME
}

void event_switch() {
  // FIXME: toggle unicode mode, i guess
}

void event_get_mouse_pos(int* x, int* y) {
  int win_x, win_y;
  SDL_PumpEvents();
  SDL_GetMouseState(&win_x, &win_y);

  *x = win_x; *y = win_y;
}
