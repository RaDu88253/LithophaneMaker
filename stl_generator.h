#ifndef LITHOPHANEMAKERV2_STL_GENERATOR_H
#define LITHOPHANEMAKERV2_STL_GENERATOR_H

#include <stdio.h>
#include <stdint.h>

struct vertex{
    double x, y, z;
};

struct vertex generate_normal(struct vertex v1, struct vertex v2, struct vertex v3);
void add_facet(FILE *output, struct vertex v1, struct vertex v2, struct vertex v3);
void create_rectangle(FILE *output, struct vertex v11, struct vertex v12, struct vertex v21, struct vertex v22);
int32_t max_dimension(int32_t dimension, int32_t block_size);
#endif //LITHOPHANEMAKERV2_STL_GENERATOR_H
