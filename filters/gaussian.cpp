#include "gaussian.h"
#include <cmath>

namespace filters {

std::vector<double> guassianKernel1D(double sigma, int radius)
{
    if (radius <= 0)
    {
        radius = static_cast<int>(std::ceil(3.8 * sigma));
    }
    const int size = 2 * radius + 1;
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

GaussianKernel2D guassianKernel2D(double sigma, int radius)
{
    if (radius <= 0)
    {
        radius = static_cast<int>(std::ceil(3.8 * sigma));
    }
    const int size = 2 * radius + 1;

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

GaussianKernel2D gaussianLoGKernel2D(double sigma, int radius)
{
    if (radius <= 0)
    {
        radius = static_cast<int>(std::ceil(3.0 * sigma));
    }

    const int size = 2 * radius + 1;
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

Image gaussianFilter(const Image &image, double sigma, int radius)
{
    if (radius <= 0)
    {
        radius = static_cast<int>(std::ceil(3.0 * sigma));
    }
    GaussianKernel2D kernel = gaussianLoGKernel2D(sigma, radius);
    Image out(image.row(), image.column());

    for (int i = 0; i < image.row(); i++)
    {
        for (int j = 0; j < image.column(); j++)
        {
            double sum = 0.0;
            double weight_sum = 0.0;

            for (int m = -radius; m <= radius; m++)
            {
                for (int n = -radius; n <= radius; n++)
                {
                    int row_idx = i + m;
                    int col_idx = j + n;

                    if (row_idx < 0)
                        row_idx = 0;
                    if (row_idx >= image.row())
                        row_idx = image.row() - 1;
                    if (col_idx < 0)
                        col_idx = 0;
                    if (col_idx >= image.column())
                        col_idx = image.column() - 1;

                    double weight = kernel(m + radius, n + radius);
                    sum += image(row_idx, col_idx) * weight;
                    weight_sum += weight;
                }
            }

            if (weight_sum > 0)
            {
                sum /= weight_sum;
            }
            out(i, j) = static_cast<unsigned int>(std::min(255.0, std::max(0.0, sum)));
        }
    }

    return out;
}

}// namespace filters