#pragma once

#include <vector>

struct Image {
    std::vector<std::vector<unsigned char>> data;
    int width;
    int height;
};

Image read_image(const char* filename);

void write_image(const char* filename, const Image& image);

Image create_empty_image(int width, int height);