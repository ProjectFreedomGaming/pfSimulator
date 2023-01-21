// SPDX-FileCopyrightText: 2023-present pfSimulator contributors
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "pfMain.h"

#include <SDL3/SDL.h>

// -- This is our main rendering window
static SDL_Window* window = NULL;

// -- Setup up the simulator
void pfMain(void)
{
    // -- Init the SDL subsystem
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
        printf("error initializing SDL: %s\n", SDL_GetError());
    }
    
    // -- Create our main window
    window = SDL_CreateWindow("pfSimulator",
                              SDL_WINDOWPOS_CENTERED,
                              SDL_WINDOWPOS_CENTERED,
                              800, 480, 0);

    // -- The do nothing until the app quits
    while (1) {
        SDL_Event event;
        SDL_WaitEvent(&event);
        if (event.type == SDL_QUIT) {
            return;
        }
    }
}

// -- Clean up our mess before exiting
void pfTerminate(void)
{
    if (window != NULL) {
        SDL_DestroyWindow(window);
        window = NULL;
    }
    
    SDL_Quit();
}
