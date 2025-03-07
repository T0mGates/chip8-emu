// common.c included with graphics.h
#include <graphics.h>
#include <SDL3/SDL_main.h>

int main(int argc, char** argv)
{
    IN_FUNC;
    Error retVal = FAIL;

    // Stores current opcode (2 bytes)
    uint16_t    opcode          = 0;

    // RAM (4KB)
    BYTE        memory[4096];

    // Registers - there are 15 8-bit general purpose registers (V0 -> VE), 16th register is for the 'carry flag'
    BYTE        registers[16];

    // Index register (0x000 - 0xFFF) - 12 bits, but allocate 16
    uint16_t    indexRegister   = 0;

    // Program counter (0x000 - 0xFFF) - 12 bits, but allocate 16
    uint16_t    programCounter  = 0;

    /*
    Memory Map
        0x000-0x1FF - Chip 8 interpreter (contains font set in emu)
        0x050-0x0A0 - Used for the built in 4x5 pixel font set (0-F)
        0x200-0xFFF - Program ROM and work RAM
    */

    // Black and white, screen is 2048 pixels (64x32)
    // each pixel is 'set' or 'unset'
    BYTE        graphics[64 * 32];

    // Timer registers - count at 60Hz
    // when above 0, count down to 0
    BYTE        delayTimer      = 0;
    // System's "buzzer" sounds when sound timer reaches 0
    BYTE        soundTimer      = 0;

    // Stack of 16 bytes, to remember where we previously were in memory before jumping somewhere else
    BYTE        stack[16];
    // Need a way to know where we are in our stack
    BYTE        stackPointer    = 0;

    // Hex based keypad (0x0 - 0xF)
    // use array to store current state of key
    BYTE        key[16];


    // Graphics
    graphicsData    graphicsObj;
    if( SUCCESS == ( retVal = initGraphics( &graphicsObj ) ) )
    {
        while( EVENT_QUIT != listenForEvent( &graphicsObj ) )
        {
            // Keep listening for event until a quit event has been registered
        }

        // Cleanup functions
        retVal = cleanupGraphics( &graphicsObj );
    }

    OUT_FUNC;
    return 0;
}