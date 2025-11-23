#pragma once

#include <assert.h>
#include <image_path.h>
#include <string>
#include <vector>

class Image
{
    std::vector<unsigned char> data_;
    int width_;
    int height_;

public:
    Image() : width_(0), height_(0) {}

    Image(int width, int height) : width_(width), height_(height)
    {
        data_.resize(width * height);
    }

    void resize(int width, int height)
    {
        width_ = width;
        height_ = height;
        data_.resize(width * height);
    }

    unsigned char &operator()(int row, int col)
    {
        assert(row >= 0 && row < width_);
        assert(col >= 0 && col < height_);
        return data_[col * width_ + row];
    }

    const unsigned char &operator()(int row, int col) const
    {
        assert(row >= 0 && row < width_);
        assert(col >= 0 && col < height_);
        return data_[col * width_ + row];
    }
    int width() const
    {
        return width_;
    }
    int height() const
    {
        return height_;
    }
};

Image read_image(const std::string &filename);

void write_image(const std::string &filename, const Image &image);

void image_show(const std::string &title, const Image &image);
