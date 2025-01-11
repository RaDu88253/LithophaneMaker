#include "bitmap_processing.h"
#include <math.h>


uint32_t read_little_endian_unsigned_int_from_keyboard(uint8_t size_in_bytes){
    uint32_t res = 0;
    for (int i = 0; i < size_in_bytes; i++){
        uint32_t aux;
        scanf("%x", &aux);
        res += aux * (uint32_t)(pow(256, i));
    }
    return res;
}
uint32_t read_little_endian_unsigned_int(FILE *input, uint8_t size_in_bytes){
    uint32_t res = 0;
    for (int i = 0; i < size_in_bytes; i++){
        uint32_t aux = fgetc(input);
        res += aux * (uint32_t)(pow(256, i));
    }
    return res;
}
 void read_bitmap_header(FILE *input, struct bmp_file_info *fisier){
    int byte1, byte2;
    byte1 = fgetc(input);
    byte2 = fgetc(input);
    if (byte1 == 'B' && byte2 == 'M')
        fisier -> is_bitmap = 1;
    else
        fisier -> is_bitmap = 0;
     read_redundant_bytes(input, 12);
}
void read_redundant_bytes(FILE *input, uint32_t number_of_bytes){
    for(int i = 0; i < number_of_bytes; i++)
        fgetc(input);
}
void read_DIB_header(FILE *input, struct bmp_file_info *fisier){
    int32_t header_size = (int) read_little_endian_unsigned_int(input, 4);
    fisier -> DIB_header_size = header_size;
    header_size -= 4;
    int32_t width = (int) read_little_endian_unsigned_int(input, 4);
    fisier -> width = width;
    header_size -= 4;
    int32_t height = (int) read_little_endian_unsigned_int(input, 4);
    fisier -> height = height;
    header_size -= 4;
    uint32_t number_of_color_planes = read_little_endian_unsigned_int(input, 2); //must be 1
    header_size -= 2;
    uint32_t number_of_bits_per_pixel = read_little_endian_unsigned_int(input, 2);
    fisier -> bits_per_pixel = number_of_bits_per_pixel;
    header_size -= 2;
    uint32_t compression_method = read_little_endian_unsigned_int(input, 4); // only works for 0
    header_size -= 4;
    uint32_t image_size = read_little_endian_unsigned_int(input, 4);
    header_size -= 4;
    int32_t horizontal_resolution = (int) read_little_endian_unsigned_int(input, 4); // in pixels per metre
    header_size -= 4;
    int32_t vertical_resolution = (int) read_little_endian_unsigned_int(input, 4); // in pixels per metre
    header_size -= 4;
    uint32_t number_of_colors_in_the_palette = read_little_endian_unsigned_int(input, 4);
    fisier -> number_of_colors_in_the_color_palette = number_of_colors_in_the_palette;
    header_size -= 4;
    uint32_t number_of_important_colors = read_little_endian_unsigned_int(input, 4);
    header_size -= 4;
    if (header_size < 0){
        printf("Eroare! Header prea mic.");
        return;
    }
    else if(header_size > 0){
        read_redundant_bytes(input, header_size);
        header_size = 0;
    }
}
void calculate_row_size(struct bmp_file_info *fisier){
    fisier -> row_size = ((fisier -> bits_per_pixel) * (fisier -> width) + 31) / 32 * 4;
}
void read_pixel_array(FILE *input, struct bmp_file_info *fisier, uint8_t *grayscale_image){
    int32_t rows_to_read = fisier -> height;
    int32_t pixels_per_row = fisier -> width;
    int32_t row_size = (int) (fisier -> row_size);
    for(int i = 0; i < rows_to_read; i++){
        for(int j = 0; j < pixels_per_row; j++){
            uint8_t blue = (unsigned char) fgetc(input);
            uint8_t green = (unsigned char) fgetc(input);
            uint8_t red = (unsigned char) fgetc(input);
            grayscale_image[(rows_to_read - 1 - i) * pixels_per_row + j] = (unsigned char) (0.2989 * red + 0.5870 * green + 0.1140 * blue);
        }
        if ((fisier -> bits_per_pixel) * pixels_per_row / 8 < row_size){
            uint32_t aux = row_size - ((fisier -> bits_per_pixel) * pixels_per_row / 8);
            read_redundant_bytes(input, aux);
        }
    }

}
