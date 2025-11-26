#pragma once

#include <assert.h>
#include <image_path.h>
#include <string>
#include <vector>

#include <opencv2/opencv.hpp>

#include "array2D.h"

using Image = Array2D<unsigned char>;

Image read_image(const std::string &filename);

void write_image(const std::string &filename, const Image &image);

cv::Mat to_cv_mat(const Image &image);

void image_show(const std::string &title, const Image &image);
