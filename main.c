#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "stl_generator.h"
#include "bitmap_processing.h"

#define STL_FILE_START "solid lithophane\n"
#define STL_FILE_END "endsolid lithophane"
#define MAX_COLOR_VALUE 255

int main(void) {

    struct stl_file_parameters params;

    strcpy(params.input_file_path, "..\\TajMahal.bmp");
    strcpy(params.output_file_path, "..\\TajMahal.stl");

    params.block_size = 1;

    params.lithophane_width = 160; //mm
    params.thickness = 2.3; //mm
    params.lithophane_depth =1.8; //m
    params.z_ratio = params.lithophane_depth / MAX_COLOR_VALUE;
    params.base_z = params.lithophane_depth - params.thickness;

    create_stl_file(params);

    return 0;
}

void create_stl_file(struct stl_file_parameters params){

    FILE *input, *output;
    input = fopen(params.input_file_path, "rb");
    output = fopen(params.output_file_path, "w");
    struct bmp_file_info fisier;
    if (input == NULL) {
        printf("FISIERUL NU EXISTA");
        return;
    }
    read_bitmap_header(input, &fisier);
    read_DIB_header(input, &fisier);
    calculate_row_size(&fisier);

    uint8_t *grayscale_image = malloc(fisier.width * fisier.height * sizeof(uint8_t));

    read_pixel_array(input, &fisier, grayscale_image);

    params.lithophane_height = (double) params.lithophane_width * fisier.height / fisier.width;


    fprintf(output, STL_FILE_START);
    struct vertex border_upper_left, border_upper_right, border_lower_left, border_lower_right;
    for(int32_t i = params.block_size; i < fisier.height; i += params.block_size){
        for(int32_t j = params.block_size; j < fisier.width; j += params.block_size){
            struct vertex upper_left, upper_right, lower_left, lower_right;

            upper_left.x = (double) ((j - params.block_size) * (params.lithophane_width / fisier.width));
            upper_left.y = (double) ((fisier.height - i + params.block_size) * (params.lithophane_height / fisier.height));
            upper_left.z = MAX_COLOR_VALUE;
            upper_left.z -= (double) ((grayscale_image[(i - params.block_size) * fisier.width + j - params.block_size]));
            upper_left.z *= params.z_ratio;

            upper_right.x = (double) ((j) * (params.lithophane_width / fisier.width));
            upper_right.y = (double) ((fisier.height - i + params.block_size) * (params.lithophane_height / fisier.height));
            upper_right.z = MAX_COLOR_VALUE;
            upper_right.z -= (double) ((grayscale_image[(i - params.block_size) * fisier.width + j]));
            upper_right.z *= params.z_ratio;

            lower_left.x = (double) ((j - params.block_size) * (params.lithophane_width / fisier.width));
            lower_left.y = (double) ((fisier.height - i) * (params.lithophane_height / fisier.height));
            lower_left.z = MAX_COLOR_VALUE;
            lower_left.z -= (double) ((grayscale_image[(i) * fisier.width + j - params.block_size]));
            lower_left.z *= params.z_ratio;

            lower_right.x = (double) ((j) * (params.lithophane_width / fisier.width));
            lower_right.y = (double) ((fisier.height - i) * (params.lithophane_height / fisier.height));
            lower_right.z = MAX_COLOR_VALUE;
            lower_right.z -= (double) ((grayscale_image[(i) * fisier.width + j]));
            lower_right.z *= params.z_ratio;

            create_rectangle(output, upper_left, upper_right, lower_left, lower_right);

            if(i == params.block_size){
                struct vertex upper_left_copy, upper_right_copy, lower_left_copy, lower_right_copy;

                upper_left_copy = upper_left;
                lower_left_copy = upper_left_copy;
                lower_left_copy.z = params.base_z;


                upper_right_copy = upper_right;
                lower_right_copy = upper_right_copy;
                lower_right_copy.z = params.base_z;

                create_rectangle(output, upper_left_copy, upper_right_copy, lower_left_copy, lower_right_copy);
                if (j == params.block_size)
                    border_upper_left = lower_left_copy;
                if (j == params.block_size * ((fisier.width - 1) / params.block_size))
                    border_upper_right = lower_right_copy;
            }
            if(j == params.block_size){

                struct vertex upper_left_copy, upper_right_copy, lower_left_copy, lower_right_copy;

                upper_left_copy = upper_left;
                lower_left_copy = lower_left;

                lower_right_copy = lower_left_copy;
                lower_right_copy.z = params.base_z;

                upper_right_copy = upper_left_copy;
                upper_right_copy.z = params.base_z;

                create_rectangle(output, upper_left_copy, upper_right_copy, lower_left_copy, lower_right_copy);
            }

            if(i == params.block_size * ((fisier.height - 1) / params.block_size)){

                struct vertex upper_left_copy, upper_right_copy, lower_left_copy, lower_right_copy;

                lower_left_copy = lower_left;
                upper_left_copy = lower_left_copy;
                upper_left_copy.z = params.base_z;

                lower_right_copy = lower_right;
                upper_right_copy = lower_right_copy;
                upper_right_copy.z = params.base_z;

                create_rectangle(output, upper_left_copy, upper_right_copy, lower_left_copy, lower_right_copy);

                if(j == params.block_size)
                    border_lower_left = upper_left_copy;
                if(j == params.block_size * ((fisier.width - 1) / params.block_size))
                    border_lower_right = upper_right_copy;
            }
            if(j == params.block_size * ((fisier.width - 1) / params.block_size)){

                struct vertex upper_left_copy, upper_right_copy, lower_left_copy, lower_right_copy;

                upper_right_copy = upper_right;
                lower_right_copy = lower_right;

                upper_left_copy = upper_right_copy;
                upper_left_copy.z = params.base_z;

                lower_left_copy = lower_right_copy;
                lower_left_copy.z = params.base_z;

                create_rectangle(output, upper_left_copy, upper_right_copy, lower_left_copy, lower_right_copy);
            }

        }
    }
    create_rectangle(output, border_upper_left, border_upper_right, border_lower_left, border_lower_right);
    fprintf(output, STL_FILE_END);

    fclose(input);
    fclose(output);
}

