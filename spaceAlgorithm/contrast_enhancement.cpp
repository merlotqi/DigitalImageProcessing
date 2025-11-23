#include "contrast_enhancement.h"
#include <algorithm>

namespace image_space_algorithm {

Image contrast_enhancement(const Image &input_image, double f1, double f2, double g1, double g2)
{
    auto value = [&](double v) {
        double r = 0.0;
        if (v <= f1)
            r = g1 / f1;
        else if (v > f1 && v <= f2)
            r = (g2 - g1) / (f2 - f1);
        else
            r = (255.0 - g2) / (255.0 - f2);
        return static_cast<unsigned char>(std::clamp(r * v, 0.0, 255.0));
    };

    Image output(input_image.width(), input_image.height());
    for (int i = 0; i < input_image.height(); ++i)
    {
        for (int j = 0; j < input_image.width(); ++j)
        {
            output(i, j) = value(input_image(i, j));
        }
    }
    return output;
}

}// namespace image_space_algorithm
