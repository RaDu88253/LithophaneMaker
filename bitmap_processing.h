#ifndef LITHOPHANEMAKERV2_BITMAP_PROCESSING_H
#define LITHOPHANEMAKERV2_BITMAP_PROCESSING_H

#include <stdint.h>
#include <stdio.h>

struct file_info{
    uint8_t is_bitmap;
    uint32_t starting_address; //the offset where the pixel array starts
    uint32_t DIB_header_size;
    int32_t width; //pixels
    int32_t height; //pixels
    uint32_t bits_per_pixel;
    uint32_t number_of_colors_in_the_color_palette;
    uint32_t row_size;

};
uint32_t read_little_endian_unsigned_int(FILE *input, uint8_t size_in_bytes);
uint32_t read_little_endian_unsigned_int_from_keyboard(uint8_t size_in_bytes);
void calculate_row_size(struct file_info *fisier);
void read_pixel_array(FILE *input, struct file_info *fisier, uint8_t *grayscale_image);
void read_bitmap_header(FILE *input, struct file_info *fisier); //reads the 14 byte header
void read_DIB_header(FILE *input, struct file_info *fisier); //reads the dib header and sets parameters inside the passed file info object
void read_redundant_bytes(FILE *input, uint32_t number_of_bytes); //consumes n bytes from the input file
#endif //LITHOPHANEMAKERV2_BITMAP_PROCESSING_H
