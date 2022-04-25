#pragma once

#include <SDL2/SDL.h>
#include <stdbool.h>

#include "global_vars.h"
#include "util.h"

bool sdl_init();
void get_draw_colour();
void set_draw_colour(float r, float g, float b, float a);
void draw_rect(float x, float y, float w, float h);
void draw_point(float x, float y);
void draw_line(float x1, float y1, float x2, float y2);
void render_entity(Entity* entity);
void sdl_quit();
