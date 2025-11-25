#include <dft.h>
#include <histogram_equalization.h>
#include <image_inversion.h>
#include <roberts.h>
#include <prewitt.h>
#include <sobel.h>
#include <laplacian.h>
#include <marr_hildreth.h>

int main()
{
    Image img = read_image(IMAGE_DATA_PATH("/lena.png"));

    write_image(IMAGE_OUTPUT_PATH("reberts.png"), segmentation::reberts(img));
    write_image(IMAGE_OUTPUT_PATH("prewitt.png"), segmentation::prewitt(img));
    write_image(IMAGE_OUTPUT_PATH("sobel.png"), segmentation::sobel(img));
    write_image(IMAGE_OUTPUT_PATH("laplacian_4.png"), segmentation::laplacian(img, 4));
    write_image(IMAGE_OUTPUT_PATH("laplacian_8.png"), segmentation::laplacian(img, 8));
    write_image(IMAGE_OUTPUT_PATH("laplacian_20.png"), segmentation::laplacian(img, 20));
    write_image(IMAGE_OUTPUT_PATH("marrHildreth_20_2_3.png"), segmentation::marrHildreth(img, 20, 2.0, 3));

    return 0;
}
