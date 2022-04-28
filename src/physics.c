#pragma once

#include <SDL2/SDL.h>

#include "global_vars.h"
#include "physics.h"
#include "util.h"

void wall_colission(Entity *entity) {
  if (entity->position.x > (1 - entity->collision_box.x / 2))
    entity->velocity.x = -1 * (abs(entity->velocity.x));
  else if (entity->position.x < -(1 - entity->collision_box.x / 2))
    entity->velocity.x = abs(entity->velocity.x);
  if (entity->position.y > (1 - entity->collision_box.y / 2))
    entity->velocity.y = -1 * (abs(entity->velocity.y));
  else if (entity->position.y < -(1 - entity->collision_box.y / 2))
    entity->velocity.y = abs(entity->velocity.y);
}

void apply_friction(Entity *entity) {
  entity->velocity = vec2_scl(&entity->velocity, FRICTION);
}

void apply_net_force(Entity *entity, vec2 *net_force) {
  entity->acceleration = *net_force;
}

void update_kinematics(Entity *entity) {
  vec2 acceleration_scaled =
      vec2_scl(&entity->acceleration, update_interval_in_ms);
  vec2 velocity_scaled = vec2_scl(&entity->velocity, update_interval_in_ms);
  entity->velocity = vec2_add(&entity->velocity, &acceleration_scaled);
  entity->position = vec2_add(&entity->position, &velocity_scaled);
}

bool entity_to_entity_colission_detection(Entity *entity_1, Entity *entity_2) {
  float dist_x = abs(entity_1->position.x - entity_2->position.x);
  float dist_y = abs(entity_1->position.y - entity_2->position.y);
  float width_sum =
      abs(entity_1->collision_box.x / 2 + entity_2->collision_box.x / 2);
  float height_sum =
      abs(entity_1->collision_box.y / 2 + entity_2->collision_box.y / 2);
  if (dist_x < width_sum && dist_y < height_sum) {
    return true;
  }
  return false;
}

void handle_entity_mouse_input(Entity *entity) {
  vec2 MOUSE_VECTOR = vec2_new(MOUSE_X, MOUSE_Y);
  if ((1 - (abs(MOUSE_VECTOR.x))) >= entity->collision_box.x / 2)
    entity->position.x = MOUSE_VECTOR.x;
  if ((1 - (abs(MOUSE_VECTOR.y))) >= entity->collision_box.y / 2)
    entity->position.y = MOUSE_VECTOR.y;
}

void handle_entity_keyboard_input(Entity *entity) {
  vec2 INPUT_VECTOR =
      vec2_new(((int)KEY_RIGHT - (int)KEY_LEFT), ((int)KEY_UP - (int)KEY_DOWN));
  switch (entity->keyboard_control_type) {
  case FORCE:
    INPUT_VECTOR = vec2_scl(&INPUT_VECTOR, 1 / entity->mass);
    wall_colission(entity);
    apply_net_force(entity, &INPUT_VECTOR);
    apply_friction(entity);
    break;
  case MOMENTUM:
    entity->velocity = vec2_scl(&INPUT_VECTOR, 1 / entity->mass);
    break;
  }
  update_kinematics(entity);
}

void *physics_thread(void *args) {
  int update_interval_in_ms = 1000 / PHYSICS_UPDATE_FREQUENCY;
  while (!USER_QUIT) {
    int start_time = SDL_GetTicks();
    switch (player.movement_control_type) {
    case MOUSE:
      handle_entity_mouse_input(&player);
      break;
    case KEYBOARD:
      handle_entity_keyboard_input(&player);
      break;
    }
    while (SDL_GetTicks() < start_time + update_interval_in_ms)
      SDL_Delay(1);
  }
  return NULL;
}
