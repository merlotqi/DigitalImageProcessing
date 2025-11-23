#include <dft.h>
#include <histogram_equalization.h>
#include <image_inversion.h>

int main()
{
    Image img = read_image("../images/lena.png");

    auto dft = DFT::compute_dftshift(img);

    auto im = DFT::compute_magnitude_spectrum(dft);

    write_image(IMAGE_OUTPUT_PATH("citywall_he.png"), im);

    return 0;
}