#pragma once

#include <assert.h>
#include <vector>

template<typename T>
class Array2D
{
protected:
    std::vector<T> data_;
    int row_;
    int column_;

public:
    Array2D() : row_(0), column_(0) {}

    Array2D(int row, int column) : row_(row), column_(column)
    {
        data_.resize(row * column);
    }

    Array2D(const std::vector<std::vector<T>> &data)
    {
        row_ = data.size();
        column_ = data[0].size();
        data_.resize(row_ * column_);
        for (int i = 0; i < row_; ++i)
        {
            for (int j = 0; j < column_; ++j)
            {
                data_[j * row_ + i] = data[i][j];
            }
        }
    }

    void resize(int row, int column)
    {
        row_ = row;
        column_ = column;
        data_.resize(row * column);
    }

    T &operator()(int row, int col)
    {
        assert(row >= 0 && row < row_);
        assert(col >= 0 && col < column_);
        return data_[col * row_ + row];
    }

    const T &operator()(int row, int col) const
    {
        assert(row >= 0 && row < row_);
        assert(col >= 0 && col < column_);
        return data_[col * row_ + row];
    }
    int row() const
    {
        return row_;
    }

    void setRow(int row, const std::vector<T> &data)
    {
        assert(row >= 0 && row < row_);
        assert(data.size() == column_);
        for (int i = 0; i < column_; ++i)
        {
            data_[i * row_ + row] = data[i];
        }
    }

    int column() const
    {
        return column_;
    }

    void setColumn(int column, const std::vector<T> &data)
    {
        assert(column >= 0 && column < column_);
        assert(data.size() == row_);
        for (int i = 0; i < row_; i++)
        {
            data_[column * row_ + i] = data[i];
        }
    }
};