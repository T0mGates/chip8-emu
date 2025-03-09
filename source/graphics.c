#include <graphics.h>

Error
    initGraphics(OUT graphicsData* GraphicsObj)
{
    IN_FUNC;

    // Assume failure
    Error retVal        = FAIL;

    if( NULL == GraphicsObj )
    {
        DEBUG( "GraphicsObj is NULL!\n" );
        retVal          = NULL_ARG;
    }
    else if( 0 > SDL_Init( SDL_INIT_VIDEO | SDL_INIT_EVENTS ) )
    {
        retVal  = SDL_INIT_FAIL;
    }
    else if( NULL == ( GraphicsObj->window = SDL_CreateWindow( "Chip8 Emu", 64 * PIXEL_SCALER_X, 32 * PIXEL_SCALER_Y, 0 ) ) )
    {
        retVal  = SDL_WINDOW_FAIL;
    }
    else if( NULL == ( GraphicsObj->renderer = SDL_CreateRenderer( GraphicsObj->window, NULL ) ) )
    {
        retVal  = SDL_RENDERER_FAIL;
    }
    else
    {
        retVal  = SUCCESS;
    }

    SDL_Log("SDL3 initialized");

    OUT_FUNC;
    return retVal;
}

Error
    cleanupGraphics(OUT graphicsData* GraphicsObj)
{
    IN_FUNC;
    Error retVal    = FAIL;

    if( NULL == GraphicsObj )
    {
        DEBUG( "GraphicsObj is NULL!\n" );
        retVal          = NULL_ARG;
    }
    else
    {
        // Start destroying and cleaning up objects
        SDL_Log("SDL3 shutdown initiated");
        SDL_DestroyRenderer( GraphicsObj->renderer );
        SDL_DestroyWindow( GraphicsObj->window );
        SDL_Quit();

        retVal          = SUCCESS;
    }

    OUT_FUNC;
    return retVal;
}

Error
    drawGraphics(IN chip8* Chip8Obj, OUT graphicsData* GraphicsObj)
{
    IN_FUNC;
    Error retVal            = FAIL;

    // Will be used to index our graphics array
    unsigned int gfxIndex   = 0;

    if( NULL == Chip8Obj )
    {
        DEBUG( "Chip8Obj is NULL!\n" );
        retVal          = NULL_ARG;
    }
    else if( NULL == GraphicsObj )
    {
        DEBUG( "GraphicsObj is NULL!\n" );
        retVal          = NULL_ARG;
    }
    else
    {
        // Set screen to the 'set bit' color, then fill in set bits with the 'set bit' color
        SDL_SetRenderDrawColor( GraphicsObj->renderer, UNSET_BIT_COLOR, 255 );
        // Clear the screen, set to RenderDrawColor's color
        SDL_RenderClear( GraphicsObj->renderer );

        // Set the next color to be the 'set bit' color
        SDL_SetRenderDrawColor( GraphicsObj->renderer, SET_BIT_COLOR, 255 );
        // Fill in set bits with black
        for( unsigned int x = 0; x < 64; x++ )
        {
            for( unsigned int y = 0; y < 32; y++ )
            {
                gfxIndex    = ( y * 64 ) + x;

                if( 1 == Chip8Obj->graphics[gfxIndex] )
                {
                    // bit is set, draw it (don't forget our scalers)
                    for( unsigned int xScale = 0; xScale < PIXEL_SCALER_X; xScale++ )
                    {
                        for( unsigned int yScale = 0; yScale < PIXEL_SCALER_Y; yScale++ )
                        {
                            // Can mess with +/- on the scales... works differently with different games I find
                            SDL_RenderPoint(  GraphicsObj->renderer,
                                                ( x * PIXEL_SCALER_X )  + xScale,
                                                ( y * PIXEL_SCALER_Y )  + yScale
                            );
                        }// for yScale
                    }// for xScale
                }// bit is set
            }// for y
        }// for x

        // Update the screen
        SDL_RenderPresent( GraphicsObj->renderer );

        // Give control back to OS in case it has to do something
        SDL_Delay( 1 );

        retVal          = SUCCESS;
    }// NULL args

    OUT_FUNC;
    return retVal;
}


// Not used
Error
    listenForEvent(INOUT graphicsData* GraphicsObj)
{
    IN_FUNC;
    Error retVal    = FAIL;

    SDL_Event event;
    bool quit        = false;

    if( NULL == GraphicsObj )
    {
        DEBUG( "GraphicsObj is NULL!\n" );
        retVal          = NULL_ARG;
    }
    else
    {
        // Wait until input is gathered
        while( !quit )
        {
            while( SDL_PollEvent( &event ) )
            {
                switch( event.type )
                {
                    case SDL_EVENT_QUIT:
                        SDL_Log("SDL3 Event: Quit");
                        quit = true;
                        break;

                    case SDL_EVENT_KEY_DOWN:
                        if( event.key.scancode == SDL_SCANCODE_ESCAPE )
                        {
                            SDL_Log("SDL3 ESC Key: Quit");
                            quit = true;
                        }// ESC pressed
                        break;
                }// switch event.type
            }// SDL_PollEvent

            SDL_SetRenderDrawColor( GraphicsObj->renderer, 0, 0, 255, 255 );
            SDL_RenderClear( GraphicsObj->renderer );
            SDL_RenderPresent( GraphicsObj->renderer );

            // Give control back to OS in case it has to do something
            SDL_Delay( 1 );
        }// while

        if( true == quit )
        {
            retVal  = EVENT_QUIT;
        }
        else
        {
            retVal  = SUCCESS;
        }// true == quit
    }// NULL args

    OUT_FUNC;
    return retVal;
}