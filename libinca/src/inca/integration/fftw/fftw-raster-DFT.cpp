/*
 * File: fftw-raster-DFT.cpp
 * 
 * Author: Ryan L. Saunders
 *
 * Copyright 2004, Ryan L. Saunders. All rights reserved.
 *
 * Description:
 *      This file implements the actual discrete Fourier transform algorithms
 *      used by the DFT raster operators defined in inca/raster/operators/DFT
 *      using the FFTW library.
 */

// Import the class definitions
#include <inca/raster/operators/DFT>
using namespace inca;
using namespace inca::raster;

// Import the FFTW library
#include <complex>
#include <fftw3.h>


// Locally-defined template functions for copying things around
template <typename scalar>
inline void unpackRaster(scalar * arr, const MultiArrayRaster<scalar, 2> & r) {
    int k = 0;
    Array<IndexType, 2> idx;
    for (idx[0] = r.base(0); idx[0] <= r.extent(0); ++idx[0])
        for (idx[1] = r.base(1); idx[1] <= r.extent(1); ++idx[1])
            arr[k++] = r(idx);
}

template <typename scalar>
inline void repackRaster(MultiArrayRaster<scalar, 2> & r,
                         const scalar * const arr,
                         bool DCInCenter) {
    typedef Array<IndexType, 2> IndexArray;

    // Figure out the dimensions of the output image and how the symmetry
    // works. This is a bit tricky because of the way FFTW handles r2c
    // transforms, omitting (most of) the redundant data.
    SizeType size = r.size(0) * (r.size(1) / 2 + 1);
    IndexArray center((r.size(0) - 1) / 2,   // Center element
                      (r.size(1) - 1) / 2);
    IndexArray center_(center[0] + !(r.size(0) % 2),    // Up one?
                       center[1] + !(r.size(1) % 2));
    cerr << "Going to copy " << size << " elements\n";
    cerr << "Into " << r.size(0) << "x" << r.size(1) << " (" << r.size() << ") raster\n";
    cerr << "Center is " << center[0] << "," << center[1] << endl;
    cerr << "      and " << center_[0] << "," << center_[1] << endl;

    if (DCInCenter) {
        // Copy out, fixing goofy quadrant mapping
        // Also, FFTW doesn't store the original 1 and 2
        //  .........       ---------
        //  ' 2 ' 1 '       | 4 | 3 |
        //  ---------  =>   ---------
        //  | 3 | 4 |       | 1 | 2 |
        //  O--------       O--------
        IndexArray lower, upper;
        lower[0] = center_[0];
        upper[0] = center_[0];
        lower[1] = center[1];
        upper[1] = center_[1];
        for (IndexType k = 0; k < size; ++k) {
            if ((k + 1) % (r.size(1) / 2 + 1) == 0 && r.size(1) % 2 == 0) {
                // Last element of even-sized row...we can ignore this...

            } else {
                // Make sure the indices are valid
                if (lower[0] > r.extent(0) || lower[0] < r.base(0))
                    cerr << k << " -- lower exceeded X-extents " << lower[0] << endl;
                if (upper[0] > r.extent(0) || upper[0] < r.base(0))
                    cerr << k << " -- upper exceeded X-extents " << upper[0] << endl;
                if (lower[1] > r.extent(1) || lower[1] < r.base(1))
                    cerr << k << " -- lower exceeded Y-extents " << lower[1] << endl;
                if (upper[1] > r.extent(1) || upper[1] < r.base(1))
                    cerr << k << " -- upper exceeded Y-extents " << upper[1] << endl;

                // Copy to both quadrants
                r(lower) = arr[k];
                r(upper) = arr[k];
            }

            // Increment our indices
            if ((k + 1) % (r.size(1) / 2 + 1) == 0) {
                lower[0]++;
                lower[1] = center[1];
                upper[0]++;
                upper[1] = center_[1];
            } else {
                lower[1]--;
                upper[1]++;
            }

            // Oooo...time to do the other half
            if (lower[0] > r.extent(0)) {
                lower[0] = r.base(0);
                upper[0] = r.base(0);
                lower[1] = center[1];
                upper[1] = center_[1];
            }
        }
#if 0
        for (int i = 0; i <= cLow[0]; ++i)
            for (int j = 0; j <= cLow[1]; ++j) {
                // Q3 & Q2
                const scalar & s1 = arr[i * (cLow[1] + 2) + j];
                r(cHigh[0] + i, cLow_ + j) = s1;
                r(cHigh[0] + i, cLow - j) = s1;

                // Q4 & Q1
                const scalar & s2 = arr[(nx - i - 1) * (cy + 2) + j];
                r(cx - i, cy_ + j) = s2;
                r(cx - i, cy - j) = s2;
            }
#endif
    } else {
        // Copy out, reflecting redundant quadrants
        // FFTW doesn't store the original 1 and 2
        //  .........       ---------
        //  ' 2 ' 1 '       | 3'| 4'|
        //  ---------  =>   ---------
        //  | 3 | 4 |       | 3 | 4 |
        //  ---------       ---------
        IndexArray upper(r.base(0), r.extent(1)),
                   lower(r.base(0), r.base(1));
        for (IndexType k = 0; k < size; ++k) {
            if ((k + 1) % (r.size(1) / 2 + 1) == 0 && r.size(1) % 2 == 0) {
                // Last element of even-sized row...we can ignore this...

            } else {
                // Make sure the indices are valid
                if (lower[0] > r.extent(0) || lower[0] < r.base(0))
                    cerr << k << " -- lower exceeded X-extents " << lower[0] << endl;
                if (upper[0] > r.extent(0) || upper[0] < r.base(0))
                    cerr << k << " -- upper exceeded X-extents " << upper[0] << endl;
                if (lower[1] > r.extent(1) || lower[1] < r.base(1))
                    cerr << k << " -- lower exceeded Y-extents " << lower[1] << endl;
                if (upper[1] > r.extent(1) || upper[1] < r.base(1))
                    cerr << k << " -- upper exceeded Y-extents " << upper[1] << endl;

                // Copy to both quadrants
                r(lower) = arr[k];
                r(upper) = arr[k];
            }

            // Increment our indices
            if ((k + 1) % (r.size(1) / 2 + 1) == 0) {
                lower[0]++;
                lower[1] = r.base(1);
                upper[0]++;
                upper[1] = r.extent(1);
            } else {
                lower[1]++;
                upper[1]--;
            }
        }
    }
}


