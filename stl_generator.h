#ifndef LITHOPHANEMAKERV2_STL_GENERATOR_H
#define LITHOPHANEMAKERV2_STL_GENERATOR_H

#include <stdio.h>
#include <stdint.h>
#include "bitmap_processing.h"

//! Wrapper for a 3-dimensional vertex
struct vertex{
    double x, y, z;
};

//! Wrapper for output file parameters
struct stl_file_parameters{
    char input_file_path[100], output_file_path[100];
    double lithophane_width, lithophane_height, thickness, lithophane_depth, z_ratio, base_z;
    int32_t block_size;
};

struct vertex generate_normal(struct vertex v1, struct vertex v2, struct vertex v3);
void add_facet(FILE *output, struct vertex v1, struct vertex v2, struct vertex v3);
void create_rectangle(FILE *output, struct vertex upper_left, struct vertex upper_right, struct vertex lower_left, struct vertex lower_right);
void create_stl_file(struct stl_file_parameters params);
int32_t max_dimension(int32_t dimension, int32_t block_size);
#endif //LITHOPHANEMAKERV2_STL_GENERATOR_H
