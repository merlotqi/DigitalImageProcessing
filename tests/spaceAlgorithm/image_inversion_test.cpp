#include <gtest/gtest.h>

#include <image_inversion.h>

TEST(image_inversion, InvertImage)
{
    Image img = read_image("tests/spaceAlgorithm/test_data/input_image.png");
    Image inverted_img = image_space_algorithm::image_inversion(img);
}