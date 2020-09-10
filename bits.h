#ifndef BITS_H
#define BITS_H

#include <stdint.h>

#define N 32

#if N == 8
#define UINT uint8_t
#define BIG_UINT uint16_t
#elif N == 16
#define UINT uint16_t
#define BIG_UINT uint32_t
#elif N == 32
#define UINT uint32_t
#define BIG_UINT uint64_t
#else
#error This bit size is not supported.
#endif

typedef UINT uint;
typedef BIG_UINT big_uint;

#define MAX ((uint)-1)

#endif