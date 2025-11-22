#include <dft.h>
#include <histogram_equalization.h>
#include <image_inversion.h>

int main()
{
    Image img = read_image(IMAGE_DATA_PATH("citywall.png"));

    auto dft = DFT::compute_dftshift(img);

    auto im = DFT::compute_magnitude_spectrum(dft);

    write_image(IMAGE_OUTPUT_PATH("citywall_he.png"), im);

    return 0;
}