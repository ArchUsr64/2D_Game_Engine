#pragma once

#include <SDL2/SDL.h>

#include "global_vars.h"
#include "util.h"

void *input_thread(void *var) {
  int update_interval_in_ms = 1000 / INPUT_UPDATE_FREQUENCY;
  SDL_Event event;
  while (!USER_QUIT) {
    int start_time = SDL_GetTicks();
    SDL_PumpEvents();
    Uint32 mouse_buttons = SDL_GetMouseState(&MOUSE_X_PX, &MOUSE_Y_PX);
    MOUSE_X = map_to_range(MOUSE_X_PX, 0, WINDOW_WIDTH, -1, 1);
    MOUSE_Y = map_to_range(MOUSE_Y_PX, 0, WINDOW_HEIGHT, 1, -1);
    MOUSE_1 = ((mouse_buttons & SDL_BUTTON_LMASK) != 0) ? true : false;
    MOUSE_2 = ((mouse_buttons & SDL_BUTTON_RMASK) != 0) ? true : false;
    while (SDL_PollEvent(&event)) {
      switch (event.key.type) {
      case SDL_QUIT:
        USER_QUIT = true;
        break;
      case SDL_KEYDOWN:
        switch (event.key.keysym.scancode) {
        case SDL_SCANCODE_W:
        case SDL_SCANCODE_UP:
          KEY_UP = true;
          break;
        case SDL_SCANCODE_A:
        case SDL_SCANCODE_LEFT:
          KEY_LEFT = true;
          break;
        case SDL_SCANCODE_S:
        case SDL_SCANCODE_DOWN:
          KEY_DOWN = true;
          break;
        case SDL_SCANCODE_D:
        case SDL_SCANCODE_RIGHT:
          KEY_RIGHT = true;
          break;
        default:
          break;
        }
        break;
      case SDL_KEYUP:
        switch (event.key.keysym.scancode) {
        case SDL_SCANCODE_W:
        case SDL_SCANCODE_UP:
          KEY_UP = false;
          break;
        case SDL_SCANCODE_A:
        case SDL_SCANCODE_LEFT:
          KEY_LEFT = false;
          break;
        case SDL_SCANCODE_S:
        case SDL_SCANCODE_DOWN:
          KEY_DOWN = false;
          break;
        case SDL_SCANCODE_D:
        case SDL_SCANCODE_RIGHT:
          KEY_RIGHT = false;
          break;
        default:
          break;
        }
        break;
      }
    }
    while (SDL_GetTicks() < start_time + update_interval_in_ms)
      SDL_Delay(1);
  }
  return NULL;
}
