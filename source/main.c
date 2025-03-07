#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>

#include <common.h>

int main(int argc, char** argv)
{
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


    // SDL
    SDL_Window* window = NULL;
    SDL_Renderer* renderer = NULL;
    int result = SDL_Init( SDL_INIT_VIDEO | SDL_INIT_EVENTS );
    if( result < 0 )
    {
        SDL_Log("SDL_Init error: %s", SDL_GetError());
        return -1;
    }

    window = SDL_CreateWindow("SDL3 Hello World", 800, 600, 0);
    if( NULL == window )
    {
        SDL_Log("SDL_CreateWindow error: %s", SDL_GetError());
        return -2;
    }

    renderer = SDL_CreateRenderer(window, NULL);
    if( NULL == renderer )
    {
        SDL_Log("SDL_CreateRenderer error: %s", SDL_GetError());
        return -3;
    }

    SDL_Log("SDL3 initialized");

    SDL_Event event;
    int quit = 0;
    while( !quit )
    {
        while( SDL_PollEvent( &event ) )
        {
            switch( event.type )
            {
                case SDL_EVENT_QUIT:
                    SDL_Log("SDL3 Event: Quit");
                    quit = 1;
                    break;

                case SDL_EVENT_KEY_DOWN:
                    if( event.key.scancode == SDL_SCANCODE_ESCAPE )
                    {
                        SDL_Log("SDL3 ESC Key: Quit");
                        quit = 1;
                    }
            }
        }

        SDL_SetRenderDrawColor( renderer, 0, 0, 255, 255 );
        SDL_RenderClear( renderer );
        SDL_RenderPresent( renderer );
        // Give control back to OS in case it has to do something
        SDL_Delay( 1 );
    }

    SDL_Log("SDL3 shutdown");
    SDL_DestroyRenderer( renderer );
    SDL_DestroyWindow( window );
    SDL_Quit();

    return 0;
}