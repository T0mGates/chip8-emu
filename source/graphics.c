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
    else if( NULL == ( GraphicsObj->window = SDL_CreateWindow( "SDL3 Hello World", 800, 600, 0 ) ) )
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

    SDL_Log("SDL3 Initialized!");

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