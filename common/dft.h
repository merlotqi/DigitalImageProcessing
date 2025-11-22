#pragma once

#include "image.h"
#include <complex>

using Complex = std::complex<double>;
using ComplexMatrix1d = std::vector<Complex>;
using ComplexMatrix2d = std::vector<ComplexMatrix1d>;

namespace DFT {

ComplexMatrix2d compute_dft(const Image &image);

Image compute_idft(const ComplexMatrix2d &mat);

ComplexMatrix2d compute_dftshift(const Image &image);

Image compute_idftshift(const ComplexMatrix2d &mat);

Image compute_magnitude_spectrum(const ComplexMatrix2d &spectrum);

Image compute_phase_spectrum(const ComplexMatrix2d &spectrum);

}// namespace DFT
