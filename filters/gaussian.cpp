#include "gaussian.h"
#include <cmath>

namespace Gaussian {

std::vector<double> guassianKernel1D(double sigma, int size)
{
    if (size <= 0)
    {
        size = static_cast<int>(std::ceil(3.8 * sigma));
    }
    size = 2 * size + 1;
    std::vector<double> kernel(size);
    double sum = 0.0;

    for (int i = -size; i <= size; ++i)
    {
        double x = static_cast<double>(i);
        double weight = std::exp(-x * x / (2.0 * sigma * sigma));
        kernel[i + size] = weight;
        sum += weight;
    }

    for (int i = 0; i < size; ++i)
    {
        kernel[i] /= sum;
    }
    return kernel;
}

GaussianKernel2D guassianKernel2D(double sigma, int size)
{
    if (size <= 0)
    {
        size = static_cast<int>(std::ceil(3.8 * sigma));
    }
    size = 2 * size + 1;

    GaussianKernel2D kernel(size, size);
    double sum = 0.0;

    for (int i = 0; i < size; i++)
    {
        for (int j = 0; j < size; j++)
        {
            double x = static_cast<double>(i - size);
            double y = static_cast<double>(j - size);

            double weight = std::exp(-1.0 * (x * x + y * y) / (2.0 * sigma * sigma));
            kernel(i, j) = weight;
            sum += weight;
        }
    }

    for (int i = 0; i < size; i++)
    {
        for (int j = 0; j < size; j++)
        {
            kernel(i, j) /= sum;
        }
    }

    return kernel;
}

GaussianKernel2D gaussianLoGKernel2D(double sigma, int size)
{
    if (size <= 0)
    {
        size = static_cast<int>(std::ceil(3.0 * sigma));
    }

    size = 2 * size + 1;
    GaussianKernel2D kernel(size, size);
    const double sigma2 = sigma * sigma;
    const double sigma4 = sigma2 * sigma2;

    for (int i = 0; i < size; ++i)
    {
        for (int j = 0; j < size; j++)
        {
            double x = static_cast<double>(i - size);
            double y = static_cast<double>(j - size);

            double r2 = x * x + y * y;
            double value = (r2 - 2.0 * sigma2) / sigma4 * std::exp(-1.0 * r2 / (2.0 * sigma2));
            kernel(i, j) = value;
        }
    }
    return kernel;
}

}// namespace Gaussian