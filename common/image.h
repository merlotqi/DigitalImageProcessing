#pragma once

#include <assert.h>
#include <image_path.h>
#include <string>
#include <vector>

#include "array2D.h"

using Image = Array2D<unsigned char>;

Image read_image(const std::string &filename);

void write_image(const std::string &filename, const Image &image);

void image_show(const std::string &title, const Image &image);
