#include "image_inversion.h"

namespace image_inversion {

Image invert_image(const Image &img)
{
    static const unsigned char MAX_PIXEL_VALUE = 255;
    Image inverted_img = create_empty_image(img.width, img.height);
    for (int i = 0; i < img.height; ++i)
    {
        for (int j = 0; j < img.width; ++j)
        {
            inverted_img.data[i][j] = MAX_PIXEL_VALUE - img.data[i][j];
        }
    }
    return inverted_img;
}

}// namespace image_inversion
