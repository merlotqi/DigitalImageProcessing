#pragma once

#include <vector>

#include <array2D.h>

using GaussianKernel2D = Array2D<double>;

namespace Gaussian {

std::vector<double> guassianKernel1D(double sigma, int size = -1);

GaussianKernel2D guassianKernel2D(double sigma, int size = -1);

GaussianKernel2D gaussianLoGKernel2D(double sigma, int size = -1);

}// namespace Gaussian
