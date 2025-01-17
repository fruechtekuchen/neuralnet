#include "encode_png.hpp"

#include "lodepng.h"
#include <iostream>


void write_greyscale_to_png(const char* filename, const unsigned char *image, unsigned width, unsigned height) {
    unsigned error = lodepng::encode(filename, image, width, height, LCT_GREY);

    if(error){
        std::cout << "encoder error " << error << ": "<< lodepng_error_text(error) << std::endl;
        exit(EXIT_FAILURE);
    }
}

