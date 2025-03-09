#ifndef INPUT_H
#define INPUT_H

// chip8.h includes common.h so don't need to include it
#include <chip8.h>
#include <SDL3/SDL.h>

// =================================
// FUNCTIONS
// =================================

/*
 * @brief           Sets the given chip8's keys state
 *
 * @param[out]      Chip8Obj            - the chip8 struct to manipulate based off the current state of keys
 *
 * @return          Error               - Either a success, or some error as defined the the Error enum
*/
Error
    updateKeyStates(OUT chip8* Chip8Obj);

#endif