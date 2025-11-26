#pragma once

#include <image.h>
#include <vector>

namespace primitive {

class HarrisDetector
{
    float k_;
    float threshold_;
    int sobel_size_;
    float gaussian_sigma_;

public:
    struct Corner
    {
        int x;
        int y;
        int response;

        Corner(int x, int y, int response) : x(x), y(y), response(response) {}

        bool operator<(const Corner &other) const
        {
            return response > other.response;
        }
    };

    HarrisDetector() : k_(0.04f), threshold_(0.01f), sobel_size_(3), gaussian_sigma_(1.5f) {}

    void setK(float k)
    {
        k_ = k;
    }
    void setThreshold(float threshold)
    {
        threshold_ = threshold;
    }
    void setSobelSize(int size)
    {
        sobel_size_ = size;
    }
    void setGaussianSigma(float sigma)
    {
        gaussian_sigma_ = sigma;
    }

    cv::Mat detect(const Image &image);

private:
    void computeSobelGradient(const Image &image, Image &dx, Image &dy);
    std::vector<Corner> nonMaximumSuppression(const Image&image, int window_size = 3);
};

}// namespace primitive
