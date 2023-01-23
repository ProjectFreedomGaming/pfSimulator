// SPDX-FileCopyrightText: 2023-present pfSimulator contributors
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "pfCpu.h"
#include "pfSimulator.h"
#include "pfMemory.h"
#include "pfTypes.h"

#include <SDL2/SDL.h>

// -- Types
typedef struct PFCpu
{
    SDL_Thread* thread;

    PFSimulator* simulator;
} PFCpu;

// -- Functions
static int _threadFunction(void* data)
{
    PFCpu* this = (PFCpu*)data;
    
    byte r = 0;
    byte g = 0;
    byte b = 0;

    while (1) {
        pfSimulatorClearDisplay(this->simulator, r, g, b);
        
        r += 12;
        g += 4;
        b += 34;

        pfSimulatorVSyncWait();

        pfSimulatorSwapDisplayBuffer(this->simulator);
    }
    
    return 0;
}

PFCpu* pfCpuNew(PFSimulator* simulator)
{
    if (simulator == NULL) {
        return NULL;
    }
    
    PFCpu* this = pfMemoryCalloc(sizeof(PFCpu));
    if (this == NULL) {
        return NULL;
    }
    
    this->thread = SDL_CreateThread(_threadFunction, "pfCpu", this);
    if (this->thread == NULL) {
        pfCpuDelete(this);
        return NULL;
    }

    this->simulator = simulator;

    return this;
}

void pfCpuDelete(PFCpu* this)
{
    if (this->thread  != NULL) {
        SDL_DetachThread(this->thread);
        this->thread = NULL;
    }
    
    this->simulator = NULL;

    pfMemoryFree(this);
}
