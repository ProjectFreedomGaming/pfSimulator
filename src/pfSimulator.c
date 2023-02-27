// SPDX-FileCopyrightText: 2023-present pfSimulator contributors
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "pfSimulator.h"
#include "pfCpu.h"
#include "pfMmu.h"
#include "pfPfx1.h"
#include "pfVSync.h"
#include "pfAssert.h"
#include "pfMemory.h"

#include <SDL2/SDL.h>

// -- Constants
#define PF_SIMULATOR_SCREEN_SIZE_X      800
#define PF_SIMULATOR_SCREEN_SIZE_Y      480

// -- Types
typedef struct PFSimulator
{
    // -- Main rendering window
    SDL_Window* window;

    // -- Renderer that is attached to the window
    SDL_Renderer* renderer;
    
    // -- CPU that runs the game code
    PFCpu *cpu;

    // -- Memory management unit
    PFMmu* mmu;

    // -- Graphics chip that does the dirty work
    PFPfx1* gfx;

} PFSimulator;

// -- Functions
bool pfSimulatorInit(void)
{
    // -- Init the SDL subsystem
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
        printf("error initializing SDL: %s\n", SDL_GetError());
        return false;
    }
    
    // -- Init our VSync simulator
    pfVsyncInit();
    
    return true;
}

void pfSimulatorShutdown(void)
{
    // -- Shutdown our VSync simulator
    pfVsyncShutdown();
    
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
    PFSimulator* this = pfMemoryCalloc(sizeof(PFSimulator));
    PF_ASSERT(this != NULL);

    // -- Create the simulator's main window
    this->window = SDL_CreateWindow("pfSimulator",
                                    SDL_WINDOWPOS_CENTERED,
                                    SDL_WINDOWPOS_CENTERED,
                                    PF_SIMULATOR_SCREEN_SIZE_X,
                                    PF_SIMULATOR_SCREEN_SIZE_Y,
                                    0);
    PF_ASSERT(this->window != NULL);
    
    this->renderer = SDL_CreateRenderer(this->window, -1, 0);
    PF_ASSERT(this->renderer != NULL);
    
    this->mmu = pfMmuNew();
    PF_ASSERT(this->mmu != NULL);

    this->gfx = pfPfx1New(this, this->mmu);
    PF_ASSERT(this->gfx != NULL);

    this->cpu = pfCpuNew(this->mmu);
    PF_ASSERT(this->cpu != NULL);
    
    return this;
}

void pfSimulatorDelete(PFSimulator* this)
{
    if (this->window != NULL) {
        SDL_DestroyWindow(this->window);
        this->window = NULL;
    }
    
    if (this->renderer != NULL) {
        SDL_DestroyRenderer(this->renderer);
        this->renderer = NULL;
    }
    
    if (this->cpu != NULL) {
        pfCpuDelete(this->cpu);
        this->cpu = NULL;
    }
    
    if (this->gfx != NULL) {
        pfPfx1Delete(this->gfx);
        this->gfx = NULL;
    }
    
    if (this->mmu != NULL) {
        pfMmuDelete(this->mmu);
        this->mmu = NULL;
    }
    
    pfMemoryFree(this);
}

size pfSimulatorGetVSyncCount(void)
{
    return pfGetVSyncCount();
}

void pfSimulatorClearDisplay(PFSimulator* this, byte r, byte g, byte b)
{
    SDL_SetRenderDrawColor(this->renderer, r, g, b, 255);
    SDL_RenderClear(this->renderer);
}

void pfSimulatorSwapDisplayBuffer(PFSimulator* this)
{
    SDL_RenderPresent(this->renderer);
}
