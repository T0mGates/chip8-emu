#ifndef CHIP8_H
#define CHIP8_H

#include <common.h>
#include <cpu.h>
#include <windows.h>

typedef struct chip8
{
    // RAM (4KB)
    BYTE        ram[4096];

    // Registers - there are 15 8-bit general purpose registers (V0 -> VE), 16th register is for the 'carry flag'
    BYTE        registers[16];

    // Index register (0x000 - 0xFFF) - 12 bits, but allocate 16
    uint16_t    indexRegister;

    // Program counter (0x000 - 0xFFF) - 12 bits, but allocate 16
    uint16_t    programCounter;

    /*
    Memory Map
        0x000-0x1FF - Chip 8 interpreter (contains font set in emu)
        0x050-0x0A0 - Used for the built in 4x5 pixel font set (0-F)
        0x200-0xFFF - Program ROM and work RAM
    */

    // Black and white, screen is 2048 pixels (64x32)
    // each pixel is 'set' or 'unset'
    // Starts top-left
    BYTE        graphics[64 * 32];

    // Timer registers - count at 60Hz
    // when above 0, count down to 0
    BYTE        delayTimer;
    // System's "buzzer" sounds when sound timer reaches 0
    BYTE        soundTimer;

    // Stack of 16 bytes, to remember where we previously were in memory before jumping somewhere else
    uint16_t    stack[16];
    // Need a way to know where we are in our stack
    uint16_t    stackPointer;

    // Hex based keypad (0x0 - 0xF)
    // use array to store current state of keys (pressed or not pressed)
    bool        keys[16];

    // Flag to update the screen
    bool        updateScreen;
}// chip8
chip8;

// =================================
// FUNCTIONS
// =================================

/*
 * @brief           Initiates a given chip8 struct with proper values. The given struct must be a pointer pointing to a valid chip8 struct
 *
 * @param[out]      Chip8Obj            - the struct to populate with proper default values
 *
 * @return          Error               - Either a success, or some error as defined the the Error enum
*/
Error
    initChip8(OUT chip8* Chip8Obj);

/*
 * @brief           Emulates one cycle of the chip8 cpu for the given chip8 struct
 *
 * @param[inout]    Chip8Obj            - the struct to edit data and read from as we emulate a cycle
 *
 * @return          Error               - Either a success, or some error as defined the the Error enum
*/
Error
    emulateCycle(INOUT chip8* Chip8Obj);

/*
 * @brief           Loads in a given game file
 *
 * @param[in]       GameFilePath        - The path to the game file
 * @param[out]      Chip8Obj            - the struct to edit data for as we read in the game file
 *
 * @return          Error               - Either a success, or some error as defined the the Error enum
*/
Error
    loadGame(OUT chip8* Chip8Obj, IN char* GameFilePath);

/*
 * @brief           Play a beeping sound
 *
 * @param[in]       NumCycles           - How many 'cycles' to play the sound for
 *
 * @return          Error               - Either a success, or some error as defined the the Error enum
*/
Error
    playBeep(IN uint8_t NumCycles);

#endif