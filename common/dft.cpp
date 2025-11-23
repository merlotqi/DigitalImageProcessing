#include "dft.h"
#include <algorithm>
#include <math.h>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

namespace DFT {

static ComplexMatrix1d dft1D(const ComplexMatrix1d &input)
{
    const int N = input.size();
    ComplexMatrix1d output(N);

    for (int n = 0; n < N; ++n)
    {
        Complex sum(0.0, 0.0);
        for (int k = 0; k < N; ++k)
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
    const int N = input.size();
    ComplexMatrix1d output(N);
    for (int n = 0; n < N; ++n)
    {
        Complex sum(0.0, 0.0);
        for (int k = 0; k < N; ++k)
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
    const int M = input.size();
    const int N = input.at(0).size();

    ComplexMatrix2d output(M, ComplexMatrix1d(N));
    ComplexMatrix2d temp(M, ComplexMatrix1d(N));

    for (int i = 0; i < M; ++i)
    {
        temp[i] = dft1D(input[i]);
    }

    ComplexMatrix2d transposed(N, ComplexMatrix1d(M));
    for (int u = 0; u < M; ++u)
    {
        for (int v = 0; v < N; ++v)
        {
            transposed[v][u] = temp[u][v];
        }
    }

    for (int i = 0; i < N; ++i)
    {
        auto colDFT = dft1D(transposed[i]);
        for (int j = 0; j < M; ++j)
        {
            output[j][i] = colDFT[j];
        }
    }
    return output;
}

static ComplexMatrix2d idft2D(const ComplexMatrix2d &input)
{
    const int M = input.size();
    const int N = input[0].size();

    ComplexMatrix2d temp(M, ComplexMatrix1d(N));
    ComplexMatrix2d output(M, ComplexMatrix1d(N));

    for (int u = 0; u < M; ++u)
    {
        temp[u] = idft1D(input[u]);
    }

    ComplexMatrix2d transposed(N, ComplexMatrix1d(M));
    for (int u = 0; u < M; ++u)
    {
        for (int v = 0; v < N; ++v)
        {
            transposed[v][u] = temp[u][v];
        }
    }

    for (int v = 0; v < N; ++v)
    {
        auto colIDFT = idft1D(transposed[v]);
        for (int u = 0; u < M; ++u)
        {
            output[u][v] = colIDFT[u];
        }
    }
    return output;
}

static ComplexMatrix2d dftshift2D(const ComplexMatrix2d &input)
{
    const int M = input.size();
    const int N = input[0].size();

    ComplexMatrix2d output(M, ComplexMatrix1d(N));

    const int shiftN = (N + 1) / 2;
    const int shiftM = (M + 1) / 2;

    for (int m = 0; m < M; ++m)
    {
        for (int n = 0; n < N; ++n)
        {
            int newM = (m + shiftM) % M;
            int newN = (n + shiftN) % N;
            output[newM][newN] = input[m][n];
        }
    }
    return output;
}

static ComplexMatrix2d idftshift2D(const ComplexMatrix2d &input)
{
    const int M = input.size();
    const int N = input.size();

    ComplexMatrix2d output(M, ComplexMatrix1d(N));

    const int shiftM = M / 2;
    const int shiftN = N / 2;

    for (int m = 0; m < M; ++m)
    {
        for (int n = 0; n < N; ++n)
        {
            int newM = (m + shiftM) % M;
            int newN = (n + shiftN) % N;
            output[newM][newN] = input[m][n];
        }
    }
    return output;
}

static ComplexMatrix2d image_to_complex_matrix(const Image &image)
{
    ComplexMatrix2d complexImg(image.height(), ComplexMatrix1d(image.width()));
    for (int i = 0; i < image.height(); ++i)
    {
        for (int j = 0; j < image.width(); ++j)
        {
            complexImg[i][j] = Complex(static_cast<double>(image(i, j)) / 255.0, 0.0);
        }
    }
    return complexImg;
}

static Image complex_matrix_to_image(const ComplexMatrix2d &mat)
{
    const int M = mat.size();
    const int N = mat[0].size();
    Image output(M, N);

    for (int i = 0; i < M; ++i)
    {
        for (int j = 0; j < N; ++j)
        {
            double value = std::real(mat[i][j]);
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
    const int M = spectrum.size();
    const int N = spectrum[0].size();
    Image magnitude(M, N);

    double maxMagnitude = 0.0;
    for (int m = 0; m < M; ++m)
    {
        for (int n = 0; n < N; ++n)
        {
            double mag = std::abs(spectrum[m][n]);
            if (mag > maxMagnitude)
                maxMagnitude = mag;
        }
    }

    for (int m = 0; m < M; ++m)
    {
        for (int n = 0; n < N; ++n)
        {
            double mag = std::abs(spectrum[m][n]);
            double logMag = std::log(1.0 + mag);
            double normalized = logMag / std::log(1.0 + maxMagnitude);
            magnitude(m, n) = static_cast<unsigned char>(normalized * 255.0);
        }
    }
    return magnitude;
}

Image compute_phase_spectrum(const ComplexMatrix2d &spectrum)
{
    const int M = spectrum.size();
    const int N = spectrum[0].size();
    Image phase(M, N);

    for (int m = 0; m < M; ++m)
    {
        for (int n = 0; n < N; ++n)
        {
            double phaseAngle = std::arg(spectrum[m][n]);
            double normalized = (phaseAngle + M_PI) / (M_PI * 2.0);
            phase(m, n) = static_cast<unsigned char>(normalized * 255.0);
        }
    }
    return phase;
}

}// namespace DFT
