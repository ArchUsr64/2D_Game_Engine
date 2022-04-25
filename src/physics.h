#pragma once

#include "vector.h"
#include <stdbool.h>
#include <SDL2/SDL.h>

typedef enum keyboard_control_type {FORCE, MOMENTUM,} Keyboard_control_type;
typedef enum movement_control_type {KEYBOARD, MOUSE,} Movement_control_type;

typedef struct entity {
  vec2 position;
  vec2 velocity;
  vec2 acceleration;
  Movement_control_type movement_control_type;
  Keyboard_control_type keyboard_control_type;
  vec2 collision_box;
  float mass;
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
