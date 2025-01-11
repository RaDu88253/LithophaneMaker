#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "stl_generator.h"

struct vertex generate_normal(struct vertex v1, struct vertex v2, struct vertex v3){

    double x = (v2.y - v1.y) * (v3.z - v1.z) - (v2.z - v1.z) * (v3.y - v1.y);
    double y = (v2.x - v1.x) * (v3.z - v1.z) - (v2.z - v1.z) * (v3.x - v1.x);
    double z = (v2.x - v1.x) * (v3.y - v1.y) - (v2.y - v1.y) * (v3.x - v1.x);

    struct vertex res;
    res.x = x, res.y = y, res.z = z;

    return res;

};


void add_facet(FILE *output, struct vertex v1, struct vertex v2, struct vertex v3){

    struct vertex normal = generate_normal(v1, v2, v3);

    fprintf(output, "\tfacet normal %e %e %e\n", normal.x, normal.y, normal.z);
    fprintf(output, "\t\touter loop\n");
    fprintf(output, "\t\t\tvertex %e %e %e\n", v1.x, v1.y, v1.z);
    fprintf(output, "\t\t\tvertex %e %e %e\n", v2.x, v2.y, v2.z);
    fprintf(output, "\t\t\tvertex %e %e %e\n", v3.x, v3.y, v3.z);
    fprintf(output, "\t\tendloop\n");
    fprintf(output, "\tendfacet\n");

}

void create_rectangle(FILE *output, struct vertex upper_left, struct vertex upper_right, struct vertex lower_left, struct vertex lower_right){

    struct vertex middle;
    middle.x = (upper_left.x + upper_right.x + lower_left.x + lower_right.x) / 4;
    middle.y = (upper_left.y + upper_right.y + lower_left.y + lower_right.y) / 4;
    middle.z = (upper_left.z + upper_right.z + lower_left.z + lower_right.z) / 4;

    add_facet(output, upper_left, upper_right, middle);
    add_facet(output, upper_right, lower_right, middle);
    add_facet(output, lower_right, lower_left, middle);
    add_facet(output, lower_left, upper_left, middle);

}

