// SPDX-FileCopyrightText: 2023-present pfSimulator contributors
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "pfVSync.h"
#include "pfAssert.h"
#include "pfTypes.h"

#include <SDL2/SDL.h>

// -- Static variables
static SDL_Thread* _thread = NULL;
static size _vsync_count = 0;

// -- Functions
static int _threadFunction(void* data)
{
    while (1) {
        // -- We generate one VSync every 1/50 of a second (Old school PAL for the win :)
        SDL_Delay(1000 / 50);

        ++_vsync_count;
    }
    
    return 0;
}

void pfVsyncInit(void)
{
    if (_thread != NULL) {
        return;
    }
    
    _thread = SDL_CreateThread(_threadFunction, "pfVSync", NULL);
    PF_ASSERT(_thread != NULL);
}

void pfVsyncShutdown(void)
{
    if (_thread != NULL) {
        SDL_DetachThread(_thread);
        _thread = NULL;
    }
}

size pfGetVSyncCount(void)
{
    return _vsync_count;
}

void pfVSyncWait(void)
{
    if (_thread == NULL) {
        return;
    }

    size current_count = _vsync_count;
    while (_vsync_count == current_count) {
        
    }
}

