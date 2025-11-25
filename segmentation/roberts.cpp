#include "roberts.h"


namespace segmentation {

Image reberts(const Image &image)
{
    Image out(image.row(), image.column());

    for (int i = 0; i < image.row()-1; i++)
    {
        for (int j = 0; j < image.column()-1; j++)
        {
            int gx = image(i, j) - image(i+1, j+1);
            int gy = image(i+1, j) - image(i, j+1);

            int gradient = std::abs(gx) + std::abs(gy);

            out(i, j) = std::min(255, std::max(0, gradient));
        }
    }
    return out;
}

}// namespace segmentation