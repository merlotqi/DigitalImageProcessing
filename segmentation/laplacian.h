#include <image.h>

namespace segmentation {

/// @brief  Apply Laplacian filter to an image
/// @param image Input image
/// @param K Kernel size 4 or 8 or 20
/// @return 
Image laplacian(const Image &image, int K);

}// namespace segmentation
