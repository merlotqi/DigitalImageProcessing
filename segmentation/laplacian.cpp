#include "prewitt.h"


namespace segmentation {

int laplacian_kernel(int r, int c, const Image &image, int k)
{
    if (k == 8)
    {
        int k_edge = image(r - 1, c - 1) + image(r - 1, c + 1) + image(r + 1, c - 1) + image(r + 1, c + 1) +
                     image(r, c - 1) + image(r, c + 1) + image(r - 1, c) + image(r + 1, c);
        int k_center = 8 * image(r, c);
        return k_center - k_edge;
    }
    else if (k == 20)
    {
        int k_edge = 2 * image(r - 1, c - 1) + 3 * image(r - 1, c) + 2 * image(r - 1, c + 1) + 3 * image(r, c - 1) +
                     3 * image(r, c + 1) + 2 * image(r + 1, c - 1) + 3 * image(r + 1, c) + 2 * image(r + 1, c + 1);
        int k_center = 20 * image(r, c);
        return k_center - k_edge;
    }
    else
    {
        // k = 4;
        return image(r - 1, c - 1) + image(r - 1, c + 1) + image(r + 1, c - 1) + image(r + 1, c + 1) - 4 * image(r, c);
    }
}

Image laplacian(const Image &image, int K)
{
    Image out(image.row(), image.column());

    for (int i = 1; i < image.row() - 1; i++)
    {
        for (int j = 1; j < image.column() - 1; j++)
        {
            int gradient = laplacian_kernel(i, j, image, K);
            out(i, j) = std::min(255, std::max(0, gradient));
        }
    }
    return out;
}

}// namespace segmentation