// Specializations for float, double, long double (implemented by
// third-party FFT library)
template <>
void inca::raster::calculateDFT(MultiArrayRaster< std::complex<float>, 2> & dest,
                                const MultiArrayRaster< float, 2 > & src,
                                bool DCInCenter) {
    float *in;
    int w = src.size(0),
        h = src.size(1),
        sz = w * h;
    std::complex<float> *out;
    fftwf_plan p;

    in = (float*)fftwf_malloc(sizeof(float) * sz);
    out = (std::complex<float>*)fftwf_malloc(sizeof(std::complex<float>) * sz);
    p = fftwf_plan_dft_r2c_2d(w, h, in, (fftwf_complex*)out, FFTW_ESTIMATE);

    unpackRaster(in, src);

    fftwf_execute(p); /* repeat as needed */

    dest.resize(src.sizes());
    repackRaster(dest, out, DCInCenter);

    fftwf_destroy_plan(p);
    fftwf_free(in); fftwf_free(out);
}

template <>
void inca::raster::calculateDFT(MultiArrayRaster< std::complex<double>, 2> & dest,
                                const MultiArrayRaster< double, 2 > & src,
                                bool DCInCenter) {
    double *in;
    int w = src.size(0),
        h = src.size(1),
        sz = w * h;
    std::complex<double> *out;
    fftw_plan p;

    in = (double*)fftw_malloc(sizeof(double) * sz);
    out = (std::complex<double>*)fftw_malloc(sizeof(std::complex<double>) * sz);
    p = fftw_plan_dft_r2c_2d(w, h, in, (fftw_complex*)out, FFTW_ESTIMATE);

    unpackRaster(in, src);

    fftw_execute(p); /* repeat as needed */

    dest.resize(src.sizes());
    repackRaster(dest, out, DCInCenter);

    fftw_destroy_plan(p);
    fftw_free(in); fftw_free(out);
}

template <>
void inca::raster::calculateDFT(MultiArrayRaster< std::complex<long double>, 2> & dest,
                                const MultiArrayRaster< long double, 2 > & src,
                                bool DCInCenter) {
    long double *in;
    int w = src.size(0),
        h = src.size(1),
        sz = w * h;
    std::complex<long double> *out;
    fftwl_plan p;

    in = (long double*)fftwl_malloc(sizeof(long double) * sz);
    out = (std::complex<long double>*)fftwl_malloc(sizeof(std::complex<long double>) * sz);
    p = fftwl_plan_dft_r2c_2d(w, h, in, (fftwl_complex*)out, FFTW_ESTIMATE);

    unpackRaster(in, src);

    fftwl_execute(p); /* repeat as needed */

    dest.resize(src.sizes());
    repackRaster(dest, out, DCInCenter);

    fftwl_destroy_plan(p);
    fftwl_free(in); fftwl_free(out);
}
