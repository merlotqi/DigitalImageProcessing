#include <gtest/gtest.h>

#include <image_inversion.h>

TEST(ImageInversionTest, InvertImage) {
    Image img = read_image("tests/spaceAlgorithm/test_data/input_image.png");
    Image inverted_img = image_inversion::invert_image(img);

}