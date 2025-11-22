#include "histogram_equalization.h"

namespace image_space_algorithm {

static std::vector<int> compute_histogram(const Image &image)
{
    std::vector<int> histogram(256, 0);
    for (int i = 0; i < image.height; ++i)
    {
        for (int j = 0; j < image.width; ++j)
        {
            ++histogram[image[i][j]];
        }
    }

    return histogram;
}

static std::vector<double> compute_cdf(const std::vector<int> &histogram, int total_pixels)
{
    std::vector<double> cdf(256, 0.0);
    cdf[0] = static_cast<double>(histogram[0]) / (double) total_pixels;
    for (int i = 1; i < 256; ++i)
    {
        cdf[i] = cdf[i - 1] + static_cast<double>(histogram[i]) / (double) total_pixels;
    }

    return cdf;
}
static Image equalization(const Image &image, const std::vector<double> &cdfs)
{
    Image output_image = create_empty_image(image.width, image.height);

    // Find the minimum non-zero CDF value
    double cdf_min = 0.0;
    for (double cdf: cdfs)
    {
        if (cdf > 0.0)
        {
            cdf_min = cdf;
            break;
        }
    }

    for (int i = 0; i < image.height; ++i)
    {
        for (int j = 0; j < image.width; ++j)
        {
            double equalizedValue = (cdfs[static_cast<int>(image[i][j])] - cdf_min) / (1.0 - cdf_min) * 255.0;
            output_image[i][j] = static_cast<unsigned char>(std::clamp(equalizedValue, 0.0, 255.0));
        }
    }
    return output_image;
}

Image histogram_equalization(const Image &input_image)
{
    // Compute histogram
    std::vector<int> histogram = compute_histogram(input_image);
    // Compute CDF
    std::vector<double> cdf = compute_cdf(histogram, input_image.width * input_image.height);
    // Equalize the image
    Image output_image = equalization(input_image, cdf);
    return output_image;
}

}// namespace image_space_algorithm
