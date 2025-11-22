#pragma once

#include <image_path.h>
#include <string>
#include <vector>

struct Image
{
    std::vector<std::vector<unsigned char>> data;
    int width;
    int height;

    std::vector<unsigned char> operator[](size_t index) const
    {
        return data[index];
    }

    std::vector<unsigned char> &operator[](size_t index)
    {
        return data[index];
    }
};

Image read_image(const std::string &filename);

void write_image(const std::string &filename, const Image &image);

Image create_empty_image(int width, int height);

void image_show(const std::string &title, const Image &image);
