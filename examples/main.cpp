#include <histogram_equalization.h>
#include <image_inversion.h>

int main()
{
    Image img = read_image(IMAGE_DATA_PATH("citywall.png"));

    auto im = image_space_algorithm::histogram_equalization(img);

    write_image(IMAGE_OUTPUT_PATH("citywall_he.png"), im);

    return 0;
}