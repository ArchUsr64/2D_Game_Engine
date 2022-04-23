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

void render_entity(Entity *entity) {
  SDL_Rect entity_rect;
  entity_rect.w = map_to_range(entity->collision_box.x, 0, 1, 0, WINDOW_WIDTH);
  entity_rect.h = map_to_range(entity->collision_box.y, 0, 1, 0, WINDOW_HEIGHT);
  entity_rect.x = map_to_range(entity->position.x, -1, 1, 0, WINDOW_WIDTH);
  entity_rect.y = map_to_range(entity->position.y, -1, 1, WINDOW_HEIGHT, 0);
  entity_rect.x -= entity_rect.w / 2;
  entity_rect.y -= entity_rect.h / 2;
  SDL_SetRenderDrawColor(RENDERER_PTR, 255, 255, 255, 255);
  SDL_RenderDrawRect(RENDERER_PTR, &entity_rect);
  SDL_SetRenderDrawColor(RENDERER_PTR, 0, 0, 0, 255);
}

int main() {
  if (!sdl_init())
    return -1;
  pthread_t input_thread_id;
  pthread_create(&input_thread_id, NULL, input_thread, NULL);
  pthread_t physics_thread_id;
  pthread_create(&physics_thread_id, NULL, physics_thread, NULL);
  while (!USER_QUIT) {
    SDL_RenderClear(RENDERER_PTR);
    render_entity(&player);
    render_entity(&enemy);
    set_draw_colour(1, 1, 1, 1);
    draw_circle(100, 150, 90);
    SDL_RenderPresent(RENDERER_PTR);
    set_draw_colour(0, 0, 0, 1);
  }
  pthread_join(input_thread_id, NULL);
  pthread_join(physics_thread_id, NULL);
  return 0;
}
