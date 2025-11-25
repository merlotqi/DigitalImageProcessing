#include "prewitt.h"
#include <gaussian.h>
#include "laplacian.h"

namespace segmentation {

Image marrHildreth(const Image &image, int K, double sigma, int radius)
{
    Image  smoothed = filters::gaussianFilter(image, sigma, radius);

    return laplacian(smoothed, K);
}

}// namespace segmentation