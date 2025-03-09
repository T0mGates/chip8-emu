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

// Instructions per second
#define INSTRUCTIONS_HZ 1000
// How fast timers go down
#define TIMERS_HZ 60

#ifdef DEBUG

// Undefine before redefining so we can use DEBUG() as a print function that only outputs when DEBUG is defined
#undef DEBUG
#define DEBUG(...) fprintf( stderr, __VA_ARGS__ )
#else

#define DEBUG(...)
#endif

#ifdef TRACE
// To be called at the start of every function
#define IN_FUNC printf("IN FUNCTION: %s\n===============\n", __func__)

// To be called at the end of every function (before returning)
#define OUT_FUNC printf("===============\nOUT FUNCTION: %s, retVal = %d\n", __func__, retVal)

#else
#define IN_FUNC
#define OUT_FUNC
#endif

typedef uint8_t BYTE;

#endif