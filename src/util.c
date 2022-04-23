#pragma once

#include "util.h"

float map_to_range(float num_to_map, float input_range_min,
                   float input_range_max, float output_range_min,
                   float output_range_max) {
  num_to_map -= input_range_min;
  num_to_map /= input_range_max - input_range_min;
  num_to_map *= output_range_max - output_range_min;
  num_to_map += output_range_min;
  return num_to_map;
}
