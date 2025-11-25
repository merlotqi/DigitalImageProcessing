#pragma once

#include <vector>

#include <array2D.h>
#include <image.h>

using GaussianKernel2D = Array2D<double>;

namespace filters {

std::vector<double> guassianKernel1D(double sigma, int radius = -1);

GaussianKernel2D guassianKernel2D(double sigma, int radius = -1);

GaussianKernel2D gaussianLoGKernel2D(double sigma, int radius = -1);

Image gaussianFilter(const Image& image, double sigma, int radius = -1);

}// namespace filters
