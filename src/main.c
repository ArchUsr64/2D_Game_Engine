#include <SDL2/SDL.h>
#include <SDL2/SDL_error.h>
#include <SDL2/SDL_touch.h>
#include <pthread.h>
#include <stdio.h>

#include "global_vars.h"
#include "input.c"
#include "physics.c"
#include "sdl_abstracted.c"
#include "util.c"
#include "vector.c"

int main() {
  if (!sdl_init())
    return -1;
  pthread_t input_thread_id;
  pthread_create(&input_thread_id, NULL, input_thread, NULL);
  pthread_t physics_thread_id;
  pthread_create(&physics_thread_id, NULL, physics_thread, NULL);
  while (!USER_QUIT && !RUNTIME_ERROR) {
    set_draw_colour(0, 0, 0, 1);
    SDL_RenderClear(RENDERER_PTR);
    render_entity(&player);
    set_draw_colour(1, 1, 1, 1);
    SDL_RenderPresent(RENDERER_PTR);
  }
  sdl_quit();
  pthread_join(input_thread_id, NULL);
  pthread_join(physics_thread_id, NULL);
  return (RUNTIME_ERROR)? -1: 0;
}
