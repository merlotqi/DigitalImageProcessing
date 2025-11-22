#pragma once

#include <image.h>

namespace image_space_algorithm {

Image contrast_enhancement(const Image &input_image, double f1, double f2, double g1, double g2);

}// namespace image_space_algorithm