#include <SDL.h>

#include "quasi88.h"

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

void event_update() {
  SDL_Event event;
  SDL_PollEvent(&event);

  switch(event.type) {
    case SDL_QUIT:
      exit(0); // FIXME make this actually work
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
