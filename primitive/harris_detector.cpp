#include "harris_detector.h"

#include <gaussian.h>

namespace primitive {


cv::Mat HarrisDetector::detect(const Image &image)
{
    Image dx, dy;
    computeSobelGradient(image, dx, dy);

    const int row = image.row();
    const int column = image.column();

    Image Ixx(row, column);
    Image Iyy(row, column);
    Image Ixy(row, column);

    for (int i = 0; i < row; i++)
    {
        for (int j = 0; j < column; j++)
        {
            int ix = dx(i, j);
            int iy = dy(i, j);

            Ixx(i, j) = (ix * ix) / 256;
            Iyy(i, j) = (iy * iy) / 256;
            Ixy(i, j) = (ix * iy) / 256;
        }
    }

    Image Ixx_gauss, Iyy_gauss, Ixy_gauss;
    Ixx_gauss = filters::gaussianFilter(Ixx, gaussian_sigma_);
    Iyy_gauss = filters::gaussianFilter(Iyy, gaussian_sigma_);
    Ixy_gauss = filters::gaussianFilter(Ixy, gaussian_sigma_);

    Image R(row, column);
    int k_int = static_cast<int>(k_ * 10000);

    for (int i = 0; i < row; i++)
    {
        for (int j = 0; j < column; j++)
        {
            int A = Ixx_gauss(i, j);
            int B = Iyy_gauss(i, j);
            int C = Ixy_gauss(i, j);

            int det = A * B - C * C;
            int trace = A + B;

            int trace_sq = (trace * trace) / 256;
            int k_trace_sq = (k_int * trace_sq) / 10000;

            int r = det - k_trace_sq;

            R(i, j) = std::clamp(r, 0, 255);
        }
    }

    auto corners = nonMaximumSuppression(R);

    cv::Mat corner_image = to_cv_mat(image);
    cv::Mat colorImage;
    cv::cvtColor(corner_image, colorImage, cv::COLOR_GRAY2BGR);
    for (const auto &corner: corners)
    {
        cv::circle(colorImage, cv::Point(corner.x, corner.y), 5, cv::Scalar(0, 0, 255), -1);
    }

    return colorImage;
}

void HarrisDetector::computeSobelGradient(const Image &image, Image &dx, Image &dy)
{
    const int row = image.row();
    const int column = image.column();

    dx.resize(row, column);
    dy.resize(row, column);

    for (int i = 1; i < row - 1; ++i)
    {
        for (int j = 1; j < column - 1; ++j)
        {

            dx(i, j) = image(i - 1, j + 1) + 2 * image(i, j + 1) + image(i + 1, j + 1) -
                       (image(i - 1, j - 1) + 2 * image(i, j - 1) + image(i + 1, j - 1));

            dy(i, j) = image(i + 1, j - 1) + 2 * image(i + 1, j) + image(i + 1, j + 1) -
                       (image(i - 1, j - 1) + 2 * image(i - 1, j) + image(i - 1, j + 1));

            dx(i, j) = std::clamp(dx(i, j) / 4, 0, 255);
            dy(i, j) = std::clamp(dy(i, j) / 4, 0, 255);
        }
    }
}

std::vector<HarrisDetector::Corner> HarrisDetector::nonMaximumSuppression(const Image &response, int window_size)
{
    std::vector<Corner> corners;
    const int pad = window_size / 2;
    const int row = response.row();
    const int column = response.column();

    for (int y = pad; y < row - pad; y++)
    {
        for (int x = pad; x < column - pad; x++)
        {
            int cur = response(y, x);

            if (cur < threshold_)
                continue;

            bool is_max = true;
            for (int wy = -pad; wy <= pad && is_max; wy++)
            {
                for (int wx = -pad; wx <= pad && is_max; wx++)
                {
                    if (wx == 0 && wy == 0)
                        continue;

                    int neighbor_x = x + wx;
                    int neighbor_y = y + wy;

                    if (response(neighbor_y, neighbor_x) >= cur)
                    {
                        is_max = false;
                    }
                }
            }

            if (is_max)
            {
                corners.push_back({x, y, cur});
            }
        }
    }
    std::sort(corners.begin(), corners.end());
    return corners;
}

}// namespace primitive