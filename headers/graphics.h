#ifndef GRAPHICS_H
#define GRAPHICS_H

#include <common.h>
#include <SDL3/SDL.h>

typedef struct Graphics
{
    SDL_Window*     window;
    SDL_Renderer*   renderer;
}
graphicsData;


// =================================
// FUNCTIONS
// =================================

/*
 * @brief           Initiates a given graphics struct with a window and renderer. The given struct must be a pointer pointing to a vlid graphics struct
 *
 * @param[out]      GraphicsObj         - the struct to populate with a proper window and renderer
 *
 * @return          Error               - Either a success, or some error as defined the the Error enum
*/
Error
    initGraphics(OUT graphicsData* GraphicsObj);

/*
 * @brief           Does what has to be done on program completion. Cleans up the given graphics struct
 *
 * @param[out]      GraphicsObj         - the struct to cleanup, data will not be usable afterwards
 *
 * @return          Error               - Either a success, or some error as defined the the Error enum
*/
Error
    cleanupGraphics(OUT graphicsData* GraphicsObj);

/*
 * @brief           Hangs until an event is received, will then do an action depending on the received event
 *
 * @param[out]      GraphicsObj         - the struct that contains a valid window/renderer to be used
 *
 * @return          Error               - Either a success, or some error as defined the the Error enum
*/
Error
    listenForEvent(INOUT graphicsData* GraphicsObj);

#endif