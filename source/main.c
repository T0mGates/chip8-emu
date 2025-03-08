// common.c included with graphics.h/chip8.h
#include <SDL3/SDL_main.h>
#include <graphics.h>
#include <chip8.h>
#include <time.h>

int main(int argc, char** argv)
{
    IN_FUNC;
    Error retVal = FAIL;

    // Initialize our randomization algo with our seed
    srand(time(NULL));

    // Graphics
    graphicsData    graphicsObj;

    // Chip8 data
    chip8 chip8Obj;

    // Init our graphical window with SDL
    if( SUCCESS == ( retVal = initGraphics( &graphicsObj ) ) )
    {

        // Init our chip8
        if( SUCCESS == ( retVal = initChip8( &chip8Obj ) ) )
        {

            // TODO: Load in a game ROM

            // Emulation loop
            while( true )
            {

            }// while true

            // Event loop
            // Keep listening for event until a quit event has been registered
            while( EVENT_QUIT != listenForEvent( &graphicsObj ) )
            {

            }// while listenForEvent
        }// initChip8

        // Cleanup functions
        retVal = cleanupGraphics( &graphicsObj );
    }// initGraphics

    OUT_FUNC;
    return 0;
}// main