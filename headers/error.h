#ifndef ERROR_H
#define ERROR_H

#include <common.h>

typedef enum
{
    // General
    SUCCESS                 = 0,
    FAIL                    = 1,
    NULL_ARG                = 2,

    // SDL
    SDL_INIT_FAIL           = 10,
    SDL_WINDOW_FAIL         = 11,
    SDL_RENDERER_FAIL       = 12,
    EVENT_QUIT              = 13,

    // CPU
    PC_NO_INCREMENT         = 20
}
Error;

#endif