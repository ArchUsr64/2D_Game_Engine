#pragma once

#define abs(x) ((x) > 0) ? (x) : -(x)

float map_to_range(float num_to_map, float range_input_min,
                   float range_input_max, float range_output_min,
                   float range_output_max);
