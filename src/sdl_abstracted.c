#pragma once

#include "sdl_abstracted.h"

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
