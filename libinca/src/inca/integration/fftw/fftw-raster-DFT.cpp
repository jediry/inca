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
    cerr << "Unpacked " << k << " elements\n";
}

// Locally-defined template functions for copying things around
template <typename scalar, typename scalar2>
inline void repackRaster(MultiArrayRaster<scalar, 2> & r, const scalar * arr,
                         scalar2 normalizationFactor) {
    int k = 0;
    Array<IndexType, 2> idx;
    for (idx[0] = r.base(0); idx[0] <= r.extent(0); ++idx[0])
        for (idx[1] = r.base(1); idx[1] <= r.extent(1); ++idx[1])
            r(idx) = arr[k++] * normalizationFactor;
    cerr << "Repacked " << k << " elements\n";
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


// DFT Specializations for float, double, and long double
template <>
void inca::raster::calculateDFT(MultiArrayRaster< std::complex<float>, 2> & dest,
                                const MultiArrayRaster< float, 2 > & src) {
    float *in;
    std::complex<float> *out;
    fftwf_plan p;
    int inW = src.size(0),
        inH = src.size(1),
        outW = inW,
        outH = (inH/2 + 1);
    float normalizationFactor = 1.0f;// / std::sqrt((float)inW * inH);

    in = (float*)fftwf_malloc(sizeof(float) * inW * inH);
    out = (std::complex<float>*)fftwf_malloc(sizeof(std::complex<float>) * outW * outH);
    p = fftwf_plan_dft_r2c_2d(inW, inH, in, (fftwf_complex*)out, FFTW_ESTIMATE);

    unpackRaster(in, src);

    fftwf_execute(p); /* repeat as needed */

    dest.resize(outW, outH);
    repackRaster(dest, out, normalizationFactor);

    fftwf_destroy_plan(p);
    fftwf_free(in); fftwf_free(out);
}

template <>
void inca::raster::calculateDFT(MultiArrayRaster< std::complex<double>, 2> & dest,
                                const MultiArrayRaster< double, 2 > & src) {
    double *in;
    std::complex<double> *out;
    fftw_plan p;
    int inW = src.size(0),
        inH = src.size(1),
        outW = inW,
        outH = (inH/2 + 1);
    double normalizationFactor = 1 / std::sqrt((double)inW * inH);

    in = (double*)fftw_malloc(sizeof(double) * inW * inH);
    out = (std::complex<double>*)fftw_malloc(sizeof(std::complex<double>) * outW * outH);
    p = fftw_plan_dft_r2c_2d(inW, inH, in, (fftw_complex*)out, FFTW_ESTIMATE);

    unpackRaster(in, src);

    fftw_execute(p); /* repeat as needed */

    dest.resize(outW, outH);
    repackRaster(dest, out, normalizationFactor);

    fftw_destroy_plan(p);
    fftw_free(in); fftw_free(out);
}

template <>
void inca::raster::calculateDFT(MultiArrayRaster< std::complex<long double>, 2> & dest,
                                const MultiArrayRaster< long double, 2 > & src) {
    long double *in;
    std::complex<long double> *out;
    fftwl_plan p;
    int inW = src.size(0),
        inH = src.size(1),
        outW = inW,
        outH = (inH/2 + 1);
    long double normalizationFactor = 1 / std::sqrt((long double)inW * inH);

    in = (long double*)fftwl_malloc(sizeof(long double) * inW * inH);
    out = (std::complex<long double>*)fftwl_malloc(sizeof(std::complex<long double>) * outW * outH);
    p = fftwl_plan_dft_r2c_2d(inW, inH, in, (fftwl_complex*)out, FFTW_ESTIMATE);

    unpackRaster(in, src);

    fftwl_execute(p); /* repeat as needed */

    dest.resize(outW, outH);
    repackRaster(dest, out, normalizationFactor);

    fftwl_destroy_plan(p);
    fftwl_free(in); fftwl_free(out);
}


// Inverse DFT Specializations for float, double, and long double
template <>
void inca::raster::calculateInverseDFT(MultiArrayRaster< float, 2> & dest,
                                 const MultiArrayRaster< std::complex<float>, 2 > & src) {
    std::complex<float> *in;
    float *out;
    fftwf_plan p;
    int inW = src.size(0),
        inH = src.size(1),
        outW = inW,
        outH = (inH - 1) * 2;
    float normalizationFactor = 1 / ((float)outW * outH);

    in = (std::complex<float>*)fftwf_malloc(sizeof(std::complex<float>) * inW * inH);
    out = (float*)fftwf_malloc(sizeof(float) * outW * outH);
    p = fftwf_plan_dft_c2r_2d(outW, outH, (fftwf_complex*)in, out, FFTW_ESTIMATE);

    unpackRaster(in, src);

    fftwf_execute(p); /* repeat as needed */

    dest.resize(outW, outH);
    repackRaster(dest, out, normalizationFactor);

    fftwf_destroy_plan(p);
    fftwf_free(in); fftwf_free(out);
}

