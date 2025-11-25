#include "sobel.h"


namespace segmentation {

Image sobel(const Image &image)
{
    Image out(image.row(), image.column());

    for (int i = 1; i < image.row() - 1; i++)
    {
        for (int j = 1; j < image.column() - 1; j++)
        {
            int gx = -image(i - 1, j - 1) - 2 * image(i - 1, j) - image(i - 1, j + 1) + image(i + 1, j - 1) +
                     2 * image(i + 1, j) + image(i + 1, j + 1);
            int gy = -image(i - 1, j - 1) - 2 * image(i, j - 1) - image(i + 1, j - 1) + image(i - 1, j + 1) +
                     2 * image(i, j + 1) + image(i + 1, j + 1);

            int gradient = std::abs(gx) + std::abs(gy);
            out(i, j) = std::min(255, std::max(0, gradient));
        }
    }
    return out;
}

}// namespace segmentation