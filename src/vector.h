#pragma once

#include <math.h>

typedef struct vec2 {
  float x;
  float y;
} vec2;

vec2 vec2_new(float x, float y);
vec2 vec2_add(vec2 *vec_1, vec2 *vec_2);
vec2 vec2_sub(vec2 *vec_1, vec2 *vec_2);
vec2 vec2_scl(vec2 *vec_1, float scl);
vec2 vec2_unt(vec2 *vec);
float vec2_dot(vec2 *vec_1, vec2 *vec_2);
float vec2_len(vec2 *vec);
