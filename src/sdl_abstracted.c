#pragma once

#include "sdl_abstracted.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

bool sdl_init() {
  if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
    printf("Failed to initialise SDL: %s", SDL_GetError());
    pl;
    return false;
  }
  WINDOW_PTR =
      SDL_CreateWindow("Test", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                       WINDOW_WIDTH, WINDOW_HEIGHT, 0);
  if (!WINDOW_PTR) {
    printf("Failed to create window: %s", SDL_GetError());
    pl;
    return false;
  }
  RENDERER_PTR = SDL_CreateRenderer(
      WINDOW_PTR, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
  if (!RENDERER_PTR) {
    printf("Failed to create renderer: %s", SDL_GetError());
    pl;
    return false;
  }
  return true;
}

void set_draw_colour(float red, float green, float blue, float alpha) {
  SDL_SetRenderDrawColor(RENDERER_PTR, map_to_range(red, 0, 1, 0, 255),
                         map_to_range(green, 0, 1, 0, 255),
                         map_to_range(blue, 0, 1, 0, 255),
                         map_to_range(alpha, 0, 1, 0, 255));
}

void draw_rect(float pos_x, float pos_y, float width, float height) {
  SDL_Rect rect = {.x = pos_x, .y = pos_y, .w = width, .h = height};
  SDL_RenderDrawRect(RENDERER_PTR, &rect);
}

void draw_point(float point_x, float point_y) {
  SDL_RenderDrawPoint(RENDERER_PTR, point_x, point_y);
}

void draw_line(float point_1_x, float point_1_y, float point_2_x,
               float point_2_y) {
  SDL_RenderDrawLine(RENDERER_PTR, point_1_x, point_1_y, point_2_x, point_2_y);
}

void render_entity(Entity *entity) {
  SDL_Rect entity_box;
  entity_box.w = map_to_range(entity->collision_box.x, 0, 1, 0, WINDOW_WIDTH);
  entity_box.h = map_to_range(entity->collision_box.y, 0, 1, 0, WINDOW_HEIGHT);
  entity_box.x = map_to_range(entity->position.x, -1, 1, 0, WINDOW_WIDTH);
  entity_box.y = map_to_range(entity->position.y, -1, 1, WINDOW_HEIGHT, 0);
  entity_box.x -= entity_box.w / 2;
  entity_box.y -= entity_box.h / 2;
  if (!entity->texture_ptr) {
    SDL_Surface *image_surface_ptr = SDL_LoadBMP(entity->texture_path);
    if (!image_surface_ptr) {
      printf("Failed to load '%s': %s", entity->texture_path, SDL_GetError());
      pl;
      RUNTIME_ERROR = true;
    }
    entity->texture_ptr =
        SDL_CreateTextureFromSurface(RENDERER_PTR, image_surface_ptr);
    SDL_FreeSurface(image_surface_ptr);
    if (!entity->texture_ptr) {
      printf("Failed to created texture: %s", SDL_GetError());
      pl;
      RUNTIME_ERROR = true;
    }
  }
  if (entity->render_collision_box)
    SDL_RenderDrawRect(RENDERER_PTR, &entity_box);
  SDL_RenderCopy(RENDERER_PTR, entity->texture_ptr, NULL, &entity_box);
}

void sdl_quit() {
  SDL_DestroyRenderer(RENDERER_PTR);
  SDL_DestroyWindow(WINDOW_PTR);
  SDL_Quit();
}
