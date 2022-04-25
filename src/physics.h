#pragma once

#include "vector.h"
#include <stdbool.h>
#include <SDL2/SDL.h>

typedef enum movement_control_type {KEYBOARD, MOUSE,} Movement_control_type;

typedef struct entity {
  vec2 position;
  vec2 velocity;
  vec2 acceleration;
  Movement_control_type movement_control_type;
  float mass;
  vec2 collision_box;
  bool render_collision_box;
  char* texture_path;
  SDL_Texture* texture_ptr;
} Entity;

typedef enum direction {
  POS_X,
  POS_Y,
  NEG_X,
  NEG_Y,
  POS_X_POS_Y,
  NEG_X_POS_Y,
  NEG_X_NEG_Y,
  POS_X_NEG_Y,
} Direction;
