#pragma once

#include "vector.h"

vec2 vec2_new(float x, float y) {
  vec2 return_vec = {.x = x, .y = y};
  return return_vec;
}
vec2 vec2_add(vec2 *vec_1, vec2 *vec_2) {
  return vec2_new(vec_1->x + vec_2->x, vec_1->y + vec_2->y);
};
vec2 vec2_sub(vec2 *vec_1, vec2 *vec_2) {
  return vec2_new(vec_1->x - vec_2->x, vec_1->y - vec_2->y);
};
vec2 vec2_def(vec2 *vec_1, vec2 *vec_2) {
  return vec2_new(vec_1->x - vec_2->x, vec_1->y - vec_2->y);
};
vec2 vec2_scl(vec2 *vec, float scl) {
  return vec2_new(vec->x * scl, vec->y * scl);
}
vec2 vec2_unt(vec2 *vec) { return vec2_scl(vec, 1 / vec2_len(vec)); };
float vec2_dot(vec2 *vec_1, vec2 *vec_2) {
  return ((vec_1->x * vec_2->x) + (vec_1->y * vec_2->y));
};
float vec2_len(vec2 *vec) {
  return (float)sqrt(pow(vec->y, 2) + pow(vec->x, 2));
};
