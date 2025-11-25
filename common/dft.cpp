#include "dft.h"
#include <algorithm>
#include <math.h>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

namespace DFT {

static ComplexMatrix1d dft1D(const ComplexMatrix1d &input)
{
    const size_t N = input.size();
    ComplexMatrix1d output(N);

    for (size_t n = 0; n < N; ++n)
    {
        Complex sum(0.0, 0.0);
        for (size_t k = 0; k < N; ++k)
        {
            double angle = -2.0 * M_PI * k * n / N;
            Complex twiddle(std::cos(angle), std::sin(angle));
            sum += input[k] * twiddle;
        }
        output[n] = sum;
    }
    return output;
}

static ComplexMatrix1d idft1D(const ComplexMatrix1d &input)
{
    const size_t N = input.size();
    ComplexMatrix1d output(N);
    for (size_t n = 0; n < N; ++n)
    {
        Complex sum(0.0, 0.0);
        for (size_t k = 0; k < N; ++k)
        {
            double angle = 2.0 * M_PI * k * n / N;
            Complex twiddle(std::cos(angle), std::sin(angle));
            sum += input[k] * twiddle;
        }
        output[n] = sum / static_cast<double>(N);
    }
    return output;
}

static ComplexMatrix2d dft2D(const ComplexMatrix2d &input)
{
    int M = input.row(), N = input.column();
    ComplexMatrix2d temp(M, N), output(M, N);

    for (int i = 0; i < M; ++i)
    {
        ComplexMatrix1d row(N);
        for (int j = 0; j < N; ++j) row[j] = input(i, j);
        temp.setRow(i, dft1D(row));
    }

    for (int j = 0; j < N; ++j)
    {
        ComplexMatrix1d col(M);
        for (int i = 0; i < M; ++i) col[i] = temp(i, j);
        output.setColumn(j, dft1D(col));
    }

    return output;
}

static ComplexMatrix2d idft2D(const ComplexMatrix2d &input)
{
    const int M = input.row();
    const int N = input.column();

    ComplexMatrix2d temp(M, N);
    ComplexMatrix2d output(M, N);

    for (int i = 0; i < M; ++i)
    {
        ComplexMatrix1d row_vec(N);
        for (int j = 0; j < N; ++j)
        {
            row_vec[j] = input(i, j);
        }
        temp.setRow(i, idft1D(row_vec));
    }

    for (int j = 0; j < N; ++j)
    {
        ComplexMatrix1d col_vec(M);
        for (int i = 0; i < M; ++i)
        {
            col_vec[i] = temp(i, j);
        }
        output.setColumn(j, idft1D(col_vec));
    }

    return output;
}

static ComplexMatrix2d dftshift2D(const ComplexMatrix2d &input)
{
    const int M = input.row();
    const int N = input.column();

    ComplexMatrix2d output(M, N);

    const int shiftN = (N + 1) / 2;
    const int shiftM = (M + 1) / 2;

    for (int m = 0; m < M; ++m)
    {
        for (int n = 0; n < N; ++n)
        {
            int newM = (m + shiftM) % M;
            int newN = (n + shiftN) % N;
            output(newM, newN) = input(m, n);
        }
    }
    return output;
}

static ComplexMatrix2d idftshift2D(const ComplexMatrix2d &input)
{
    const int M = input.row();
    const int N = input.column();

    ComplexMatrix2d output(M, N);

    const int shiftM = M / 2;
    const int shiftN = N / 2;

    for (int m = 0; m < M; ++m)
    {
        for (int n = 0; n < N; ++n)
        {
            int newM = (m + shiftM) % M;
            int newN = (n + shiftN) % N;
            output(newM, newN) = input(m, n);
        }
    }
    return output;
}

static ComplexMatrix2d image_to_complex_matrix(const Image &image)
{
    ComplexMatrix2d complexImg(image.row(), image.column());
    for (int i = 0; i < image.row(); ++i)
    {
        for (int j = 0; j < image.column(); ++j)
        {
            complexImg(i, j) = Complex(static_cast<double>(image(i, j)) / 255.0, 0.0);
        }
    }
    return complexImg;
}

static Image complex_matrix_to_image(const ComplexMatrix2d &mat)
{
    const int M = mat.row();
    const int N = mat.column();
    Image output(M, N);

    for (int i = 0; i < M; ++i)
    {
        for (int j = 0; j < N; ++j)
        {
            double value = std::real(mat(i, j));
            value = std::max(0.0, std::min(255.0, value * 255.0));
            output(i, j) = static_cast<unsigned int>(std::round(value));
        }
    }
    return output;
}

ComplexMatrix2d compute_dft(const Image &image)
{
    auto complexImg = image_to_complex_matrix(image);

    return dft2D(complexImg);
}

Image compute_idft(const ComplexMatrix2d &mat)
{
    auto DFT = idft2D(mat);

    return complex_matrix_to_image(DFT);
}

ComplexMatrix2d compute_dftshift(const Image &image)
{
    auto complexImg = image_to_complex_matrix(image);

    return dftshift2D(complexImg);
}

Image compute_idftshift(const ComplexMatrix2d &mat)
{
    auto DTFShift = idftshift2D(mat);

    return complex_matrix_to_image(DTFShift);
}

Image compute_magnitude_spectrum(const ComplexMatrix2d &spectrum)
{
    const int M = spectrum.row();
    const int N = spectrum.column();
    Image magnitude(M, N);

    double maxMagnitude = 0.0;
    for (int m = 0; m < M; ++m)
    {
        for (int n = 0; n < N; ++n)
        {
            double mag = std::abs(spectrum(m, n));
            if (mag > maxMagnitude)
                maxMagnitude = mag;
        }
    }

    for (int m = 0; m < M; ++m)
    {
        for (int n = 0; n < N; ++n)
        {
            double mag = std::abs(spectrum(m, n));
            double logMag = std::log(1.0 + mag);
            double normalized = logMag / std::log(1.0 + maxMagnitude);
            magnitude(m, n) = static_cast<unsigned char>(normalized * 255.0);
        }
    }
    return magnitude;
}

Image compute_phase_spectrum(const ComplexMatrix2d &spectrum)
{
    const int M = spectrum.row();
    const int N = spectrum.column();
    Image phase(M, N);

    for (int m = 0; m < M; ++m)
    {
        for (int n = 0; n < N; ++n)
        {
            double phaseAngle = std::arg(spectrum(m, n));
            double normalized = (phaseAngle + M_PI) / (M_PI * 2.0);
            phase(m, n) = static_cast<unsigned char>(normalized * 255.0);
        }
    }
    return phase;
}
}// namespace DFT
