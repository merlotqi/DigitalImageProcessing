#include "image_inversion.h"

namespace image_space_algorithm {

Image image_inversion(const Image &img)
{
    static const unsigned char MAX_PIXEL_VALUE = 255;
    Image inverted_img(img.row(), img.column());
    for (int i = 0; i < img.row(); ++i)
    {
        for (int j = 0; j < img.column(); ++j)
        {
            inverted_img(i, j) = MAX_PIXEL_VALUE - img(i, j);
        }
    }
    return inverted_img;
}

}// namespace image_space_algorithm
