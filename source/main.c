// common.c included with graphics.h/chip8.h
#include <SDL3/SDL_main.h>
#include <graphics.h>
#include <chip8.h>
#include <input.h>
#include <time.h>
#include <Windows.h>

int main(int argc, char** argv)
{
    IN_FUNC;
    Error           retVal              = FAIL;

    // Initialize our randomization algo with our seed
    srand(time(NULL));

    // Graphics
    graphicsData    graphicsObj;

    // Chip8 data
    chip8           chip8Obj;

    // Holds num of milliseconds to wait before next cycle/timer update
    double          cycleInterval        = ( 1.0 / INSTRUCTIONS_HZ ) * 1000;
    double          timerInterval        = ( 1.0 / TIMERS_HZ ) * 1000;
    // Holds time in milliseconds, time starts when SDL library is initialized
    double          t                    = SDL_GetTicks();
    // Holds the time at which a new cycle/timer update should be done
    double          nextCycle            = t;
    double          nextTimerUpdate      = t;
    bool            quit                 = false;

    // If we weren't given a .ch8 file as our CLI arg don't bother doing anything, return with error
    if( argc < 2 )
    {
        fprintf( stderr, "Expected a .ch8 file as our first CLI argument. Exiting.\n" );
        return -1;
    }

    // Init our graphical window with SDL
    if( SUCCESS == ( retVal = initGraphics( &graphicsObj ) ) )
    {
        // Init our chip8
        if( SUCCESS == ( retVal = initChip8( &chip8Obj ) ) )
        {
            // Load in a game
            if( SUCCESS == ( retVal = loadGame( &chip8Obj, argv[1] ) ) )
            {
                // Emulation loop
                while( !quit )
                {
                    t = SDL_GetTicks();

                    if( t >= nextCycle )
                    {
                        // Means we can do a new cycle
                        nextCycle   = t + cycleInterval;

                        // Emulate one cycle
                        if( SUCCESS != ( retVal = ( emulateCycle( &chip8Obj ) ) ) )
                        {
                            fprintf( stderr, "Error occured in emulateCycle: %d\n", retVal );
                        }// SUCCESS

                        // If draw flag is set - update the screen
                        if( chip8Obj.updateScreen )
                        {
                            if( SUCCESS != ( retVal = ( drawGraphics( &chip8Obj, &graphicsObj ) ) ) )
                            {
                                fprintf( stderr, "Error occured in drawGraphics: %d\n", retVal );
                            }// SUCCESS

                            chip8Obj.updateScreen   = false;
                        }// updateScreen

                        // Store keys' states
                        if( SUCCESS != ( retVal = ( updateKeyStates( &chip8Obj ) ) ) )
                        {
                            if( EVENT_QUIT == retVal )
                            {
                                quit = true;
                            }
                            else
                            {
                                fprintf( stderr, "Error occured in updateKeyStates: %d\n", retVal );
                            }// EVENT_QUIT

                        }// SUCCESS

                        // Handle timers at a different Hz
                        if( t >= nextTimerUpdate )
                        {
                            // Means we can do a new cycle
                            nextTimerUpdate   = t + timerInterval;

                            // Update timers (decrement if > 0)
                            if( chip8Obj.delayTimer > 0 )
                            {
                                chip8Obj.delayTimer--;
                            }// delayTimer > 0

                            if( chip8Obj.soundTimer > 0 )
                            {
                                chip8Obj.soundTimer--;
                                // If soundTimer was > 0, means we should be beeping
                                DEBUG( "BEEP!\n" );
                                // This is done as soon as soundTimer is set to a value, so no need to handle it here
                            }// soundTImer > 0
                        }// t >= nextTimerUpdate
                    }
                    else
                    {
                        // Don't monopolize the CPU
                        Sleep(0);
                    }
                }// while !quit

                // Event loop
                // Keep listening for event until a quit event has been registered
                //while( EVENT_QUIT != listenForEvent( &graphicsObj ) )
                //{

                //}// while listenForEvent
            }// loadGame
        }// initChip8

        if( GAME_FILE_NOT_FOUND == retVal )
        {
            fprintf( stderr, "File at path: %s does not exist\n", argv[1] );
        }// GAME_FILE_NOT_FOUND

        // Cleanup functions
        retVal = cleanupGraphics( &graphicsObj );
    }// initGraphics

    OUT_FUNC;
    return 0;
}// main