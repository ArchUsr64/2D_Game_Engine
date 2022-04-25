#pragma once

#include "vector.h"

typedef struct entity {
  vec2 position;
  vec2 velocity;
  vec2 acceleration;
  float mass;
  vec2 collision_box;
  char* texture_path;
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
