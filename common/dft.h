#pragma once

#include "image.h"
#include <complex>

using Complex = std::complex<double>;
using ComplexMatrix1d = std::vector<Complex>;

class ComplexMatrix2d
{
    std::vector<Complex> data_;
    int row_;
    int column_;

public:
    ComplexMatrix2d() : row_(0), column_(0) {}

    ComplexMatrix2d(int row, int column) : row_(row), column_(column)
    {
        data_.resize(row * column);
    }

    ComplexMatrix2d(const std::vector<ComplexMatrix1d> &mat)
    {
        row_ = static_cast<int>(mat.size());
        column_ = static_cast<int>(mat[0].size());
        data_.resize(row_ * column_);
        for (int i = 0; i < row_; i++)
        {
            for (int j = 0; j < column_; j++)
            {
                data_[j * row_ + i] = mat[i][j];
            }
        }
    }

    void resize(int row, int column)
    {
        row_ = row;
        column_ = column;
        data_.resize(row * column);
    }

    Complex &operator()(int row, int col)
    {
        assert(row >= 0 && row < row_);
        assert(col >= 0 && col < column_);
        return data_[col * row_ + row];
    }

    const Complex &operator()(int row, int col) const
    {
        assert(row >= 0 && row < row_);
        assert(col >= 0 && col < column_);
        return data_[col * row_ + row];
    }
    int row() const
    {
        return row_;
    }


    void row(int row, const ComplexMatrix1d &mat)
    {
        assert(row >= 0 && row < row_);
        assert(mat.size() == column_);
        for (int i = 0; i < column_; i++)
        {
            data_[i * row_ + row] = mat[i];
        }
    }

    int column() const
    {
        return column_;
    }

    void column(int column, const ComplexMatrix1d &mat)
    {
        assert(column >= 0 && column < column_);
        assert(mat.size() == row_);
        for (int i = 0; i < row_; i++)
        {
            data_[column * row_ + i] = mat[i];
        }
    }
};


namespace DFT {

ComplexMatrix2d compute_dft(const Image &image);

Image compute_idft(const ComplexMatrix2d &mat);

ComplexMatrix2d compute_dftshift(const Image &image);

Image compute_idftshift(const ComplexMatrix2d &mat);

Image compute_magnitude_spectrum(const ComplexMatrix2d &spectrum);

Image compute_phase_spectrum(const ComplexMatrix2d &spectrum);

}// namespace DFT
