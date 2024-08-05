#ifndef _BASE_TYPES_H_
#define _BASE_TYPES_H_

#if defined __C51__


typedef unsigned char uint8_t;
typedef unsigned short uint16_t;
typedef unsigned long uint32_t;
typedef uint32_t uint64_t[2];

typedef signed char int8_t;
typedef short int16_t;
typedef long int32_t;
typedef int32_t int64_t[2];


typedef uint8_t  U8;
typedef uint16_t U16;
typedef uint32_t U32;

typedef int8_t   S8;
typedef int16_t  S16;
typedef int32_t  S32;

#endif

#endif