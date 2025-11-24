#pragma once

#include <assert.h>
#include <image_path.h>
#include <string>
#include <vector>

class Image
{
    std::vector<unsigned char> data_;
    int row_;
    int column_;

public:
    Image() : row_(0), column_(0) {}

    Image(int row, int column) : row_(row), column_(column)
    {
        data_.resize(row * column);
    }

    void resize(int row, int column)
    {
        row_ = row;
        column_ = column;
        data_.resize(row * column);
    }

    unsigned char &operator()(int row, int col)
    {
        assert(row >= 0 && row < row_);
        assert(col >= 0 && col < column_);
        return data_[col * row_ + row];
    }

    const unsigned char &operator()(int row, int col) const
    {
        assert(row >= 0 && row < row_);
        assert(col >= 0 && col < column_);
        return data_[col * row_ + row];
    }
    int row() const
    {
        return row_;
    }
    int column() const
    {
        return column_;
    }
};

Image read_image(const std::string &filename);

void write_image(const std::string &filename, const Image &image);

void image_show(const std::string &title, const Image &image);
