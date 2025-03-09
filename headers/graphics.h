#ifndef GRAPHICS_H
#define GRAPHICS_H

// chip8.h includes common.h
#include <chip8.h>
#include <SDL3/SDL.h>

#define PIXEL_SCALER_X      14
#define PIXEL_SCALER_Y      14

#define SET_BIT_COLOR       255, 255, 255
#define UNSET_BIT_COLOR     0, 0, 0

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
 * @brief           Initiates a given graphics struct with a window and renderer. The given struct must be a pointer pointing to a valid graphics struct
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
 * @brief           Renders graphics given the current state of the graphics array found in the given chip8 struct
 *
 * @param[in]       Chip8Obj            - the struct containing the chip8 data
 * @param[out]      GraphicsObj         - the struct containing the rendering/graphical data
 *
 * @return          Error               - Either a success, or some error as defined the the Error enum
*/
Error
    drawGraphics(IN chip8* Chip8Obj, OUT graphicsData* GraphicsObj);

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