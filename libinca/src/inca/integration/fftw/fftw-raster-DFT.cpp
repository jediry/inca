/*
 * File: fftw-raster-DFT.cpp
 *
 * Author: Ryan L. Saunders
 *
 * Copyright 2004, Ryan L. Saunders. All rights reserved.
 *
 * Description:
 *      This file implements the discrete Fourier transform algorithms
 *      used by the DFT raster operators defined in
 *      inca/raster/operators/fourier using the FFTW library.
 */

// Import the class definitions
#include <inca/raster/operators/fourier>
using namespace inca;
using namespace inca::raster;

// Import the FFTW library
#include <complex>
#include <fftw3.h>


namespace inca {
    namespace raster {

// Specializations for IEEE single-precision floating point numbers
#ifdef HAVE_LIBFFTW3F

    // Complex data type
    typedef std::complex<float> cmplx_f;

    // Memory allocation functions
    template <>
    float * dft_memory_allocate<float>(inca::SizeType n) {
        return (float *) fftwf_malloc(sizeof(float) * n);
    }
    template <>
    cmplx_f * dft_memory_allocate<cmplx_f>(inca::SizeType n) {
        return (cmplx_f *) fftwf_malloc(sizeof(cmplx_f) * n);
    }

    // Memory deallocation functions
    template <> void dft_memory_deallocate<cmplx_f>(cmplx_f * p) { fftwf_free(p); }
    template <> void dft_memory_deallocate<float>(float * p) { fftwf_free(p); }

    // Transformation functions
    template <>
    void dft_forward_transform<float, 2>(const Array<SizeType, 2> & sizes,
                                         cmplx_f * out, float const * in) {
        fftwf_plan p = fftwf_plan_dft_r2c_2d(sizes[0], sizes[1],
                                            const_cast<float *>(in),
                                            reinterpret_cast<fftwf_complex *>(out),
                                            FFTW_ESTIMATE);
        fftwf_execute(p);
        fftwf_destroy_plan(p);
    }
    template <>
    void dft_backward_transform<float, 2>(const Array<SizeType, 2> & sizes,
                                          float * out, cmplx_f const * in) {
        fftwf_plan p = fftwf_plan_dft_c2r_2d(sizes[0], sizes[1],
                                            reinterpret_cast<fftwf_complex *>(
                                                const_cast<cmplx_f *>(in)),
                                            out,
                                            FFTW_ESTIMATE);
        fftwf_execute(p);
        fftwf_destroy_plan(p);
    }

#endif


// Specializations for IEEE double-precision floating point numbers
#ifdef HAVE_LIBFFTW3

    // Complex data type
    typedef std::complex<double> cmplx_d;

    // Memory allocation functions
    template <>
    double * dft_memory_allocate<double>(inca::SizeType n) {
        return (double *) fftw_malloc(sizeof(double) * n);
    }
    template <>
    cmplx_d * dft_memory_allocate<cmplx_d>(inca::SizeType n) {
        return (cmplx_d *) fftw_malloc(sizeof(cmplx_d) * n);
    }

    // Memory deallocation functions
    template <> void dft_memory_deallocate<double>(double * p) { fftw_free(p); }
    template <> void dft_memory_deallocate<cmplx_d>(cmplx_d * p) { fftw_free(p); }

    // Transformation functions
    template <>
    void dft_forward_transform<double, 2>(const Array<SizeType, 2> & sizes,
                                          cmplx_d * out, double const * in) {
        fftw_plan p = fftw_plan_dft_r2c_2d(sizes[0], sizes[1],
                                            const_cast<double *>(in),
                                            reinterpret_cast<fftw_complex *>(out),
                                            FFTW_ESTIMATE);
        fftw_execute(p);
        fftw_destroy_plan(p);
    }
    template <>
    void dft_backward_transform<double, 2>(const Array<SizeType, 2> & sizes,
                                           double * out, cmplx_d const * in) {
        fftw_plan p = fftw_plan_dft_c2r_2d(sizes[0], sizes[1],
                                        reinterpret_cast<fftw_complex *>(
                                            const_cast<cmplx_d *>(in)),
                                        out,
                                        FFTW_ESTIMATE);
        fftw_execute(p);
        fftw_destroy_plan(p);
    }

#endif


// Specializations for IEEE long-double-precision floating point numbers
#ifdef HAVE_LIBFFTW3L

    // Complex data type
    typedef std::complex<long double> cmplx_l;

    // Memory allocation functions
    template <>
    long double * dft_memory_allocate<long double>(inca::SizeType n) {
        return (long double *) fftwl_malloc(sizeof(long double) * n);
    }
    template <>
    cmplx_l * dft_memory_allocate<cmplx_l>(inca::SizeType n) {
        return (cmplx_l *) fftwl_malloc(sizeof(cmplx_l) * n);
    }

    // Memory deallocation functions
    template <> void dft_memory_deallocate<long double>(long double * p) { fftwl_free(p); }
    template <> void dft_memory_deallocate<cmplx_l>(cmplx_l * p) { fftwl_free(p); }

    // Transformation functions
    template <>
    void dft_forward_transform<long double, 2>(const Array<SizeType, 2> & sizes,
                                               cmplx_l * out, long double const * in) {
        fftwl_plan p = fftwl_plan_dft_r2c_2d(sizes[0], sizes[1],
                                            const_cast<long double *>(in),
                                            reinterpret_cast<fftwl_complex *>(out),
                                            FFTW_ESTIMATE);
        fftwl_execute(p);
        fftwl_destroy_plan(p);
    }
    template <>
    void dft_backward_transform<long double, 2>(const Array<SizeType, 2> & sizes,
                                                long double * out, cmplx_l const * in) {
        fftwl_plan p = fftwl_plan_dft_c2r_2d(sizes[0], sizes[1],
                                            reinterpret_cast<fftwl_complex *>(
                                                const_cast<cmplx_l *>(in)),
                                            out,
                                            FFTW_ESTIMATE);
        fftwl_execute(p);
        fftwl_destroy_plan(p);
    }

#endif

    }
}

#if 0

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

#endif
