#include <stdio.h>
#include <stdlib.h>
#include "stl_generator.h"
#include "bitmap_processing.h"

int main(void) {

    FILE *output;
    char output_file_path[250] = "..\\output.stl";
    output = fopen(output_file_path, "w");

    FILE *input;
    char input_file_path[250] = "..\\mona-lisa-gioconda-1.bmp";
    input = fopen(input_file_path, "rb");
    struct file_info fisier;
    if (input == NULL) {
        printf("FISIERUL NU EXISTA");
        return 0;
    }
    read_bitmap_header(input, &fisier);
    read_DIB_header(input, &fisier);
    calculate_row_size(&fisier);

    uint8_t *grayscale_image = malloc(fisier.width * fisier.height * sizeof(uint8_t));

    read_pixel_array(input, &fisier, grayscale_image);

    int32_t block_size = 2;

    double lithophane_width = 60; //mm
    double lithophane_height = 1.00 * lithophane_width * fisier.height / fisier.width;
    double thickness = 2.4; //mm
    double lithophane_depth = 1.5; //m
    double z_ratio = lithophane_depth / 255;
    double base_z = lithophane_depth - thickness;

    fprintf(output, "solid lithophane\n");
    struct vertex upper_left, upper_right, lower_left, lower_right;
    for(int32_t i = block_size; i < fisier.height; i += block_size){
        for(int32_t j = block_size; j < fisier.width; j += block_size){
            struct vertex v11, v12, v21, v22;

            v11.x = (double) ((j - block_size) * (lithophane_width / fisier.width));
            v11.y = (double) ((fisier.height - i + block_size) * (lithophane_height / fisier.height));
            v11.z = 255;
            v11.z -= (double) ((grayscale_image[(i - block_size) * fisier.width + j - block_size]));
            v11.z *= z_ratio;

            v12.x = (double) ((j) * (lithophane_width / fisier.width));
            v12.y = (double) ((fisier.height - i + block_size) * (lithophane_height / fisier.height));
            v12.z = 255;
            v12.z -= (double) ((grayscale_image[(i - block_size) * fisier.width + j]));
            v12.z *= z_ratio;

            v21.x = (double) ((j - block_size) * (lithophane_width / fisier.width));
            v21.y = (double) ((fisier.height - i) * (lithophane_height / fisier.height));
            v21.z = 255;
            v21.z -= (double) ((grayscale_image[(i) * fisier.width + j - block_size]));
            v21.z *= z_ratio;

            v22.x = (double) ((j) * (lithophane_width / fisier.width));
            v22.y = (double) ((fisier.height - i) * (lithophane_height / fisier.height));
            v22.z = 255;
            v22.z -= (double) ((grayscale_image[(i) * fisier.width + j]));
            v22.z *= z_ratio;

            create_rectangle(output, v11, v12, v21, v22);

            if(i == block_size){
                struct vertex v11c, v12c, v21c, v22c;

                v11c = v11;
                v21c = v11c;
                v21c.z = base_z;


                v12c = v12;
                v22c = v12c;
                v22c.z = base_z;

                create_rectangle(output, v11c, v12c, v21c, v22c);
                if (j == block_size)
                    upper_left = v21c;
                if (j == block_size * ((fisier.width - 1) / block_size))
                    upper_right = v22c;
            }
            if(j == block_size){

                struct vertex v11c, v12c, v21c, v22c;

                v11c = v11;
                v21c = v21;

                v22c = v21c;
                v22c.z = base_z;

                v12c = v11c;
                v12c.z = base_z;

                create_rectangle(output, v11c, v12c, v21c, v22c);
            }

            if(i == block_size * ((fisier.height - 1) / block_size)){

                struct vertex v11c, v12c, v21c, v22c;

                v21c = v21;
                v11c = v21c;
                v11c.z = base_z;

                v22c = v22;
                v12c = v22c;
                v12c.z = base_z;

                create_rectangle(output, v11c, v12c, v21c, v22c);

                if(j == block_size)
                    lower_left = v11c;
                if(j == block_size * ((fisier.width - 1) / block_size))
                    lower_right = v12c;
            }
            if(j == block_size * ((fisier.width - 1) / block_size)){
                struct vertex v11c, v12c, v21c, v22c;

                v12c = v12;
                v22c = v22;

                v11c = v12c;
                v11c.z = base_z;

                v21c = v22c;
                v21c.z = base_z;

                create_rectangle(output, v11c, v12c, v21c, v22c);
            }

        }
    }
    create_rectangle(output, upper_left, upper_right, lower_left, lower_right);
    fprintf(output, "endsolid lithophane");

    fclose(input);
    fclose(output);
    return 0;
}
