// SPDX-FileCopyrightText: 2023-present pfSimulator contributors
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "pfSimulator.h"
#include "pfMemory.h"

#include <SDL2/SDL.h>

// -- Constants
#define PF_SIMULATOR_SCREEN_SIZE_X      800
#define PF_SIMULATOR_SCREEN_SIZE_Y      480

// -- Types
typedef struct PFSimulator
{
    // -- This is our main rendering window
    SDL_Window* window;

} PFSimulator;

// -- Functions
bool pfSimulatorInit(void)
{
    // -- Init the SDL subsystem
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
        printf("error initializing SDL: %s\n", SDL_GetError());
        return FALSE;
    }
    
    return TRUE;
}

void pfSimulatorShutdown(void)
{
    SDL_Quit();
}

PFSimulatorUpdateStatus pfSimulatorUpdate(void)
{
    SDL_Event event;
    SDL_WaitEvent(&event);

    if (event.type == SDL_QUIT) {
        return kMustQuit;
    }
    
    return kNone;
}

PFSimulator* pfSimulatorNew(void)
{
    PFSimulator* this = pfMemoryAlloc(sizeof(PFSimulator));

    // -- Create the simulator's main window
    this->window = SDL_CreateWindow("pfSimulator",
                                    SDL_WINDOWPOS_CENTERED,
                                    SDL_WINDOWPOS_CENTERED,
                                    PF_SIMULATOR_SCREEN_SIZE_X,
                                    PF_SIMULATOR_SCREEN_SIZE_Y,
                                    0);
    if (this->window == NULL) {
        pfSimulatorDelete(this);
        return NULL;
    }

    return this;
}

void pfSimulatorDelete(PFSimulator* this)
{
    if (this->window != NULL) {
        SDL_DestroyWindow(this->window);
        this->window = NULL;
    }
    
    pfMemoryFree(this);
}
