#include <SDL.h>

#include "quasi88.h"
#include "keyboard.h" // KEY88_xxx syms

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

int keydown_to_key88(SDL_Keysym keysym) {
  // TODO: use a map for this
  if(keysym.sym >= SDLK_0 && keysym.sym <= SDLK_9) {
    return KEY88_0 + (keysym.sym - SDLK_0); // hack
  }
  if(keysym.sym == SDLK_b) { return KEY88_B; }
  if(keysym.sym == SDLK_e) { return KEY88_E; }
  if(keysym.sym == SDLK_p) { return KEY88_P; } // UGH
  if(keysym.sym == SDLK_RETURN) {
    return KEY88_RETURNL;
  }
  if(keysym.sym == SDLK_SPACE) {
    return KEY88_SPACE;
  }
  if(keysym.sym == SDLK_BACKSPACE) {
    return KEY88_DEL;
  }

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

void event_numlock_on() {
  numlock_setup(TRUE);
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
