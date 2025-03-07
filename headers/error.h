#ifndef ERROR_H
#define ERROR_H

#include <common.h>

typedef enum
{
    SUCCESS                 = 0,
    FAIL                    = 1,
    // SDL
    SDL_INIT_FAIL           = 2,
    SDL_WINDOW_FAIL         = 3,
    SDL_RENDERER_FAIL       = 4,
    EVENT_QUIT              = 5
}
Error;

#endif