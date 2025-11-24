#include "image.h"
#include <opencv2/opencv.hpp>

Image read_image(const std::string &filename)
{
    cv::Mat img = cv::imread(filename, cv::IMREAD_GRAYSCALE);
    Image image(img.cols, img.rows);
    for (int i = 0; i < image.row(); ++i)
    {
        for (int j = 0; j < image.column(); ++j)
        {
            image(i, j) = img.at<unsigned char>(i, j);
        }
    }
    return image;
}

void write_image(const std::string &filename, const Image &image)
{
    cv::Mat img(image.row(), image.column(), CV_8UC1);
    for (int i = 0; i < image.row(); ++i)
    {
        for (int j = 0; j < image.column(); ++j)
        {
            img.at<unsigned char>(i, j) = image(i, j);
        }
    }
    cv::imwrite(filename, img);
}

void image_show(const std::string &title, const Image &image)
{
    cv::Mat img(image.row(), image.column(), CV_8UC1);
    for (int i = 0; i < image.row(); ++i)
    {
        for (int j = 0; j < image.column(); ++j)
        {
            img.at<unsigned char>(i, j) = image(i, j);
        }
    }
    cv::imshow(title, img);
    cv::waitKey(0);
}
