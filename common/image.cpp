#include "image.h"
#include <opencv2/opencv.hpp>

Image read_image(const std::string &filename)
{
    cv::Mat img = cv::imread(filename, cv::IMREAD_GRAYSCALE);
    Image image;
    if (img.empty())
    {
        image.width = 0;
        image.height = 0;
        return image;
    }
    image.width = img.cols;
    image.height = img.rows;
    image.data.resize(image.height, std::vector<unsigned char>(image.width));
    for (int i = 0; i < image.height; ++i)
    {
        for (int j = 0; j < image.width; ++j)
        {
            image.data[i][j] = img.at<unsigned char>(i, j);
        }
    }
    return image;
}

void write_image(const std::string &filename, const Image &image)
{
    cv::Mat img(image.height, image.width, CV_8UC1);
    for (int i = 0; i < image.height; ++i)
    {
        for (int j = 0; j < image.width; ++j)
        {
            img.at<unsigned char>(i, j) = image.data[i][j];
        }
    }
    cv::imwrite(filename, img);
}

Image create_empty_image(int width, int height)
{
    Image img;
    img.width = width;
    img.height = height;
    img.data.resize(height, std::vector<unsigned char>(width, 0));
    return img;
}

void image_show(const std::string &title, const Image &image)
{
    cv::Mat img(image.height, image.width, CV_8UC1);
    for (int i = 0; i < image.height; ++i)
    {
        for (int j = 0; j < image.width; ++j)
        {
            img.at<unsigned char>(i, j) = image.data[i][j];
        }
    }
    cv::imshow(title, img);
    cv::waitKey(0);
}