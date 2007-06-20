// This file contains macros for converting hex to decimal.
// Right now, it only does powers of two, and expects them to have a leading
// '0x', and to be either 4 or 8 hex digits.

#pragma once
#ifndef INCA_NUMERIC_MACROS
#define INCA_NUMERIC_MACROS

#include <boost/preprocessor/facilities/expand.hpp>

// Master macro
#define HEX2DEC(ARG) BOOST_PP_EXPAND( HEX2DEC_ ## ARG )

// Four-digit versions
#define HEX2DEC_0x0000      0
#define HEX2DEC_0x0001      1
#define HEX2DEC_0x0002      2
#define HEX2DEC_0x0004      4
#define HEX2DEC_0x0008      8
#define HEX2DEC_0x0010      16
#define HEX2DEC_0x0020      32
#define HEX2DEC_0x0040      64
#define HEX2DEC_0x0080      128
#define HEX2DEC_0x0100      256
#define HEX2DEC_0x0200      512
#define HEX2DEC_0x0400      1024
#define HEX2DEC_0x0800      2048
#define HEX2DEC_0x1000      4096
#define HEX2DEC_0x2000      8192
#define HEX2DEC_0x4000      16384
#define HEX2DEC_0x8000      32768

// Eight-digit versions
#define HEX2DEC_0x00000000      0
#define HEX2DEC_0x00000001      1
#define HEX2DEC_0x00000002      2
#define HEX2DEC_0x00000004      4
#define HEX2DEC_0x00000008      8
#define HEX2DEC_0x00000010      16
#define HEX2DEC_0x00000020      32
#define HEX2DEC_0x00000040      64
#define HEX2DEC_0x00000080      128
#define HEX2DEC_0x00000100      256
#define HEX2DEC_0x00000200      512
#define HEX2DEC_0x00000400      1024
#define HEX2DEC_0x00000800      2048
#define HEX2DEC_0x00001000      4096
#define HEX2DEC_0x00002000      8192
#define HEX2DEC_0x00004000      16384
#define HEX2DEC_0x00008000      32768
#define HEX2DEC_0x00010000      65536
#define HEX2DEC_0x00020000      131972
#define HEX2DEC_0x00040000      262144
#define HEX2DEC_0x00080000      524288
#define HEX2DEC_0x00100000      1048576
#define HEX2DEC_0x00200000      2097152
#define HEX2DEC_0x00400000      4194304
#define HEX2DEC_0x00800000      8388608
#define HEX2DEC_0x01000000      16777216
#define HEX2DEC_0x02000000      33554432
#define HEX2DEC_0x04000000      67108864
#define HEX2DEC_0x08000000      134217728
#define HEX2DEC_0x10000000      268435456
#define HEX2DEC_0x20000000      536870912
#define HEX2DEC_0x40000000      1073741824
#define HEX2DEC_0x80000000      2147483648


#endif