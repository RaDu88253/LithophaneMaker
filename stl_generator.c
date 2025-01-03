#include <stdio.h>
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

void create_rectangle(FILE *output, struct vertex v11, struct vertex v12, struct vertex v21, struct vertex v22){
    struct vertex aux;
    aux.x = (v11.x + v12.x + v21.x + v22.x) / 4;
    aux.y = (v11.y + v12.y + v21.y + v22.y) / 4;
    aux.z = (v11.z + v12.z + v21.z + v22.z) / 4;

    add_facet(output, v11, v12, aux);
    add_facet(output, v12, v22, aux);
    add_facet(output, v22, v21, aux);
    add_facet(output, v21, v11, aux);

}

