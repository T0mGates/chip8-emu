#ifndef COMMON_H
#define COMMON_H

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <error.h>
#include <stdbool.h>

#define IN const
#define INOUT
#define OUT

#ifdef DEBUG

// Undefine before redefining so we can use DEBUG() as a print function that only outputs when DEBUG is defined
#undef DEBUG
#define DEBUG(...) fprintf( stderr, __VA_ARGS__ )

// To be called at the start of every function
#define IN_FUNC DEBUG("IN FUNCTION: %s\n===============\n", __func__)

// To be called at the end of every function (before returning)
#define OUT_FUNC DEBUG("===============\nOUT FUNCTION: %s, retVal = %d\n", __func__, retVal)

#else

#define DEBUG(...)
#define IN_FUNC
#define OUT_FUNC

#endif

typedef uint8_t BYTE;

#endif