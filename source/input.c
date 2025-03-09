#include <input.h>

Error
    updateKeyStates(OUT chip8* Chip8Obj)
{
    IN_FUNC;
    Error retVal    = FAIL;

    if( NULL == Chip8Obj )
    {
        DEBUG( "Chip8Obj is NULL!\n" );
        retVal          = NULL_ARG;
    }
    else
    {
        /*
        Reference drawing:

        Keypad                   Keyboard
        +-+-+-+-+                +-+-+-+-+
        |1|2|3|C|                |1|2|3|4|
        +-+-+-+-+                +-+-+-+-+
        |4|5|6|D|                |Q|W|E|R|
        +-+-+-+-+       =>       +-+-+-+-+
        |7|8|9|E|                |A|S|D|F|
        +-+-+-+-+                +-+-+-+-+
        |A|0|B|F|                |Z|X|C|V|
        +-+-+-+-+                +-+-+-+-+

        */

        SDL_Event event;

        // Get all the events since the call to this function
        while( SDL_PollEvent( &event ) )
        {
            switch( event.type )
            {
                case SDL_EVENT_QUIT:
                    printf( "Quit detected\n" );
                    retVal      = EVENT_QUIT;
                    break;

                case SDL_EVENT_KEY_DOWN:
                case SDL_EVENT_KEY_UP:
                    if( event.key.scancode == SDL_SCANCODE_ESCAPE )
                    {
                        if( SDL_EVENT_KEY_DOWN == event.type )
                        {
                            printf( "Quit detected\n" );
                            retVal      = EVENT_QUIT;
                        }// SDL_EVENT_KEY_DOWN
                    }
                    else if( event.key.scancode == SDL_SCANCODE_1 )
                    {
                        // Set to 1 if it was pressed down, else 0
                        Chip8Obj->keys[0x1] = ( event.type == SDL_EVENT_KEY_DOWN );
                    }
                    else if( event.key.scancode == SDL_SCANCODE_2 )
                    {
                        // Set to 1 if it was pressed down, else 0
                        Chip8Obj->keys[0x2] = ( event.type == SDL_EVENT_KEY_DOWN );
                    }
                    else if( event.key.scancode == SDL_SCANCODE_3 )
                    {
                        // Set to 1 if it was pressed down, else 0
                        Chip8Obj->keys[0x3] = ( event.type == SDL_EVENT_KEY_DOWN );
                    }
                    else if( event.key.scancode == SDL_SCANCODE_4 )
                    {
                        // Set to 1 if it was pressed down, else 0
                        Chip8Obj->keys[0xC] = ( event.type == SDL_EVENT_KEY_DOWN );
                    }
                    else if( event.key.scancode == SDL_SCANCODE_Q )
                    {
                        // Set to 1 if it was pressed down, else 0
                        Chip8Obj->keys[0x4] = ( event.type == SDL_EVENT_KEY_DOWN );
                    }
                    else if( event.key.scancode == SDL_SCANCODE_W )
                    {
                        // Set to 1 if it was pressed down, else 0
                        Chip8Obj->keys[0x5] = ( event.type == SDL_EVENT_KEY_DOWN );
                    }
                    else if( event.key.scancode == SDL_SCANCODE_E )
                    {
                        // Set to 1 if it was pressed down, else 0
                        Chip8Obj->keys[0x6] = ( event.type == SDL_EVENT_KEY_DOWN );
                    }
                    else if( event.key.scancode == SDL_SCANCODE_R )
                    {
                        // Set to 1 if it was pressed down, else 0
                        Chip8Obj->keys[0xD] = ( event.type == SDL_EVENT_KEY_DOWN );
                    }
                    else if( event.key.scancode == SDL_SCANCODE_A )
                    {
                        // Set to 1 if it was pressed down, else 0
                        Chip8Obj->keys[0x7] = ( event.type == SDL_EVENT_KEY_DOWN );
                    }
                    else if( event.key.scancode == SDL_SCANCODE_S )
                    {
                        // Set to 1 if it was pressed down, else 0
                        Chip8Obj->keys[0x8] = ( event.type == SDL_EVENT_KEY_DOWN );
                    }
                    else if( event.key.scancode == SDL_SCANCODE_D )
                    {
                        // Set to 1 if it was pressed down, else 0
                        Chip8Obj->keys[0x9] = ( event.type == SDL_EVENT_KEY_DOWN );
                    }
                    else if( event.key.scancode == SDL_SCANCODE_F )
                    {
                        // Set to 1 if it was pressed down, else 0
                        Chip8Obj->keys[0xE] = ( event.type == SDL_EVENT_KEY_DOWN );
                    }
                    else if( event.key.scancode == SDL_SCANCODE_Z )
                    {
                        // Set to 1 if it was pressed down, else 0
                        Chip8Obj->keys[0xA] = ( event.type == SDL_EVENT_KEY_DOWN );
                    }
                    else if( event.key.scancode == SDL_SCANCODE_X )
                    {
                        // Set to 1 if it was pressed down, else 0
                        Chip8Obj->keys[0x0] = ( event.type == SDL_EVENT_KEY_DOWN );
                    }
                    else if( event.key.scancode == SDL_SCANCODE_C )
                    {
                        // Set to 1 if it was pressed down, else 0
                        Chip8Obj->keys[0xB] = ( event.type == SDL_EVENT_KEY_DOWN );
                    }
                    else if( event.key.scancode == SDL_SCANCODE_V )
                    {
                        // Set to 1 if it was pressed down, else 0
                        Chip8Obj->keys[0xF] = ( event.type == SDL_EVENT_KEY_DOWN );
                    }// if key.scancode
                    break;

            }// switch event.type
        }// SDL_PollEvent

        if( retVal != EVENT_QUIT )
        {
            retVal      = SUCCESS;
        }// retVal != EVENT_QUIT
    }// NULL args

    OUT_FUNC;
    return retVal;
}